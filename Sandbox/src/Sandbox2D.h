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

	// Temp
	Scotch::Ref<Scotch::Texture2D> m_Texture;
	Scotch::Ref<Scotch::Texture2D> m_SpriteSheet;
	Scotch::Ref<Scotch::Texture2D> m_Logo;

	Scotch::Ref<Scotch::SubTexture2D> m_TextureStairs, m_TreeTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0 };

};