#include "FPSComponent.h"
#include "GameObject.h"
#include "TextRenderComponent.h"
#include "DaeTime.h"
#include <iostream>
#include <format>

FPSComponent::FPSComponent(dae::GameObject* owner) : dae::BaseComponent(owner), m_textUpdateTimer(0)
{
	m_pTextRenderComponent = GetOwner()->GetComponent<dae::TextRenderComponent>();
	assert(m_pTextRenderComponent != nullptr && "FPSComponent constructed before TextRenderComponent");
}

void FPSComponent::Update()
{
	// I don't think this check works if something destroys the TextRenderComponent after the FPSComponent has updated and am not sure how to fix that
	if (m_pTextRenderComponent == nullptr)
		return;
	if (m_pTextRenderComponent->IsToBeDestroyed())
	{
		m_pTextRenderComponent = nullptr;
		return;
	}

	m_textUpdateTimer += dae::Time::GetInstance().GetDeltaTime();

	if (m_textUpdateTimer >= TEXT_UPDATE_DELAY)
	{
		float fps = 1 / dae::Time::GetInstance().GetDeltaTime();

		std::string fpsText = std::format("{:.1f} FPS", fps);
		m_pTextRenderComponent->SetText(fpsText);
		m_textUpdateTimer -= TEXT_UPDATE_DELAY;
	}
}
