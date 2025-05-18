#pragma once
#include "BaseComponent.h"
#include "SubjectObserver.h"


class HealthComponent final : public dae::BaseComponent, public dae::Subject
{
public:
	HealthComponent(dae::GameObject* owner, int startingHealth);
	~HealthComponent() = default;
	void Update() override {};
	int TakeDamage(int damage);
	int GetHealth() const;
private:
	int m_CurrentHealth{};
	int m_MaxHealth{};
};

namespace dae { class TextRenderComponent; }
class LivesDisplayComponent final : public dae::BaseComponent, public dae::Observer
{
public:
	LivesDisplayComponent(dae::GameObject* owner, int startingLives = 3);
	~LivesDisplayComponent() = default;
	void Update() override {};
	void OnNotify(dae::Event event) override;
private:
	dae::TextRenderComponent* m_pTextRenderComponent{};
	int m_Lives{};
};

class ScoreDisplayComponent final : public dae::BaseComponent, public dae::Observer, public dae::Subject
{
public:
	ScoreDisplayComponent(dae::GameObject* owner);
	~ScoreDisplayComponent() = default;
	void Update() override {};
	void OnNotify(dae::Event event) override;
private:
	dae::TextRenderComponent* m_pTextRenderComponent{};
	int m_Score{};
};
