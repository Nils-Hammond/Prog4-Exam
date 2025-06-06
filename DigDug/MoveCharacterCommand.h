#pragma once
#include "GameCommands.h"

class MoveComponent;

class MoveCharacterCommand : public GameActorCommand
{
	glm::vec3 m_direction;
	float m_speed;
	MoveComponent* m_moveComponent;
public:
	MoveCharacterCommand(dae::GameObject* actor, glm::vec3 direction, float speed = 100.f);
	void Execute() override;
};

