#include "GameCommands.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include "LivesAndPoints.h"
#include <algorithm>

GameActorCommand::GameActorCommand(dae::GameObject* actor) :
	m_actor{ actor }
{
}

MoveCommand::MoveCommand(dae::GameObject* actor, glm::vec3 direction)
	: GameActorCommand(actor), m_direction(direction)
{
}

void MoveCommand::Execute()
{
	MoveComponent* moveComponent = GetGameActor()->GetComponent<MoveComponent>();
	if (moveComponent != nullptr)
	{
		moveComponent->SetMoveDirection(m_direction);
	}
}

MoveCharacterCommand::MoveCharacterCommand(dae::GameObject* actor, glm::vec3 direction, float speed) :
	GameActorCommand(actor), m_direction(direction), m_speed(speed)
{
}

static float SnapToGrid(float value, float cellSize) {
    return std::round(value / cellSize) * cellSize;
}

void MoveCharacterCommand::Execute()
{
    constexpr float cellSize = 32.0f; // Move this somewhere else
    auto* actor = GetGameActor();

    glm::vec3 pos = actor->GetTransform()->GetLocalPosition();

    if (m_direction.x == 0.f && m_direction.y != 0.f) {
        float xAligned = std::fmod(pos.x, cellSize);
        if (xAligned != 0.f) {
            float targetX = SnapToGrid(pos.x, cellSize);
            float deltaX = targetX - pos.x;
            float moveX = std::clamp(deltaX, -m_speed, m_speed);
            if (std::abs(deltaX) <= 2.f) {
                pos.x = targetX;
                actor->GetTransform()->SetLocalPosition(pos);
                actor->GetComponent<MoveComponent>()->SetMoveDirection(m_direction);
            }
            else {
                actor->GetComponent<MoveComponent>()->SetMoveDirection(glm::vec3((moveX > 0) ? 1.f : -1.f, 0.f, 0.f));
            }
            return;
        }
    }
    else if (m_direction.y == 0.f && m_direction.x != 0.f) {
        float yAligned = std::fmod(pos.y, cellSize);
        if (yAligned != 0.f) {
            float targetY = SnapToGrid(pos.y, cellSize);
            float deltaY = targetY - pos.y;
            float moveY = std::clamp(deltaY, -m_speed, m_speed);
            if (std::abs(deltaY) <= 2.f) {
                pos.y = targetY;
                actor->GetTransform()->SetLocalPosition(pos);
                actor->GetComponent<MoveComponent>()->SetMoveDirection(m_direction);
            }
            else {
                actor->GetComponent<MoveComponent>()->SetMoveDirection(glm::vec3(0.f, (moveY > 0) ? 1.f : -1.f, 0.f));
            }
            return;
        }
    }
    actor->GetComponent<MoveComponent>()->SetMoveDirection(m_direction);
}

DieCommand::DieCommand(dae::GameObject* actor) :
	GameActorCommand(actor)
{
}

void DieCommand::Execute()
{
	HealthComponent* healthComponent = GetGameActor()->GetComponent<HealthComponent>();
	if (healthComponent != nullptr)
	{
		healthComponent->TakeDamage(healthComponent->GetHealth());
	}
}