#include "FPSComponent.h"
#include "TextRenderComponent.h"
#include "GameObject.h"
#include "Time.h"
#include <iostream>
#include <format>

dae::FPSComponent::FPSComponent(std::weak_ptr<GameObject> parentObject)
	: BaseComponent(parentObject), m_frames(0), m_timeSinceLastFixedUpdate(0)
{
}

void dae::FPSComponent::FixedUpdate()
{
	float fps = m_frames / m_timeSinceLastFixedUpdate;
	if (m_timeSinceLastFixedUpdate == 0)
		fps = 0;
	m_timeSinceLastFixedUpdate = 0;
	m_frames = 0;
	std::string fpsText = std::format("{:.1f} FPS", fps);
	if (auto parent = _parentObjectPtr.lock())
	{
		parent->GetComponent<TextRenderComponent>()->SetText(fpsText);
	}
}

void dae::FPSComponent::Update()
{
	++m_frames;
	m_timeSinceLastFixedUpdate += Time::GetInstance().GetDeltaTime();
}
