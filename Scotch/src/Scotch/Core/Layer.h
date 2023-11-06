#pragma once

#include "Scotch/Core/Core.h"
#include "Scotch/Events/Event.h"

#include "Scotch/Core/TimeStep.h"

namespace Scotch {

	class SCOTCH_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_Debugname; }

	protected:
		std::string m_Debugname;
	};
}
