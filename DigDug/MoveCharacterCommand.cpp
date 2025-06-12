#include "MoveCharacterCommand.h"
#include "GameObject.h"
#include "MoveComponent.h"
#include <iostream>
#include <algorithm>

MoveCharacterCommand::MoveCharacterCommand(dae::GameObject* actor, glm::vec3 direction, float speed) :
    GameActorCommand(actor), m_direction(direction), m_speed(speed)
{
	m_moveComponent = actor->GetComponent<MoveComponent>();
}

//static float SnapToGrid(float value, float cellSize) {
//    return std::round(value / cellSize) * cellSize;
//}

void MoveCharacterCommand::Execute()
{
    constexpr float cellSize = 48.0f;
    auto* actor = GetGameActor();

    glm::vec3 pos = actor->GetTransform()->GetLocalPosition();

    glm::vec3 currentDir = m_moveComponent->GetFacingDirection();

    if ((currentDir.x == 0 && m_direction.x != 0 && currentDir.y != 0 && m_direction.y == 0) ||
    (currentDir.x != 0 && m_direction.x == 0 && currentDir.y == 0 && m_direction.y != 0)) {
        if (m_direction.x != 0) {
            float yAligned = std::fmod(pos.y, cellSize);
            if (yAligned <= 2.f) {
                actor->GetTransform()->SetLocalPosition(pos);
                m_moveComponent->SetMoveDirection(m_direction * m_speed);
            }
            else {
                m_moveComponent->SetMoveDirection(currentDir * m_speed);
            }
        }
        else if (m_direction.y != 0) {
            float xAligned = std::fmod(pos.x, cellSize);
            if (xAligned <= 2.f) {
                actor->GetTransform()->SetLocalPosition(pos);
                m_moveComponent->SetMoveDirection(m_direction * m_speed);
            }
            else {
                m_moveComponent->SetMoveDirection(currentDir * m_speed);
            }
        }
    }
    else {
        m_moveComponent->SetMoveDirection(m_direction * m_speed);
    }
}