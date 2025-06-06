#pragma once
#include "Command.h"
#include "glm.hpp"

class GameActorCommand : public dae::Command
{
	dae::GameObject* m_actor;
protected:
	dae::GameObject* GetGameActor() const { return m_actor; }
public:
	GameActorCommand(dae::GameObject* actor);
	virtual ~GameActorCommand() = default;
};

class MoveCommand : public GameActorCommand
{
	glm::vec3 m_direction;
public:
	MoveCommand(dae::GameObject* actor, glm::vec3 direction);
	void Execute() override;
};

class DieCommand : public GameActorCommand
{
public:
	DieCommand(dae::GameObject* actor);
	void Execute() override;
};
