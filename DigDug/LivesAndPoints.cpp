#include "LivesAndPoints.h"
#include "TextRenderComponent.h"
#include "GameObject.h"


HealthComponent::HealthComponent(dae::GameObject* owner, int startingHealth)
	: dae::BaseComponent(owner), m_CurrentHealth(startingHealth), m_MaxHealth(startingHealth)
{
}

int HealthComponent::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;
	if (m_CurrentHealth <= 0)
	{
		Notify(dae::Event{ dae::make_sdbm_hash("PlayerDied") });
		m_CurrentHealth = m_MaxHealth;
	}
	return m_CurrentHealth;
}

int HealthComponent::GetHealth() const
{
	return m_CurrentHealth;
}

LivesDisplayComponent::LivesDisplayComponent(dae::GameObject* owner, int startingLives)
	: dae::BaseComponent(owner), m_Lives(startingLives)
{
	m_pTextRenderComponent = GetOwner()->GetComponent<dae::TextRenderComponent>();
	assert(m_pTextRenderComponent != nullptr && "LivesDisplayComponent constructed before TextRenderComponent");
	m_pTextRenderComponent->SetText("# Lives: " + std::to_string(m_Lives));
}

void LivesDisplayComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("PlayerDied"))
	{
		--m_Lives;
		m_pTextRenderComponent->SetText("# Lives: " + std::to_string(m_Lives));
	}
}

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* owner)
	: dae::BaseComponent(owner)
{
	m_pTextRenderComponent = GetOwner()->GetComponent<dae::TextRenderComponent>();
	assert(m_pTextRenderComponent != nullptr && "ScoreDisplayComponent constructed before TextRenderComponent");
	m_pTextRenderComponent->SetText("Score: 0");
}

void ScoreDisplayComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("PookaDied"))
	{
		m_Score += 200;
		m_pTextRenderComponent->SetText("Score: " + std::to_string(m_Score));
	}
	else if (event.id == dae::make_sdbm_hash("FygarDied"))
	{
		m_Score += 400;
		m_pTextRenderComponent->SetText("Score: " + std::to_string(m_Score));
	}
	// Should this just be a ScoreChanged event passing the score someway and letting observers decide if it wins or not?
	if (m_Score >= 500)
	{
		Notify(dae::Event{ dae::make_sdbm_hash("PlayerWon") });
	}
}
