#include "MoveComponent.h"
#include "GameObject.h"
#include "DaeTime.h"
#include "RenderComponent.h"

MoveComponent::MoveComponent(dae::GameObject* owner)
	: dae::BaseComponent(owner), m_direction(), m_oldDirection(),
	m_renderComponent(nullptr), m_isFacingRight(true)
{
	m_renderComponent = GetOwner()->GetComponent<dae::RenderComponent>();
}

void MoveComponent::Update()
{
	if (m_direction == glm::vec3(0.f, 0.f, 0.f))
		return;
	glm::vec2 scale = m_renderComponent->GetScale();
	if (m_direction.x > 0.f)
	{
		m_isFacingRight = true;
		m_renderComponent->SetScale(scale.x > 0 ? scale.x : -scale.x, scale.y);
	}
	else if (m_direction.x < 0.f)
	{
		m_isFacingRight = false;
		m_renderComponent->SetScale(scale.x > 0 ? -scale.x : scale.x, scale.y);
	}
	if (m_direction.y > 0.f)
		m_renderComponent->SetAngle(scale.x > 0 ? 90.f : -90.f);
	else if (m_direction.y < 0.f)
		m_renderComponent->SetAngle(scale.x > 0 ? -90.f : 90.f);
	else
		m_renderComponent->SetAngle(0.f);
	glm::vec3 oldPosition = GetOwner()->GetTransform()->GetLocalPosition();
	GetOwner()->GetTransform()->SetLocalPosition(oldPosition + m_direction * dae::Time::GetInstance().GetDeltaTime());
	m_direction = glm::vec3(0.f, 0.f, 0.f);
}

void MoveComponent::SetMoveDirection(glm::vec3 direction)
{
	//std::cout << "Direction: " << direction.x << ", " << direction.y << ", " << direction.z << std::endl;
	m_direction = direction;
	m_oldDirection = direction;
	m_oldDirection /= glm::length(m_oldDirection);
}
