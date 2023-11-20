#include "EditorLayer.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "Scotch/Core/KeyCodes.h"

#include "Scotch/Scene/SceneSerializer.h"
#include "Scotch/Utils/PlatformUtils.h"
#include "ImGuizmo.h"

#include "Scotch/Math/Math.h"

namespace Scotch {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f, true)
    {
    }

    void EditorLayer::OnAttach()
    {
        SH_PROFILE_FUNCTION();

        // Load Textures
        {
            m_Texture = Texture2D::Create("assets/textures/whitepaper.jpg");
            m_SpriteSheet = Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");
            m_Logo = Texture2D::Create("assets/textures/olx-logo.png");

            m_TextureStairs = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128.0f, 128.0f });
            m_TreeTexture = SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128.0f, 128.0f }, { 1, 2 });
        }

        // Setup Framebuffer
        {
            FrameBufferSpecification fbSpec;
            fbSpec.Attachments = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth};
            fbSpec.Width = 1280;
            fbSpec.Height = 720;
            m_FrameBuffer = FrameBuffer::Create(fbSpec);
        }

        NewScene();

        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

        // Old Scene Details
#if 0
        {
            Entity greenSquare = m_ActiveScene->CreateEntity("Green Square");
            greenSquare.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            Entity RedSquare = m_ActiveScene->CreateEntity("Red Square");
            RedSquare.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

            Entity camera = m_ActiveScene->CreateEntity("Camera A");
            camera.AddComponent<CameraComponent>();

            Entity camera2 = m_ActiveScene->CreateEntity("Camera B");
            camera2.AddComponent<CameraComponent>();
            camera2.GetComponent<CameraComponent>().Primary = false;

            class CameraController : public ScriptableEntity
            {
            public:

                void OnUpdate(TimeStep ts)
                {
                    auto& translation = GetComponent<TransformComponent>().Translation;
                    float speed = 5.0f;

                    if (Input::IsKeyPressed(Key::A))
                        translation.x -= speed * ts;
                    if (Input::IsKeyPressed(Key::D))
                        translation.x += speed * ts;
                    if (Input::IsKeyPressed(Key::W))
                        translation.y += speed * ts;
                    if (Input::IsKeyPressed(Key::S))
                        translation.y -= speed * ts;
                }
            };

            camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
        }
#endif
    }

    void EditorLayer::OnDetach()
    {
        SH_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(TimeStep ts)
    {
        SH_PROFILE_FUNCTION();

        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

        //Resize
        if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
        }

        // Update
        if(m_ViewportFocused)
        {
            m_CameraController.OnUpdate(ts);
            m_EditorCamera.OnUpdate(ts);
        }


        // Render
        Renderer2D::ResetStats();
        m_FrameBuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();

        // Clear entityID attachment to -1
        m_FrameBuffer->ClearAttachment(1, -1);

        // Update scene
        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

        auto [mx, my] =  ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;
        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;

        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX <= (int)viewportSize.x && mouseY <= (int)viewportSize.y)
        {
            int pixeldata = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
            m_HoveredEntity = pixeldata == -1 ? Entity() : Entity{(entt::entity)pixeldata, m_ActiveScene.get()};
        }

        //{
        //    static float rotation = 0.0f;
        //    rotation += ts * 50.f;
        //    Renderer2D::BeginScene(m_CameraController.GetCamera());
        //    Renderer2D::DrawQuad({ 0.5f, 0.5f }, glm::radians(45.0f), { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });
        //    Renderer2D::DrawQuad({ 0.5f, 0.5f }, 0.0f, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });
        //    Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        //    Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.1f }, 0.0f, { 0.8f, 0.8f }, m_Logo);
        //    Renderer2D::DrawQuad({ 1.0f, 0.0f }, -rotation * 0.5f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        //    Renderer2D::DrawQuad({ 0.0f, -0.5f }, rotation, { 0.5f, 0.5f }, m_Texture, 1.0f, { 1.0f, 0.7f, 0.8f, 1.0f });
        //    Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, 0.0f, { 10.0f, 10.0f }, m_Texture, 5.0f);
        //    Renderer2D::EndScene();
        //    Renderer2D::BeginScene(m_CameraController.GetCamera());
        //    for (float y = -5.0f; y < 5.0f; y += 0.5f)
        //    {
        //        for (float x = -5.0f; x < 5.0f; x += 0.5f)
        //        {
        //            glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
        //            Renderer2D::DrawQuad({ x + 0.25f, y + 0.25f }, 0.0f, { 0.45f, 0.45f }, color);
        //        }
        //    }
        //    Renderer2D::EndScene();
        //}

        m_FrameBuffer->Unbind();

    }

    void EditorLayer::OnImGuiRender()
    {
        SH_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSize = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSize;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Scene", "Ctrl+N"))
                    NewScene();

                if (ImGui::MenuItem("Load Scene...", "Ctrl+O"))
                    LoadScene();

                if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
                    SaveScene();

                if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
                    SaveSceneAs();

                ImGui::Separator();

                if (ImGui::MenuItem("Exit")) Application::Get().Close();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        m_SceneHierarchyPanel.OnImGuiRender();

        ImGui::Begin("Renderer2D Stats");

        std::string name = "None";
        if (m_HoveredEntity)
            name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
        ImGui::Text("Hovered Entity: %s", name.c_str());

        auto stats = Renderer2D::GetStats();

        ImGui::Text("Draw calls: %d", stats.DrawCalls);
        ImGui::Text("Quads: %d", stats.QuadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
        ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImguiLayer()->BlockEvents(!m_ViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

        uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x,m_ViewportSize.y), ImVec2{ 0,1 }, ImVec2{ 1,0 });

        // Gizmos

        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity && m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            float windowWidth = (float)ImGui::GetWindowWidth();
            float windowHeight = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, 
                m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            // Camera

            // Runtime Camera from entity
            // auto cameraEntity = m_ActiveScene->GetPrimaryCamera();
            // const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            // const glm::mat4& cameraProjection = camera.GetProjection();
            // glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

            // EditorCamera
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
            glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

            // Snapping
            bool snap = Input::IsKeyPressed(Key::LeftControl);
            float snapValue = 0.5f;
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
                snapValue = 45.0f;

            float snapValues[3] = { snapValue, snapValue, snapValue };
            
            // Entity Transform
            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            bool isGizmoDrawn = ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), 
                (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, rotation, scale;
                Math::DecomposeTransform(transform, translation, rotation, scale);

                tc.Translation = translation;
                tc.Rotation = rotation;
                tc.Scale = scale;

            }
        }



        ImGui::End();
        ImGui::PopStyleVar();


        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        m_CameraController.OnEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(SH_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(SH_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent e)
    {
        //Shortcuts
        if (e.GetRepeatCount() > 0)
            return false;

        bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
        bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

        switch (e.GetKeyCode())
        {
            case Key::S:
            {
                if (control)
                {
                    if (shift) SaveSceneAs();
                    else SaveScene();
                }
                break;
            }
            case Key::O:
            {
                if (control)
                    LoadScene();
                break;
            }
            case Key::N:
            {
                if (control)
                    NewScene();
                break;
            }

            //Gizmos
            case Key::Q:
                m_GizmoType = -1;
                break;
            case Key::W:
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            case Key::E:
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            case Key::R:
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
        }
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent e)
    {
        if (e.GetMouseButton() == Mouse::ButtonLeft)
        {
            if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt)) // conditions for can mouse pick
                m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
        }
        return false;
    }

    void EditorLayer::NewScene()
    {
        m_ActiveScene = CreateRef<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
        m_ActiveSceneFilepath = std::string();

        // Setting new scene template
        Entity square = m_ActiveScene->CreateEntity("Square");
        square.AddComponent<SpriteRendererComponent>();
        Entity camera = m_ActiveScene->CreateEntity("Main Camera");
        camera.AddComponent<CameraComponent>();
    }

    void EditorLayer::LoadScene()
    {
        std::string filepath = FileDialogs::OpenFile("Scotch Scene (*.scotch)\0*.scotch\0");
        if (!filepath.empty())
        {
            m_ActiveScene = CreateRef<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
        m_ActiveSceneFilepath = filepath;
    }

    void EditorLayer::SaveScene()
    {
        if (!m_ActiveSceneFilepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(m_ActiveSceneFilepath);
        }
        else
        {
            SaveSceneAs();
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        m_ActiveSceneFilepath = FileDialogs::SaveFile("Scotch Scene (*.scotch)\0*.scotch\0");
        if (!m_ActiveSceneFilepath.empty()) SaveScene();
    }

}