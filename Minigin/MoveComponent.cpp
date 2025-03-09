#include "MoveComponent.h"
#include "GameObject.h"
#include "Time.h"

dae::MoveComponent::MoveComponent(GameObject* owner, float speed)
	: BaseComponent(owner), m_speed(speed), m_direction()
{
}

void dae::MoveComponent::Update()
{
	glm::vec3 oldPosition = GetOwner()->GetTransform()->GetLocalPosition();
	GetOwner()->GetTransform()->SetLocalPosition(oldPosition + m_direction * m_speed * Time::GetInstance().GetDeltaTime());
	m_direction = glm::vec3(0.f, 0.f, 0.f);
}

void dae::MoveComponent::SetMoveDirection(glm::vec3 direction)
{
	m_direction += direction;
}
