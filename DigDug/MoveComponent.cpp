#include "MoveComponent.h"
#include "GameObject.h"
#include "DaeTime.h"

MoveComponent::MoveComponent(dae::GameObject* owner, float speed)
	: dae::BaseComponent(owner), m_speed(speed), m_direction()
{
}

void MoveComponent::Update()
{
	glm::vec3 oldPosition = GetOwner()->GetTransform()->GetLocalPosition();
	GetOwner()->GetTransform()->SetLocalPosition(oldPosition + m_direction * m_speed * dae::Time::GetInstance().GetDeltaTime());
	m_direction = glm::vec3(0.f, 0.f, 0.f);
}

void MoveComponent::SetMoveDirection(glm::vec3 direction)
{
	m_direction += direction;
}
