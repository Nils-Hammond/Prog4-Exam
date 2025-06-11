#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
#include <array>

class CollisionComponent;
class GridCell : public dae::BaseComponent
{
public:
	GridCell(dae::GameObject* owner);
	GridCell(dae::GameObject* owner, std::array<bool, 8> verticalCells, std::array<bool, 8> horiontalCells);
	~GridCell();
	GridCell(const GridCell& other) = delete;
	GridCell(GridCell&& other) = delete;
	GridCell& operator=(const GridCell& other) = delete;
	GridCell& operator=(GridCell&& other) = delete;
	void Update() override;
	void FixedUpdate() override;
	void Render() const;
private:
	std::array<bool, 8> m_verticalSubCells, m_horizontalSubCells;
};