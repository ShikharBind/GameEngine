#include "shpch.h"

#include "Scotch/Core/LayerStack.h"
#include "Scotch/Renderer/FrameBuffer.h"
#include "Scotch/Renderer/Renderer.h"
#include "Scotch/Renderer/Renderer2D.h"
#include "Scotch/Scene/Entity.h"
#include "Scotch/Scene/SceneSerializer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <yaml-cpp/yaml.h>

#include <cmath>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;
using namespace Scotch;

namespace
{
    void Require(bool condition, const std::string& message)
    {
        if (!condition)
            throw std::runtime_error(message);
    }

    void Pass(const char* message)
    {
        std::cout << "PASS: " << message << '\n';
    }

    bool Near(float actual, float expected)
    {
        return std::abs(actual - expected) < 0.0001f;
    }

    void WriteFixture(const fs::path& path, const std::string& text)
    {
        std::ofstream out(path);
        out << text;
        out.close();
        Require(static_cast<bool>(out), "Cannot write test fixture: " + path.string());
    }

    void TestScenes(const fs::path& output, const fs::path& examples)
    {
        auto source = CreateRef<Scene>();
        source->OnViewportResize(800, 600);
        auto entity = source->CreateEntity("Smoke: sprite and camera");
        auto& transform = entity.GetComponent<TransformComponent>();
        transform.Translation = { 1.25f, -2.5f, 3.75f };
        transform.Rotation = { 0.25f, -0.5f, 0.75f };
        transform.Scale = { 2.0f, 3.0f, 4.0f };
        entity.AddComponent<SpriteRendererComponent>(glm::vec4(0.25f, 0.5f, 0.75f, 1.0f));
        auto& camera = entity.AddComponent<CameraComponent>();
        camera.Camera.SetOrthographic(13.5f, -2.0f, 25.0f);
        camera.Camera.SetPerspective(0.75f, 0.2f, 200.0f);
        camera.FixedAspectRatio = true;

        const auto first = output / "roundtrip-first.scotch";
        const auto second = output / "roundtrip-second.scotch";
        SceneSerializer sourceSerializer(source);
        Require(sourceSerializer.Serialize(first.string()), "Initial scene save failed");

        auto restored = CreateRef<Scene>();
        restored->OnViewportResize(800, 600);
        SceneSerializer restoredSerializer(restored);
        Require(restoredSerializer.Deserialize(first.string()), "Scene roundtrip load failed");
        auto loaded = restored->GetPrimaryCamera();
        Require(static_cast<bool>(loaded), "Primary camera was not restored");
        Require(loaded.GetComponent<TagComponent>().Tag == "Smoke: sprite and camera", "Tag changed");
        const auto& loadedTransform = loaded.GetComponent<TransformComponent>();
        Require(glm::length(loadedTransform.Translation - transform.Translation) < 0.0001f, "Translation changed");
        Require(glm::length(loadedTransform.Rotation - transform.Rotation) < 0.0001f, "Rotation changed");
        Require(glm::length(loadedTransform.Scale - transform.Scale) < 0.0001f, "Scale changed");
        Require(glm::length(loaded.GetComponent<SpriteRendererComponent>().Color -
            entity.GetComponent<SpriteRendererComponent>().Color) < 0.0001f, "Sprite color changed");
        const auto& loadedCamera = loaded.GetComponent<CameraComponent>();
        Require(loadedCamera.Primary && loadedCamera.FixedAspectRatio, "Camera flags changed");
        Require(loadedCamera.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective,
            "Camera projection type changed");
        Require(Near(loadedCamera.Camera.GetPerspectiveVerticalFOV(), 0.75f) &&
            Near(loadedCamera.Camera.GetPerspectiveNearClip(), 0.2f) &&
            Near(loadedCamera.Camera.GetPerspectiveFarClip(), 200.0f) &&
            Near(loadedCamera.Camera.GetOrthographicSize(), 13.5f) &&
            Near(loadedCamera.Camera.GetOrthographicNearClip(), -2.0f) &&
            Near(loadedCamera.Camera.GetOrthographicFarClip(), 25.0f), "Camera settings changed");
        Require(restoredSerializer.Serialize(second.string()), "Roundtrip re-save failed");
        Require(YAML::Dump(YAML::LoadFile(first.string())) == YAML::Dump(YAML::LoadFile(second.string())),
            "Normalized scene data changed on roundtrip");
        Pass("scene tag, transform, sprite and camera roundtrip");

        for (const char* filename : { "Example.scotch", "Example2.scotch", "CheckNewSave.scotch" })
        {
            auto example = CreateRef<Scene>();
            example->OnViewportResize(800, 600);
            SceneSerializer serializer(example);
            Require(serializer.Deserialize((examples / filename).string()), std::string("Existing scene failed: ") + filename);
            Require(serializer.Serialize((output / filename).string()), "Existing scene could not be saved");
        }
        Pass("all three existing example scenes load and save");

        const std::pair<const char*, const char*> failures[] = {
            { "syntax.scotch", "Scene: Invalid\nEntities: [\n" },
            { "conversion.scotch", "Scene: Invalid\nEntities:\n  - Entity: 1\n    TransformComponent:\n      Translation: [bad, 0, 0]\n      Rotation: [0, 0, 0]\n      Scale: [1, 1, 1]\n" },
            { "root.scotch", "- not a scene\n" },
            { "missing-scene.scotch", "Entities: []\n" },
            { "entities.scotch", "Scene: Invalid\nEntities: scalar\n" }
        };
        for (const auto& fixture : failures)
        {
            const auto path = output / fixture.first;
            WriteFixture(path, fixture.second);
            auto invalid = CreateRef<Scene>();
            Require(!SceneSerializer(invalid).Deserialize(path.string()),
                std::string("Invalid scene was accepted: ") + fixture.first);
        }
        auto missing = CreateRef<Scene>();
        Require(!SceneSerializer(missing).Deserialize((output / "nonexistent" / "scene.scotch").string()),
            "Missing scene was accepted");
        Require(!sourceSerializer.Serialize(output.string()), "Saving over a directory should fail");
        Require(!sourceSerializer.Serialize((output / "nonexistent" / "scene.scotch").string()),
            "Saving into a missing directory should fail");
        Pass("malformed, invalid, missing and unwritable scene paths return failure");
    }

    void TestLayerCleanup()
    {
        struct TrackingLayer : Layer
        {
            int& Detaches;
            int& Destructions;
            bool& OrderValid;
            bool Detached = false;

            TrackingLayer(int& detaches, int& destructions, bool& orderValid)
                : Detaches(detaches), Destructions(destructions), OrderValid(orderValid) {}
            void OnDetach() override { Detached = true; ++Detaches; }
            ~TrackingLayer() override { OrderValid = OrderValid && Detached; ++Destructions; }
        };
        int detaches = 0, destructions = 0;
        bool orderValid = true;
        {
            LayerStack stack;
            stack.PushLayer(new TrackingLayer(detaches, destructions, orderValid));
            stack.PushOverlay(new TrackingLayer(detaches, destructions, orderValid));
            stack.Clear();
            stack.Clear();
        }
        Require(detaches == 2 && destructions == 2 && orderValid, "Layer cleanup order or ownership failed");
        Pass("layers detach once before destruction; repeated clear is harmless");
    }

    void CheckGL(const char* operation)
    {
        const GLenum error = glGetError();
        Require(error == GL_NO_ERROR, std::string(operation) + " produced OpenGL error " + std::to_string(error));
    }

    struct GraphicsSession
    {
        GLFWwindow* Window = nullptr;
        bool GlfwReady = false;
        bool RendererReady = false;

        void Initialize()
        {
            glfwSetErrorCallback([](int code, const char* description) {
                std::cerr << "GLFW " << code << ": " << description << '\n';
            });
            GlfwReady = glfwInit() == GLFW_TRUE;
            Require(GlfwReady, "GLFW initialization failed");
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            Window = glfwCreateWindow(128, 128, "Scotch smoke tests", nullptr, nullptr);
            Require(Window != nullptr, "OpenGL 4.5 hidden window creation failed");
            glfwMakeContextCurrent(Window);
            Require(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) != 0, "GLAD initialization failed");
            Require(GLAD_GL_VERSION_4_5 != 0, "OpenGL 4.5 is required");
            GLint textureUnits = 0;
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
            Require(textureUnits >= 32, "Current renderer shader requires 32 fragment texture units");
            std::cout << "GPU: " << glGetString(GL_RENDERER) << "; OpenGL " << glGetString(GL_VERSION) << '\n';
            Renderer::Init();
            RendererReady = true;
            CheckGL("renderer initialization");
        }

        ~GraphicsSession()
        {
            if (RendererReady)
                Renderer::Shutdown();
            if (Window)
                glfwDestroyWindow(Window);
            if (GlfwReady)
                glfwTerminate();
        }
    };

    void TestGraphics()
    {
        GraphicsSession graphics;
        graphics.Initialize();
        FrameBufferSpecification spec;
        spec.Width = spec.Height = 128;
        spec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER,
            FramebufferTextureFormat::Depth };
        auto framebuffer = FrameBuffer::Create(spec);
        framebuffer->Bind();
        OrthographicCamera camera(-1.0f, 1.0f, -1.0f, 1.0f);

        Renderer2D::ResetStats();
        Renderer2D::BeginScene(camera);
        Renderer2D::EndScene();
        Require(Renderer2D::GetStats().DrawCalls == 0, "Empty batch issued a draw call");
        CheckGL("empty batch");
        Pass("empty renderer batch issues no draw calls");

        std::vector<Ref<Texture2D>> textures;
        std::vector<uint32_t> textureIDs;
        for (uint32_t index = 0; index < 33; ++index)
        {
            auto texture = Texture2D::Create(1, 1);
            uint32_t color = 0xff000000u | (index + 1) * 0x00050301u;
            texture->SetData(&color, sizeof(color));
            textureIDs.push_back(texture->GetRendererID());
            textures.push_back(texture);
        }
        for (bool useSubtextures : { false, true })
        {
            RenderCommand::Clear();
            framebuffer->ClearAttachment(1, -1);
            Renderer2D::ResetStats();
            Renderer2D::BeginScene(camera);
            for (size_t index = 0; index < textures.size(); ++index)
            {
                const auto& texture = textures[index];
                const glm::vec3 position(-0.875f + (index % 8) * 0.25f,
                    -0.875f + (index / 8) * 0.25f, 0.0f);
                const auto transform = glm::translate(glm::mat4(1.0f), position) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
                if (useSubtextures)
                    Renderer2D::DrawQuad(transform, SubTexture2D::CreateFromCoords(texture, { 0, 0 }, { 1, 1 }),
                        1.0f, glm::vec4(1.0f), 100 + static_cast<int>(index));
                else
                    Renderer2D::DrawQuad(transform, texture, 1.0f, glm::vec4(1.0f), 100 + static_cast<int>(index));
            }
            Renderer2D::EndScene();
            const auto stats = Renderer2D::GetStats();
            Require(stats.DrawCalls == 2 && stats.QuadCount == 33,
                useSubtextures ? "Subtexture batch overflow handling failed" : "Texture batch overflow handling failed");
            for (int index = 0; index < 33; ++index)
            {
                const int x = 8 + (index % 8) * 16;
                const int y = 8 + (index / 8) * 16;
                Require(framebuffer->ReadPixel(1, x, y) == 100 + index, "A batched quad was not rendered");
                std::array<uint8_t, 4> color{};
                glReadBuffer(GL_COLOR_ATTACHMENT0);
                glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color.data());
                Require(color[0] == index + 1 && color[1] == (index + 1) * 3 &&
                    color[2] == (index + 1) * 5 && color[3] == 255, "A batched quad used the wrong texture");
            }
            CheckGL(useSubtextures ? "subtexture batching" : "texture batching");
        }
        Pass("33 distinct textures render correct colors/IDs in two draws through both textured paths");

        RenderCommand::Clear();
        framebuffer->ClearAttachment(1, -1);
        Renderer2D::BeginScene(camera);
        Renderer2D::DrawQuad(glm::mat4(1.0f), glm::vec4(1.0f), 42);
        Renderer2D::EndScene();
        Require(framebuffer->ReadPixel(1, 64, 64) == 42, "Rendered entity ID picking failed");
        Require(framebuffer->ReadPixel(1, 0, 0) == -1, "Background should contain no entity");
        Require(framebuffer->ReadPixel(1, -1, 0) == -1 && framebuffer->ReadPixel(1, 0, -1) == -1 &&
            framebuffer->ReadPixel(1, 128, 0) == -1 && framebuffer->ReadPixel(1, 0, 128) == -1,
            "Out-of-bounds picking was not rejected");
        framebuffer->Resize(64, 96);
        framebuffer->Bind();
        framebuffer->ClearAttachment(1, 73);
        Require(framebuffer->ReadPixel(1, 63, 95) == 73, "Framebuffer resize or valid edge picking failed");
        CheckGL("framebuffer picking and resize");
        Pass("entity picking, viewport edges and framebuffer resize");

        framebuffer->Unbind();
        framebuffer.reset();
        textures.clear();
        Renderer::Shutdown();
        graphics.RendererReady = false;
        for (uint32_t id : textureIDs)
            Require(glIsTexture(id) == GL_FALSE, "Renderer retained a texture after shutdown");
        CheckGL("renderer resource shutdown");
        Renderer::Init();
        graphics.RendererReady = true;
        Renderer2D::ResetStats();
        Renderer2D::BeginScene(camera);
        Renderer2D::EndScene();
        Require(Renderer2D::GetStats().DrawCalls == 0, "Renderer reinitialization retained stale batch data");
        CheckGL("renderer reinitialization");
        Pass("renderer releases textures before context destruction and initializes again");
    }
}

int main(int argc, char** argv)
{
    try
    {
        Require(argc == 3, "Usage: SmokeTests.exe <artifact directory> <example scene directory>");
        const fs::path output = argv[1];
        fs::create_directories(output);
        Log::Init();
        TestScenes(output, argv[2]);
        TestLayerCleanup();
        TestGraphics();
        std::cout << "All smoke tests passed.\n";
        return 0;
    }
    catch (const std::exception& error)
    {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
