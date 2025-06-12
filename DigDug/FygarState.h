#pragma once
#include <memory>

class FygarComponent;
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
};


