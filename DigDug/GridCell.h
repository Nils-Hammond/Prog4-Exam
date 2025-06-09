#pragma once
#include "BaseComponent.h"

class GridCell : public dae::BaseComponent
{
public:
	GridCell(dae::GameObject* owner, int x, int y);
	~GridCell() = default;
	GridCell(const GridCell& other) = delete;
	GridCell(GridCell&& other) = delete;
	GridCell& operator=(const GridCell& other) = delete;
	GridCell& operator=(GridCell&& other) = delete;
	void Update() override {};
	void FixedUpdate() override {};
private:
	int m_x;
	int m_y;
};