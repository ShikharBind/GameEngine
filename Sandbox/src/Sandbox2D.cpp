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
	{
		SH_PROFILE_SCOPE("Renderer Prep");
		Scotch::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Scotch::RenderCommand::Clear();
	}

	{
		SH_PROFILE_SCOPE("Renderer Draw");
		Scotch::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Scotch::Renderer2D::DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });
		Scotch::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Scotch::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 2.5f, 2.5f }, glm::radians(45.0f), m_Texture);

		Scotch::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	SH_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Scotch::Event& e)
{
	m_CameraController.OnEvent(e);
}
