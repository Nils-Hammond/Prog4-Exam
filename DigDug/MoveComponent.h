#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
#include "RenderComponent.h"

class MoveComponent : public dae::BaseComponent
{
public:
	MoveComponent(dae::GameObject* owner);
	~MoveComponent() = default;
	MoveComponent(const MoveComponent& other) = delete;
	MoveComponent(MoveComponent&& other) = delete;
	MoveComponent& operator=(const MoveComponent& other) = delete;
	MoveComponent& operator=(MoveComponent&& other) = delete;

	void Update() override;
	void SetMoveDirection(glm::vec3 direction);
	glm::vec3 GetFacingDirection() const { return m_oldDirection; }
	bool IsFacingRight() const { return m_isFacingRight; }

private:
	glm::vec3 m_direction;
	glm::vec3 m_oldDirection;
	bool m_isFacingRight;
	dae::RenderComponent* m_renderComponent;
};