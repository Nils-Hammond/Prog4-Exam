#pragma once
#include <memory>
#include "glm.hpp"
#include "MoveCharacterCommand.h"
#include <random>


class FygarComponent;
namespace FygarStates
{
	class FygarState
	{
	public:
		virtual ~FygarState() = default;
		virtual std::unique_ptr<FygarState> Update(FygarComponent* fygar) = 0;
		virtual void Exit(FygarComponent* fygar) = 0;
		virtual void Enter(FygarComponent* fygar) = 0;
	};

	class IdleState : public FygarState
	{
	public:
		std::unique_ptr<FygarState> Update(FygarComponent* fygar) override;
		void Exit(FygarComponent* fygar) override;
		void Enter(FygarComponent* fygar) override;
	};

	class MovingState : public FygarState
	{
	public:
		std::unique_ptr<FygarState> Update(FygarComponent* fygar) override;
		void Exit(FygarComponent* fygar) override;
		void Enter(FygarComponent* fygar) override;
	private:
		bool BreakBounceLoop(FygarComponent* fygar);
		glm::vec3 GetRandomGridDirection();
		std::mt19937 m_randomNumGen{};
		glm::vec3 m_desiredDirection{};
		float m_timeSinceLastDirectionChange{};
		const float DIRECTION_CHANGE_INTERVAL{ 2.0f };
		float m_ghostCooldownTimer{};
		float m_ghostCooldownDuration{};
		const float MAX_GHOST_COOLDOWN_DURATION{ 50.0f };
		const float MIN_GHOST_COOLDOWN_DURATION{ 20.0f };
		int m_bouncesSinceLastDirectionChange{};
		std::unique_ptr<MoveCharacterCommand> m_moveCommand{};
	};

	class InflatedState : public FygarState
	{
	public:
		std::unique_ptr<FygarState> Update(FygarComponent* fygar) override;
		void Exit(FygarComponent* fygar) override;
		void Enter(FygarComponent* fygar) override;
	private:
		void Inflate(FygarComponent* fygar);
		void Deflate(FygarComponent* fygar);
		int m_inflationProgress{};
		const int MAX_INFLATION_PROGRESS{ 3 };
		float m_deflationTimer{};
		const float DEFLATION_COOLDOWN{ 1.f };
	};

	class AttackingState : public FygarState
	{
	public:
		std::unique_ptr<FygarState> Update(FygarComponent* fygar) override;
		void Exit(FygarComponent* fygar) override;
		void Enter(FygarComponent* fygar) override;
	};

	class DyingState : public FygarState
	{
	public:
		std::unique_ptr<FygarState> Update(FygarComponent* fygar) override;
		void Exit(FygarComponent* fygar) override;
		void Enter(FygarComponent* fygar) override;
	private:
		float m_deathTimer{};
		const float DEATH_DURATION{ 1.0f };
	};

	class CrushedState : public FygarState
	{
	public:
		std::unique_ptr<FygarState> Update(FygarComponent* fygar) override;
		void Exit(FygarComponent* fygar) override;
		void Enter(FygarComponent* fygar) override;
	};

	class GhostState : public FygarState
	{
	public:
		std::unique_ptr<FygarState> Update(FygarComponent* fygar) override;
		void Exit(FygarComponent* fygar) override;
		void Enter(FygarComponent* fygar) override;
	private:
		const dae::GameObject* m_pTargetPlayer{ nullptr };
		float m_ghostingDuration{};
		const float MIN_GHOSTING_DURATION{ 3.0f };
		std::unique_ptr<MoveCommand> m_moveCommand{};
	};
}