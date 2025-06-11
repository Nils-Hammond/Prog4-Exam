#pragma once
#include "BaseComponent.h"
#include "glm.hpp"
#include <array>
#include "SubjectObserver.h"

namespace dae {
	class ColliderComponent;
}
class GridCell : public dae::BaseComponent, public dae::Observer
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
	void OnNotify(dae::Event event) override;
private:
	void RevealSubcell(const dae::ColliderComponent& playerCollider);
	static constexpr int NUM_SUB_CELLS{ 8 };
	std::array<bool, NUM_SUB_CELLS> m_verticalSubCells, m_horizontalSubCells;
	dae::ColliderComponent* m_pColliderComponent;
};