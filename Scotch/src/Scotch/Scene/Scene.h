#pragma once

#include "entt.hpp"

#include "Scotch/Core/TimeStep.h"

namespace Scotch {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(TimeStep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 1, m_ViewportHeight = 1;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}