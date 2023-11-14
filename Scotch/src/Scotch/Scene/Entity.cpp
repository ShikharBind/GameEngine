#include "shpch.h"
#include "Entity.h"

namespace Scotch
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) {}
}