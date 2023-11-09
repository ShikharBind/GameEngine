#include "Sandbox2D.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	SH_PROFILE_FUNCTION();

	m_Texture = Scotch::Texture2D::Create("assets/textures/whitepaper.jpg");
    m_SpriteSheet = Scotch::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");
    m_Logo = Scotch::Texture2D::Create("assets/textures/olx-logo.png");

    m_TextureStairs = Scotch::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128.0f, 128.0f });
    m_TreeTexture = Scotch::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128.0f, 128.0f }, {1, 2});

    m_CameraController.SetZoomLevel(2.0f);
}

void Sandbox2D::OnDetach()
{
	SH_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Scotch::TimeStep ts)
{
    SH_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(ts);


    // Render
    // Reset stats
    Scotch::Renderer2D::ResetStats();

    {
        SH_PROFILE_SCOPE("Renderer Prep");
        Scotch::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Scotch::RenderCommand::Clear();
    }

#if 0
    {
        SH_PROFILE_SCOPE("Renderer Draw");
        Scotch::Renderer2D::BeginScene(m_CameraController.GetCamera());

        static float rotation = 0.0f;
        rotation += ts * 50.f;

        // Scotch::Renderer2D::DrawQuad({ 0.5f, 0.5f }, glm::radians(45.0f), { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });
        Scotch::Renderer2D::DrawQuad({ 0.5f, 0.5f }, 0.0f, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });
        Scotch::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Scotch::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.1f }, 0.0f, { 0.8f, 0.8f }, m_Logo);
        Scotch::Renderer2D::DrawQuad({ 1.0f, 0.0f }, -rotation * 0.5f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Scotch::Renderer2D::DrawQuad({ 0.0f, -0.5f }, rotation, { 0.5f, 0.5f }, m_Texture, 1.0f, { 1.0f, 0.7f, 0.8f, 1.0f });
        Scotch::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, 0.0f, { 10.0f, 10.0f }, m_Texture, 5.0f);

        Scotch::Renderer2D::EndScene();


        Scotch::Renderer2D::BeginScene(m_CameraController.GetCamera());

        for (float y = -5.0f; y < 5.0f; y += 0.5f)
        {
            for (float x = -5.0f; x < 5.0f; x += 0.5f)
            {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
                Scotch::Renderer2D::DrawQuad({ x + 0.25f, y + 0.25f }, 0.0f, { 0.45f, 0.45f }, color);
            }
        }

        Scotch::Renderer2D::EndScene();
    }
#endif

    Scotch::Renderer2D::BeginScene(m_CameraController.GetCamera());
    Scotch::Renderer2D::DrawQuad({ 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, m_TextureStairs);
    Scotch::Renderer2D::DrawQuad({ 1.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, m_TextureStairs);
    Scotch::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 1.0f, 2.0f }, m_TreeTexture);
    Scotch::Renderer2D::EndScene();
	
}

void Sandbox2D::OnImGuiRender()
{
	SH_PROFILE_FUNCTION();

    ImGui::Begin("Settings");

    auto stats = Scotch::Renderer2D::GetStats();

    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

    ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Scotch::Event& e)
{
	m_CameraController.OnEvent(e);
}
