#pragma once

class PlayerComponent;

class PlayerState
{
public:
	virtual ~PlayerState() {};
	virtual PlayerState* Update(PlayerComponent* player) = 0;
	virtual void Exit(PlayerComponent* player) = 0;
	virtual void Enter(PlayerComponent* player) = 0;
};

class IdleState : public PlayerState
{
public:
	PlayerState* Update(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Enter(PlayerComponent* player) override;
};

class MovingState : public PlayerState
{
public:
	PlayerState* Update(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Enter(PlayerComponent* player) override;
};

class AttackingState : public PlayerState
{
public:
	PlayerState* Update(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Enter(PlayerComponent* player) override;
};

class DyingState : public PlayerState
{
public:
	PlayerState* Update(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Enter(PlayerComponent* player) override;
};

class CrushedState : public PlayerState
{
public:
	PlayerState* Update(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Enter(PlayerComponent* player) override;
};