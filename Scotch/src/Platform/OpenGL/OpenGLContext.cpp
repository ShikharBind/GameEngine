#include "shpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Scotch { 
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		SH_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		SH_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SH_CORE_ASSERT(status, "Failed to initialize Glad!");

		SH_CORE_INFO("OpenGL Info: \n\t\t Vendor: {0}\n\t\t Renderer: {1}\n\t\t Version: {2}", 
			(const char*)glGetString(GL_VENDOR),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VERSION)
		);
	}
	void OpenGLContext::SwapBuffers()
	{
		SH_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}
}