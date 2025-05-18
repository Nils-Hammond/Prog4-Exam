#include "SelfDestruct.h"
#include "GameObject.h"
#include "DaeTime.h"

SelfDestruct::SelfDestruct(dae::GameObject* owner, float timeToLive)
	: BaseComponent(owner)
	, m_TimeToLive(timeToLive)
	, m_CurrentTime(0)
{
}

void SelfDestruct::Update()
{
	m_CurrentTime += dae::Time::GetInstance().GetDeltaTime();
	if (m_CurrentTime >= m_TimeToLive)
	{
		GetOwner()->Destroy();
	}
}
