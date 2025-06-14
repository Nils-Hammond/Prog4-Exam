#pragma once
#include <memory>
//#include "PumpComponent.h"

namespace dae
{
	class ColliderComponent;
}

class PlayerComponent;
class EnemyComponent;
class PumpComponent;

namespace PlayerStates
{
	class PlayerState
	{
	public:
		virtual ~PlayerState() = default;
		virtual std::unique_ptr<PlayerState> Update(PlayerComponent* player) = 0;
		virtual void Exit(PlayerComponent* player) = 0;
		virtual void Enter(PlayerComponent* player) = 0;
	};

	class IdleState : public PlayerState
	{
	public:
		std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
		void Exit(PlayerComponent* player) override;
		void Enter(PlayerComponent* player) override;
	};

	class MovingState : public PlayerState
	{
	public:
		std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
		void Exit(PlayerComponent* player) override;
		void Enter(PlayerComponent* player) override;
	};

	class DiggingState : public PlayerState
	{
	public:
		std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
		void Exit(PlayerComponent* player) override;
		void Enter(PlayerComponent* player) override;
	private:
		float shovelTimer{};
		const float MAX_SHOVEL_DURATION{ 0.5f };
	};

	class AttackingState : public PlayerState
	{
	public:
		std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
		void Exit(PlayerComponent* player) override;
		void Enter(PlayerComponent* player) override;
	private:
		PumpComponent* m_pumpComponent{};
		bool m_pumpConnected{ false };
		const dae::ColliderComponent* m_pEnemyCollider{ nullptr };
		float m_attackTimer{};
		const float ATTACK_COOLDOWN{ 0.55f };
		EnemyComponent* m_pEnemy;
	};

	class DyingState : public PlayerState
	{
	public:
		std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
		void Exit(PlayerComponent* player) override;
		void Enter(PlayerComponent* player) override;
	private:
		float m_deathTimer{};
		const float DEATH_DURATION{ 1.0f };
		bool m_isFullyDead{};
	};

	class CrushedState : public PlayerState
	{
	public:
		std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
		void Exit(PlayerComponent* player) override;
		void Enter(PlayerComponent* player) override;
	};
}