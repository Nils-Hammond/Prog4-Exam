#include "Transform.h"
#include "GameObject.h"

dae::Transform::Transform(GameObject* owner, float x, float y, float z)
	: BaseComponent(owner), m_localPosition{ x, y, z }, m_worldPosition{}, m_positionIsDirty{ true }
{
}

/*
void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
*/

void dae::Transform::SetLocalPosition(float x, float y, float z)
{
	m_localPosition.x = x;
	m_localPosition.y = y;
	m_localPosition.z = z;
	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec3& pos)
{
	m_localPosition = pos;
	SetPositionDirty();
}

const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_positionIsDirty)
		UpdateWorldPosition();
	return m_worldPosition;
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_positionIsDirty)
	{
		if (GetOwner()->GetParent() == nullptr)
			m_worldPosition = m_localPosition;
		else
			m_worldPosition = GetOwner()->GetParent()->GetTransform()->GetWorldPosition() + m_localPosition;
	}
	m_positionIsDirty = false;
}

void dae::Transform::SetPositionDirty()
{
	m_positionIsDirty = true;
	for (auto child : GetOwner()->GetChildren())
	{
		child->GetTransform()->SetPositionDirty();
	}
}
