#pragma once

#include <vector>
#include "SDL_rect.h"
namespace dae
{
	class ColliderComponent;
	class CollisionSystem
	{
	public:
		virtual ~CollisionSystem() {};
		virtual void RegisterCollider(ColliderComponent* collider) = 0;
		virtual void UnregisterCollider(ColliderComponent* collider) = 0;
		virtual void FixedUpdate() = 0;
		virtual std::vector<ColliderComponent*> GetCollisions(const ColliderComponent& collider) const = 0;
		virtual std::vector<ColliderComponent*> GetCollisions(const SDL_Rect& colliderRect) const = 0;
	};

	class SimpleCollisionSystem : public CollisionSystem
	{
	public:
		void RegisterCollider(ColliderComponent* collider) override;
		void UnregisterCollider(ColliderComponent* collider) override;
		void FixedUpdate() override;
		std::vector<ColliderComponent*> GetCollisions(const ColliderComponent& collider) const override;
		std::vector<ColliderComponent*> GetCollisions(const SDL_Rect& colliderRect) const override;
	private:
		std::vector<ColliderComponent*> m_pColliders;
	};

	class NullCollisionSysten : public CollisionSystem
	{
	public:
		void RegisterCollider(ColliderComponent* ) override {}
		void UnregisterCollider(ColliderComponent* ) override {}
		void FixedUpdate() override {}
		std::vector<ColliderComponent*> GetCollisions(const ColliderComponent& ) const override { return {}; }
		std::vector<ColliderComponent*> GetCollisions(const SDL_Rect& ) const { return{}; }
	};
}