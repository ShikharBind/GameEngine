#include "shpch.h"
#include "OpenGLBuffer.h"

#include "glad/glad.h"

namespace Scotch {

	// VertexBuffer ///////////////////////////////////////////////////////////////
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		SH_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		SH_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RenderID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		SH_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		SH_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



	// IndexBuffer ///////////////////////////////////////////////////////////////
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		SH_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RenderID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		SH_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RenderID);
	}
	void OpenGLIndexBuffer::Bind() const
	{
		SH_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		SH_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}