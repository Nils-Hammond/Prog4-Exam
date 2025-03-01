#include "RotateAround.h"
#include "GameObject.h"
#include "Transform.h"
#include "Time.h"

dae::RotateAround::RotateAround(GameObject* owner, float speed, float radius)
	: BaseComponent(owner)
	, m_Speed(speed)
	, m_Radius(radius)
	, m_Angle(0)
{
}

void dae::RotateAround::Update()
{
	m_Angle += m_Speed * Time::GetInstance().GetDeltaTime();
	float x = m_Radius * cos(m_Angle);
	float y = m_Radius * sin(m_Angle);
	GetOwner()->GetTransform()->SetLocalPosition(x, y);
}
