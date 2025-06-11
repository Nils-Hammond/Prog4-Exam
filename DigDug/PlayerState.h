#pragma once
#include <memory>
//#include "PumpComponent.h"

class PlayerComponent;
class PumpComponent;

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
};

class AttackingState : public PlayerState
{
public:
	std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Enter(PlayerComponent* player) override;
private:
	PumpComponent* m_pumpComponent;
};

class DyingState : public PlayerState
{
public:
	std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Enter(PlayerComponent* player) override;
};

class CrushedState : public PlayerState
{
public:
	std::unique_ptr<PlayerState> Update(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Enter(PlayerComponent* player) override;
};