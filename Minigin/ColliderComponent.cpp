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
	, m_pSubject(std::make_unique<Subject>())
	, m_xOffset(0)
	, m_yOffset(0)
	, m_isActive(false)
{
	ServiceLocator::GetCollisionSystem().RegisterCollider(this);
	m_colliderRect.x = static_cast<int>(GetOwner()->GetTransform()->GetWorldPosition().x) + m_xOffset;
	m_colliderRect.y = static_cast<int>(GetOwner()->GetTransform()->GetWorldPosition().y) + m_yOffset;
}

dae::ColliderComponent::~ColliderComponent()
{
	ServiceLocator::GetCollisionSystem().UnregisterCollider(this);
}

void dae::ColliderComponent::Update()
{
	m_colliderRect.x = static_cast<int>(GetOwner()->GetTransform()->GetWorldPosition().x) + m_xOffset;
	m_colliderRect.y = static_cast<int>(GetOwner()->GetTransform()->GetWorldPosition().y) + m_yOffset;
}

void dae::ColliderComponent::Render() const
{
#if _DEBUG
	if (!m_isActive)
		return;
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 255, 255);

	SDL_RenderDrawRect(sdlRenderer, &m_colliderRect);
#endif
}

const SDL_Rect& dae::ColliderComponent::GetColliderRect() const
{
	return m_colliderRect;
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
	SetActive(true);
}

bool dae::ColliderComponent::IsOverlapping(const ColliderComponent& other) const
{
	return SDL_HasIntersection(&m_colliderRect, &other.m_colliderRect) != 0;
}

void dae::ColliderComponent::OnCollision(const ColliderComponent& other)
{
	m_pSubject->Notify(Event{ make_sdbm_hash("OnCollision"), std::any{&other} });
}

void dae::ColliderComponent::AddObserver(Observer* observer)
{
	m_pSubject->AddObserver(observer);
}

void dae::ColliderComponent::RemoveObserver(Observer* observer)
{
	m_pSubject->RemoveObserver(observer);
}
