#pragma once

#include "Scotch/Renderer/Shader.h"

namespace Scotch {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		uint64_t m_RenderID;
	};
}