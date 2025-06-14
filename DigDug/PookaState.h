#pragma once
#include <memory>
#include "glm.hpp"
#include "MoveCharacterCommand.h"
#include <random> // RNG should prolly be seeded in the minigin

class PookaComponent;
namespace PookaStates
{
	class PookaState
	{
	public:
		virtual ~PookaState() = default;
		virtual std::unique_ptr<PookaState> Update(PookaComponent* pooka) = 0;
		virtual void Exit(PookaComponent* pooka) = 0;
		virtual void Enter(PookaComponent* pooka) = 0;
	};

	class MovingState : public PookaState
	{
	public:
		std::unique_ptr<PookaState> Update(PookaComponent* pooka) override;
		void Exit(PookaComponent* pooka) override;
		void Enter(PookaComponent* pooka) override;
	private:
		bool BreakBounceLoop(PookaComponent* pooka);
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

	class InflatedState : public PookaState
	{
	public:
		std::unique_ptr<PookaState> Update(PookaComponent* pooka) override;
		void Exit(PookaComponent* pooka) override;
		void Enter(PookaComponent* pooka) override;
	private:
		void Inflate(PookaComponent* pooka);
		void Deflate(PookaComponent* pooka);
		int m_inflationProgress{};
		const int MAX_INFLATION_PROGRESS{ 3 };
		float m_deflationTimer{};
		const float DEFLATION_COOLDOWN{ 1.f };
	};

	class CrushedState : public PookaState
	{
	public:
		std::unique_ptr<PookaState> Update(PookaComponent* pooka) override;
		void Exit(PookaComponent* pooka) override;
		void Enter(PookaComponent* pooka) override;
	};

	class DyingState : public PookaState
	{
	public:
		std::unique_ptr<PookaState> Update(PookaComponent* pooka) override;
		void Exit(PookaComponent* pooka) override;
		void Enter(PookaComponent* pooka) override;
	private:
		float m_deathTimer{};
		const float DEATH_DURATION{ 1.0f };
	};

	class GhostState : public PookaState
	{
	public:
		std::unique_ptr<PookaState> Update(PookaComponent* pooka) override;
		void Exit(PookaComponent* pooka) override;
		void Enter(PookaComponent* pooka) override;
	private:
		const dae::GameObject* m_pTargetPlayer{ nullptr };
		float m_ghostingDuration{};
		const float MIN_GHOSTING_DURATION{ 3.0f };
		std::unique_ptr<MoveCommand> m_moveCommand{};
	};
}
