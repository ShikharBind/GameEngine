#include "shpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Scotch {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}