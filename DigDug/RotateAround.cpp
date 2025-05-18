#include "RotateAround.h"
#include "GameObject.h"
#include "Transform.h"
#include "DaeTime.h"

RotateAround::RotateAround(dae::GameObject* owner, float speed, float radius)
	: dae::BaseComponent(owner)
	, m_Speed(speed)
	, m_Radius(radius)
	, m_Angle(0)
{
}

void RotateAround::Update()
{
	m_Angle += m_Speed * dae::Time::GetInstance().GetDeltaTime();
	float x = m_Radius * cos(m_Angle);
	float y = m_Radius * sin(m_Angle);
	GetOwner()->GetTransform()->SetLocalPosition(x, y);
}
