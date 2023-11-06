#pragma once

#include "RenderCommand.h"

#include "Camera.h"
#include "Texture.h"

namespace Scotch
{
	
	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();
		

		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		// Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture);
	};

}