#pragma once
#include "BaseComponent.h"
#include "SubjectObserver.h"

namespace dae
{
	class TextRenderComponent;
	class RenderComponent;
}

class HealthComponent final : public dae::BaseComponent
{
public:
	HealthComponent(dae::GameObject* owner, int startingHealth);
	~HealthComponent() = default;
	void Update() override {};
	int TakeDamage(int damage);
	int GetHealth() const;
	void RegisterObserver(dae::Observer* observer);
	void UnregisterObserver(dae::Observer* observer);
private:
	int m_CurrentHealth{};
	int m_MaxHealth{};
	std::unique_ptr<dae::Subject> m_pSubject{};
};

class LivesDisplayComponent final : public dae::BaseComponent, public dae::Observer
{
public:
	LivesDisplayComponent(dae::GameObject* owner, int startingLives = 3);
	~LivesDisplayComponent() = default;
	void Update() override {};
	void OnNotify(dae::Event event) override;
private:
	std::vector<dae::RenderComponent*> m_pLifeIcons;
	//int m_Lives{};
};

class ScoreDisplayComponent final : public dae::BaseComponent, public dae::Observer
{
public:
	ScoreDisplayComponent(dae::GameObject* owner);
	~ScoreDisplayComponent() = default;
	void Update() override {};
	void OnNotify(dae::Event event) override;
private:
	dae::TextRenderComponent* m_pTextRenderComponent{};
	int m_score{};
};
