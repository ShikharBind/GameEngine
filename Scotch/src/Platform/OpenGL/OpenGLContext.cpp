#include "shpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include <stdexcept>

namespace Scotch { 
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		if (!windowHandle)
			throw std::invalid_argument("OpenGL context requires a valid GLFW window.");
	}
	void OpenGLContext::Init()
	{
		SH_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		if (glfwGetCurrentContext() != m_windowHandle)
			throw std::runtime_error("Could not make the OpenGL context current.");

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!status)
			throw std::runtime_error("GLAD could not load OpenGL functions from the graphics driver.");
		if (!GLAD_GL_VERSION_4_5)
			throw std::runtime_error("Scotch requires OpenGL 4.5 or newer.");

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
