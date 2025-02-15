#include "Transform.h"

dae::Transform::Transform(std::weak_ptr<GameObject> parentObject, float x, float y, float z)
	: BaseComponent(parentObject), m_position{ x, y, z }
{
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
