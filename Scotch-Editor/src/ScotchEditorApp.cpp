#include <Scotch.h>
#include <Scotch/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "EditorLayer.h"

namespace Scotch {

	class ScotchEditor : public Application
	{
	public:
		ScotchEditor() : Application("Scotch Editor")
		{
			// PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~ScotchEditor()
		{

		}
	};

	Application* CreateApplication()
	{
		return new ScotchEditor();
	}
}