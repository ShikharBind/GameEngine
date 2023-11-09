#pragma once

#include "RenderCommand.h"

#include "Camera.h"
#include "Texture.h"
#include "SubTexture2D.h"

namespace Scotch
{
	
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();
		

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		// Primitives
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const Ref<Texture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const Ref<SubTexture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec2& scale, const Ref<SubTexture2D> texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void FlushAndReset();
		static void AddQuadToVB(const glm::vec3& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const glm::vec2* texCoords, float textureIndex, float tilingFactor);
	};

}