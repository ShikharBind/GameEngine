#include <Scotch.h>
#include <Scotch/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Scotch::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true), m_SquarePosition(0.0)
	{
		m_VertexArray = Scotch::VertexArray::Create();
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};
		Scotch::Ref<Scotch::VertexBuffer> m_VertexBuffer(Scotch::VertexBuffer::Create(vertices, sizeof(vertices)));

		Scotch::BufferLayout layout = {
			{Scotch::ShaderDataType::Float3, "a_Position"},
			{Scotch::ShaderDataType::Float4, "a_Color"}
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };

		Scotch::Ref<Scotch::IndexBuffer> m_IndexBuffer(Scotch::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}	
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}	
		)";

		m_Shader = Scotch::Shader::Create("triangle Shader", vertexSrc, fragmentSrc);

		// Rendering Square //////////////////

		m_SquareVA = Scotch::VertexArray::Create();

		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		};
		Scotch::Ref<Scotch::VertexBuffer> m_SquareVB(Scotch::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		m_SquareVB->SetLayout({
			{Scotch::ShaderDataType::Float3, "a_Position"},
			{Scotch::ShaderDataType::Float2, "a_TexCoord"}
			});
		m_SquareVA->AddVertexBuffer(m_SquareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 1, 3, 2 };

		Scotch::Ref<Scotch::IndexBuffer> m_SquareIB(Scotch::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(m_SquareIB);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}	
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color.x, u_Color.y, u_Color.z, 1.0);
			}	
		)";

		m_FlatColorShader = Scotch::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_Texture = Scotch::Texture2D::Create("assets/textures/whitepaper.jpg");

		std::dynamic_pointer_cast<Scotch::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Scotch::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Scotch::TimeStep ts) override
	{
		// Update
		m_CameraController.OnUpdate(ts);

		if (Scotch::Input::IsKeyPressed(SH_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		else if (Scotch::Input::IsKeyPressed(SH_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * ts;

		if (Scotch::Input::IsKeyPressed(SH_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;
		else if (Scotch::Input::IsKeyPressed(SH_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * ts;

		// Render
		Scotch::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Scotch::RenderCommand::Clear();

		Scotch::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Scotch::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Scotch::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f - 0.95f, y * 0.11f - 0.95f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition + pos) * scale;
				Scotch::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Scotch::Renderer::Submit(m_ShaderLibrary.Get("Texture"), m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		

		// Triangle
		// Scotch::Renderer::Submit(m_Shader, m_VertexArray);

		Scotch::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Scotch::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Scotch::ShaderLibrary m_ShaderLibrary;
	Scotch::Ref<Scotch::Shader> m_Shader;
	Scotch::Ref<Scotch::VertexArray> m_VertexArray;

	Scotch::Ref<Scotch::Shader> m_FlatColorShader;
	Scotch::Ref<Scotch::VertexArray> m_SquareVA;

	Scotch::Ref<Scotch::Texture2D> m_Texture;

	Scotch::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 1.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Scotch::Application
{
public:
	Sandbox()
	{
		// PushLayer(new ExampleLayer());
		PushOverlay(new Sandbox2D());
	}

	~Sandbox() 
	{

	}
};

Scotch::Application* Scotch::CreateApplication()
{
	return new Sandbox();
}