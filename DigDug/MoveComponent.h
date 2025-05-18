#pragma once
#include "BaseComponent.h"
#include "glm.hpp"

class MoveComponent : public dae::BaseComponent
{
public:
	MoveComponent(dae::GameObject* owner, float speed);
	~MoveComponent() = default;
	MoveComponent(const MoveComponent& other) = delete;
	MoveComponent(MoveComponent&& other) = delete;
	MoveComponent& operator=(const MoveComponent& other) = delete;
	MoveComponent& operator=(MoveComponent&& other) = delete;

	void Update() override;
	void SetMoveDirection(glm::vec3 direction);

private:
	float m_speed;
	glm::vec3 m_direction;
};