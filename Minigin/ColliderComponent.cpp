#include "ColliderComponent.h"
#include "SDL.h"
#include "Renderer.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"

std::vector<dae::ColliderComponent*> dae::ColliderComponent::m_colliders;

dae::ColliderComponent::ColliderComponent(dae::GameObject* owner, const unsigned int tag)
	: BaseComponent(owner)
	, m_tag(tag)
	, m_colliderRect{ 0, 0, 0, 0 }
	, m_subject()
	, m_xOffset(0)
	, m_yOffset(0)
{
	m_colliders.emplace_back(this);
}

dae::ColliderComponent::~ColliderComponent()
{
	m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), this), m_colliders.end());
}

void dae::ColliderComponent::FixedUpdate()
{
	m_colliderRect.x = static_cast<int>(GetOwner()->GetTransform()->GetWorldPosition().x) + m_xOffset;
	m_colliderRect.y = static_cast<int>(GetOwner()->GetTransform()->GetWorldPosition().y) + m_yOffset;

	for (auto& collider : m_colliders)
	{
		if (collider != this && IsOverlapping(*collider))
		{
			m_subject.Notify(Event{ make_sdbm_hash("Collision"), collider });
			collider->m_subject.Notify(Event{ make_sdbm_hash("Collision"), this });
		}
	}
}

void dae::ColliderComponent::Render() const
{
#if _DEBUG
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255);

	SDL_RenderDrawRect(sdlRenderer, &m_colliderRect);
#endif
}

void dae::ColliderComponent::OffsetColliderRect(int x, int y)
{
	m_xOffset += x;
	m_yOffset += y;
}

void dae::ColliderComponent::ResizeColliderRect(int width, int height)
{
	m_colliderRect.w = width;
	m_colliderRect.h = height;
}

bool dae::ColliderComponent::IsOverlapping(const ColliderComponent& other) const
{
	return SDL_HasIntersection(&m_colliderRect, &other.m_colliderRect) != 0;
}

void dae::ColliderComponent::AddObserver(Observer* observer)
{
	m_subject.AddObserver(observer);
}

void dae::ColliderComponent::RemoveObserver(Observer* observer)
{
	m_subject.RemoveObserver(observer);
}
