#pragma once

namespace Scotch {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Scotch::ShaderDataType::None:		return 0;
			break;
		case Scotch::ShaderDataType::Float:		return 4;
			break;
		case Scotch::ShaderDataType::Float2:	return 4 * 2;
			break;
		case Scotch::ShaderDataType::Float3:	return 4 * 3;
			break;
		case Scotch::ShaderDataType::Float4:	return 4 * 4;
			break;
		case Scotch::ShaderDataType::Mat3:		return 4 * 3 * 3;
			break;
		case Scotch::ShaderDataType::Mat4:		return 4 * 4 * 4;
			break;
		case Scotch::ShaderDataType::Int:		return 4;
			break;
		case Scotch::ShaderDataType::Int2:		return 4 * 2;
			break;
		case Scotch::ShaderDataType::Int3:		return 4 * 3;
			break;
		case Scotch::ShaderDataType::Int4:		return 4 * 4;
			break;
		case Scotch::ShaderDataType::Bool:		return 1;
			break;
		default:
			break;
		}

		SH_CORE_ASSERT(false, "Unknown Shader Data Type!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case Scotch::ShaderDataType::None:		return 0;
				break;
			case Scotch::ShaderDataType::Float:		return 1;
				break;
			case Scotch::ShaderDataType::Float2:	return 2;
				break;
			case Scotch::ShaderDataType::Float3:	return 3;
				break;
			case Scotch::ShaderDataType::Float4:	return 4;
				break;
			case Scotch::ShaderDataType::Mat3:		return 3 * 3;
				break;
			case Scotch::ShaderDataType::Mat4:		return 4 * 4;
				break;
			case Scotch::ShaderDataType::Int:		return 1;
				break;
			case Scotch::ShaderDataType::Int2:		return 2;
				break;
			case Scotch::ShaderDataType::Int3:		return 3;
				break;
			case Scotch::ShaderDataType::Int4:		return 4;
				break;
			case Scotch::ShaderDataType::Bool:		return 1;
				break;
			default:
				break;
			}

			SH_CORE_ASSERT(false, "Unknown Shader Data Type!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements)
		: m_Elements(elements) 
		{
			CalculateOffsetsandStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetsandStride()
		{
			uint32_t offset = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
			}

			m_Stride = offset;
		}


		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}