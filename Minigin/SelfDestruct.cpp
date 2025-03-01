#include "SelfDestruct.h"
#include "GameObject.h"
#include "Time.h"
#include "TextRenderComponent.h"

dae::SelfDestruct::SelfDestruct(GameObject* owner, float timeToLive)
	: BaseComponent(owner)
	, m_TimeToLive(timeToLive)
	, m_CurrentTime(0)
{
}

void dae::SelfDestruct::Update()
{
	m_CurrentTime += Time::GetInstance().GetDeltaTime();
	if (m_CurrentTime >= m_TimeToLive)
	{
		GetOwner()->Destroy();
	}
}
