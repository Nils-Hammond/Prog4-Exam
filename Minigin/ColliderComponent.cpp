#include "ColliderComponent.h"
#include "SDL.h"
#include "Renderer.h"
#include "GameObject.h"
#include "SpriteRenderComponent.h"
#include "ServiceLocator.h"
#include <iostream>

dae::ColliderComponent::ColliderComponent(dae::GameObject* owner, const unsigned int tag)
	: BaseComponent(owner)
	, m_tag(tag)
	, m_colliderRect{ 0, 0, 0, 0 }
	, m_subject()
	, m_xOffset(0)
	, m_yOffset(0)
{
	ServiceLocator::GetCollisionSystem().RegisterCollider(this);
}

dae::ColliderComponent::~ColliderComponent()
{
	ServiceLocator::GetCollisionSystem().UnregisterCollider(this);
}

void dae::ColliderComponent::FixedUpdate()
{
	m_colliderRect.x = static_cast<int>(GetOwner()->GetTransform()->GetWorldPosition().x) + m_xOffset;
	m_colliderRect.y = static_cast<int>(GetOwner()->GetTransform()->GetWorldPosition().y) + m_yOffset;
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
	m_xOffset = x;
	m_yOffset = y;
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

void dae::ColliderComponent::OnCollision(const ColliderComponent& other)
{
	m_subject.Notify(Event{ make_sdbm_hash("OnCollision"), std::any{&other} });
}

void dae::ColliderComponent::AddObserver(Observer* observer)
{
	m_subject.AddObserver(observer);
}

void dae::ColliderComponent::RemoveObserver(Observer* observer)
{
	m_subject.RemoveObserver(observer);
}
