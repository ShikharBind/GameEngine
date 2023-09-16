#include <Scotch.h>

class ExampleLayer : public Scotch::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Scotch::VertexArray::Create());
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
		};
		std::shared_ptr<Scotch::VertexBuffer> m_VertexBuffer(Scotch::VertexBuffer::Create(vertices, sizeof(vertices)));

		Scotch::BufferLayout layout = {
			{Scotch::ShaderDataType::Float3, "a_Position"},
			{Scotch::ShaderDataType::Float4, "a_Color"}
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };

		std::shared_ptr<Scotch::IndexBuffer> m_IndexBuffer(Scotch::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			
			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(Scotch::Shader::Create(vertexSrc, fragmentSrc));

		// Rendering Square //////////////////

		m_SquareVA.reset(Scotch::VertexArray::Create());

		float squareVertices[4 * 3] = {
			-0.6f, -0.6f, 0.0f,
			 0.6f, -0.6f, 0.0f,
			-0.6f,  0.6f, 0.0f,
			 0.6f,  0.6f, 0.0f
		};
		std::shared_ptr<Scotch::VertexBuffer> m_SquareVB(Scotch::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		m_SquareVB->SetLayout({
			{Scotch::ShaderDataType::Float3, "a_Position"}
			});
		m_SquareVA->AddVertexBuffer(m_SquareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 1, 3, 2 };

		std::shared_ptr<Scotch::IndexBuffer> m_SquareIB(Scotch::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(m_SquareIB);

		std::string blueShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			
			out vec3 v_Position;

			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}	
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}	
		)";

		m_BlueShader.reset(Scotch::Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void UpdateCameraTransform()
	{
		if (Scotch::Input::IsKeyPressed(SH_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed;
		else if (Scotch::Input::IsKeyPressed(SH_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed;

		if (Scotch::Input::IsKeyPressed(SH_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed;
		else if (Scotch::Input::IsKeyPressed(SH_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed;

		if (Scotch::Input::IsKeyPressed(SH_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed;
		else if (Scotch::Input::IsKeyPressed(SH_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed;
	}

	void OnUpdate() override
	{
		UpdateCameraTransform();

		Scotch::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Scotch::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Scotch::Renderer::BeginScene(m_Camera);

		Scotch::Renderer::Submit(m_BlueShader, m_SquareVA);
		Scotch::Renderer::Submit(m_Shader, m_VertexArray);

		Scotch::Renderer::EndScene();
	}

	void OnEvent(Scotch::Event& event) override
	{

	}

private:
	std::shared_ptr<Scotch::Shader> m_Shader;
	std::shared_ptr<Scotch::VertexArray> m_VertexArray;

	std::shared_ptr<Scotch::Shader> m_BlueShader;
	std::shared_ptr<Scotch::VertexArray> m_SquareVA;

	Scotch::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 0.1f;
};

class Sandbox : public Scotch::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		// PushOverlay(new Scotch::ImGuiLayer());
	}

	~Sandbox() 
	{

	}
};

Scotch::Application* Scotch::CreateApplication()
{
	return new Sandbox();
}