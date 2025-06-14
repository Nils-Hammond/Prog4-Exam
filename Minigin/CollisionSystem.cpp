#include "CollisionSystem.h"
#include "ColliderComponent.h"

void dae::SimpleCollisionSystem::RegisterCollider(ColliderComponent* collider)
{
	m_pColliders.emplace_back(collider);
}

void dae::SimpleCollisionSystem::UnregisterCollider(ColliderComponent* collider)
{
	m_pColliders.erase(std::remove(m_pColliders.begin(), m_pColliders.end(), collider), m_pColliders.end());
}

void dae::SimpleCollisionSystem::FixedUpdate()
{
	for (size_t leftIndex{}; leftIndex < m_pColliders.size(); ++leftIndex)
	{
		for (size_t rightIndex = leftIndex + 1; rightIndex < m_pColliders.size(); ++rightIndex)
		{
			if (m_pColliders[rightIndex]->GetTag() == m_pColliders[leftIndex]->GetTag() 
				|| !m_pColliders[rightIndex]->IsActive() || !m_pColliders[leftIndex]->IsActive())
			{
				// Skip check
			}
			else if (m_pColliders[leftIndex]->IsOverlapping(*m_pColliders[rightIndex]))
			{
				m_pColliders[leftIndex]->OnCollision(*m_pColliders[rightIndex]);
				m_pColliders[rightIndex]->OnCollision(*m_pColliders[leftIndex]);
			}
		}
	}
}

std::vector<dae::ColliderComponent*> dae::SimpleCollisionSystem::GetCollisions(const ColliderComponent& collider) const
{
	std::vector<ColliderComponent*> overlaps;
	for (const auto& other : m_pColliders)
	{
		if (other != &collider && other->IsActive() && collider.IsOverlapping(*other))
			overlaps.emplace_back(other);
	}
	return overlaps;
}

std::vector<dae::ColliderComponent*> dae::SimpleCollisionSystem::GetCollisions(const SDL_Rect& colliderRect) const
{
	std::vector<ColliderComponent*> overlaps;
	for (const auto& other : m_pColliders)
	{
		if (SDL_HasIntersection(&colliderRect, &other->GetColliderRect()))
			overlaps.emplace_back(other);
	}
	return overlaps;
}
