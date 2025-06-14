#pragma once
#include "BaseComponent.h"
#include "SubjectObserver.h"
#include "glm.hpp"

namespace dae {
	class SpriteRenderComponent;
}
class MoveComponent;

class EnemyComponent : public dae::BaseComponent, public dae::Observer
{
public:
	EnemyComponent(dae::GameObject* owner, float speed);
	virtual ~EnemyComponent() = default;

	virtual void Update() = 0;
	virtual void Kill() = 0;
	virtual void Inflate() = 0;
	virtual void OnNotify(dae::Event event) override = 0;
	virtual void Reset() = 0;

	virtual bool IsDead() const { return m_isDead; };
	virtual bool IsInflating() const { return m_isInflating; }
	virtual float GetSpeed() const { return m_speed; }
	virtual dae::SpriteRenderComponent* GetSpriteRenderComponent() const;
	virtual MoveComponent* GetMoveComponent() const;
	virtual const dae::GameObject* GetClosestPlayer() const;
	virtual void SetPlayers(const std::vector<const dae::GameObject*>& players);
	virtual void RegisterObserver(dae::Observer* observer);
	virtual void UnregisterObserver(dae::Observer* observer);
protected:
	float m_speed;
	bool m_isInflating;
	bool m_isDead;
	glm::vec3 m_initialPosition;
	MoveComponent* m_pMoveComponent;
	dae::SpriteRenderComponent* m_pSpriteRenderComponent;
	std::vector<const dae::GameObject*> m_pPlayers;
	std::unique_ptr<dae::Subject> m_pDeathSubject;
};

