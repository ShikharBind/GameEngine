#pragma once

#include "Scotch/Core/Layer.h"

#include "Scotch/Events/ApplicationEvent.h"
#include "Scotch/Events/MouseEvent.h"
#include "Scotch/Events/KeyEvent.h"

namespace Scotch {

	class SCOTCH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void  End();

		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = true;
	};

}