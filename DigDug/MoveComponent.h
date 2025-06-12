#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
#include <array>
#include "GameConstants.h"

namespace dae {
	class ColliderComponent;
	class RenderComponent;
}
class GridCell;
class MoveComponent : public dae::BaseComponent
{
public:
	MoveComponent(dae::GameObject* owner, const std::array<GridCell*, LEVEL_WIDTH * LEVEL_HEIGHT>& grid, bool canDig = false);
	~MoveComponent();
	MoveComponent(const MoveComponent& other) = delete;
	MoveComponent(MoveComponent&& other) = delete;
	MoveComponent& operator=(const MoveComponent& other) = delete;
	MoveComponent& operator=(MoveComponent&& other) = delete;

	void Update() override;
	void Render() const override;
	void SetMoveDirection(glm::vec3 direction);
	glm::vec3 GetFacingDirection() const { return m_oldDirection; }
	bool IsFacingRight() const { return m_isFacingRight; }
	bool IsMoving() const;
	void SetActive(bool isActive) { m_isActive = isActive; }
private:
	bool CanMoveTo(const glm::vec2& newPos);
	glm::vec3 m_direction;
	glm::vec3 m_oldDirection;
	bool m_isFacingRight;
	bool m_isMoving;
	bool m_isActive;
	bool m_canDig;
	std::array<GridCell*, LEVEL_WIDTH * LEVEL_HEIGHT> m_grid;
	dae::RenderComponent* m_renderComponent;
	dae::ColliderComponent* m_collider;
};