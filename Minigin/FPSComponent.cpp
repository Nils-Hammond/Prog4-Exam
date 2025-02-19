#include "FPSComponent.h"
#include "TextRenderComponent.h"
#include "GameObject.h"
#include "Time.h"
#include <iostream>
#include <format>

dae::FPSComponent::FPSComponent(GameObject* owner)
	: BaseComponent(owner), m_textUpdateTimer(0)
{
	m_pTextRenderComponent = GetOwner()->GetComponent<TextRenderComponent>();
	assert(m_pTextRenderComponent != nullptr && "FPSComponent constructed before TextRenderComponent");
}

void dae::FPSComponent::FixedUpdate()
{
}

void dae::FPSComponent::Update()
{
	m_textUpdateTimer += Time::GetInstance().GetDeltaTime();

	if (m_textUpdateTimer >= TEXT_UPDATE_DELAY)
	{
		float fps = 1 / Time::GetInstance().GetDeltaTime();

		std::string fpsText = std::format("{:.1f} FPS", fps);
		m_pTextRenderComponent->SetText(fpsText);
		m_textUpdateTimer -= TEXT_UPDATE_DELAY;
	}
}
