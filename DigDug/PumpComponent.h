#pragma once
#include "RenderComponent.h"
#include "SubjectObserver.h"

namespace dae {
	class ColliderComponent;
}

class PlayerComponent;
class PumpComponent : public dae::RenderComponent, public dae::Observer
{
public:
	PumpComponent(dae::GameObject* owner, const std::string& filename, float scale = 1.f);
	~PumpComponent();
	PumpComponent(const PumpComponent& other) = delete;
	PumpComponent(PumpComponent&& other) = delete;
	PumpComponent& operator=(const PumpComponent& other) = delete;
	PumpComponent& operator=(PumpComponent&& other) = delete;

	void Update() override;
	void Render() const override;
	void Activate();
	bool IsActive() const;
	void OnNotify(dae::Event event) override;
	dae::ColliderComponent* GetHitEnemy() const;
private:
	void UpdateCollider();
	bool m_isActive;
	bool m_isPumping;
	float m_elapsedTime;
	float m_maxDuration;
	glm::vec2 m_direction;
	dae::ColliderComponent* m_pColliderComponent;
	dae::ColliderComponent* m_pEnemyCollider;
	PlayerComponent* m_pPlayerComponent;
};

