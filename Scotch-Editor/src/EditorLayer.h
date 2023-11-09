#pragma once

#include <Scotch.h>

namespace Scotch {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(TimeStep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		Scotch::OrthographicCameraController m_CameraController;

		// Temp
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<Texture2D> m_Logo;

		Ref<SubTexture2D> m_TextureStairs, m_TreeTexture;

		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = {0, 0};
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0 };

	};
}