#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	// Rendering Square //////////////////

	m_SquareVA = Scotch::VertexArray::Create();

	float squareVertices[4 * 5] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};
	Scotch::Ref<Scotch::VertexBuffer> m_SquareVB(Scotch::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

	m_SquareVB->SetLayout({
		{Scotch::ShaderDataType::Float3, "a_Position"}
	});
	m_SquareVA->AddVertexBuffer(m_SquareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	Scotch::Ref<Scotch::IndexBuffer> m_SquareIB(Scotch::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(m_SquareIB);

	m_Shader = Scotch::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Scotch::TimeStep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Scotch::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Scotch::RenderCommand::Clear();

	Scotch::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Scotch::OpenGLShader>(m_Shader)->Bind();
	std::dynamic_pointer_cast<Scotch::OpenGLShader>(m_Shader)->UploadUniformFloat4("u_Color", m_SquareColor);
	
	Scotch::Renderer::Submit(m_Shader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Scotch::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Scotch::Event& e)
{
	m_CameraController.OnEvent(e);
}
