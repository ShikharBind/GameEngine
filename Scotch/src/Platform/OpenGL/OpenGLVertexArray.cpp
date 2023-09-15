#include "shpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Scotch {

	static GLenum ShaderDataTypeToOpenGLBasetype(ShaderDataType type)
	{
		switch (type)
		{
		case Scotch::ShaderDataType::Float:		return GL_FLOAT;
			break;
		case Scotch::ShaderDataType::Float2:	return GL_FLOAT;
			break;
		case Scotch::ShaderDataType::Float3:	return GL_FLOAT;
			break;
		case Scotch::ShaderDataType::Float4:	return GL_FLOAT;
			break;
		case Scotch::ShaderDataType::Mat3:		return GL_FLOAT;
			break;
		case Scotch::ShaderDataType::Mat4:		return GL_FLOAT;
			break;
		case Scotch::ShaderDataType::Int:		return GL_INT;
			break;
		case Scotch::ShaderDataType::Int2:		return GL_INT;
			break;
		case Scotch::ShaderDataType::Int3:		return GL_INT;
			break;
		case Scotch::ShaderDataType::Int4:		return GL_INT;
			break;
		case Scotch::ShaderDataType::Bool:		return GL_BOOL;
			break;
		default:
			break;
		}

		SH_CORE_ASSERT(false, "Unknown Shader Data Type!");
		return 0;
	}


	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		SH_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");


		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBasetype(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}