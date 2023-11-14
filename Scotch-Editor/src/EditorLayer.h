#pragma once

#include <Scotch.h>
#include "Panels/SceneHierarchyPanel.h"

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
		bool OnKeyPressedEvent(KeyPressedEvent e);

		void NewScene();
		void LoadScene();
		void SaveScene();
		void SaveSceneAs();

	private:
		Scotch::OrthographicCameraController m_CameraController;
		Ref<FrameBuffer> m_FrameBuffer;
		Ref<Scene> m_ActiveScene;
		std::string m_ActiveSceneFilepath = std::string();

		// View Port
		glm::vec2 m_ViewportSize = {0, 0};
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		//Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;

		// Temp
		Ref<Texture2D> m_Texture, m_SpriteSheet, m_Logo;
		Ref<SubTexture2D> m_TextureStairs, m_TreeTexture;
	};
}