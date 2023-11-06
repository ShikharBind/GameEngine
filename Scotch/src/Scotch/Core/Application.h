#pragma once

#include "Core.h"
#include "Window.h"
#include "Scotch/Core/LayerStack.h"
#include "Scotch/Events/Event.h"
#include "Scotch/Events/ApplicationEvent.h"

#include "Scotch/ImGui/ImGuiLayer.h"

#include "Scotch/Core/TimeStep.h"

namespace Scotch {
	class Application
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
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	// To be defined in client
	Application* CreateApplication();

}