#pragma once

#include <Scotch.h>

class Sandbox2D : public Scotch::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Scotch::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Scotch::Event& e) override;

private:
	Scotch::OrthographicCameraController m_CameraController;
	//Scotch::ShaderLibrary m_ShaderLibrary;

	// Temp
	Scotch::Ref<Scotch::Shader> m_Shader;
	Scotch::Ref<Scotch::VertexArray> m_SquareVA;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0 };

};