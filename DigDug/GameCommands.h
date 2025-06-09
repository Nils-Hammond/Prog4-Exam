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

class PlaySoundCommand : public dae::Command
{
	std::string m_soundFile;
	int m_volume;
	bool m_loop;
	int m_channel;
public:
	PlaySoundCommand(const std::string& soundFile, int volume = 128, bool loop = false, int channel = -1);
	void Execute() override;
};