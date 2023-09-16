#pragma once

#include "Core.h"
#include "Window.h"
#include "Scotch/LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Scotch/ImGui/ImGuiLayer.h"

#include "Scotch/Renderer/Shader.h"
#include "Scotch/Renderer/Buffer.h"
#include "Scotch/Renderer/VertexArray.h"

#include "Scotch/Renderer/Camera.h"

namespace Scotch {
	class SCOTCH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}