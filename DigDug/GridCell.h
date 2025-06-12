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
	~GridCell();
	GridCell(const GridCell& other) = delete;
	GridCell(GridCell&& other) = delete;
	GridCell& operator=(const GridCell& other) = delete;
	GridCell& operator=(GridCell&& other) = delete;
	void Update() override;
	void FixedUpdate() override;
	void Render() const override;
	bool IsSubCellDug(size_t index, bool horizontal);
	void OnNotify(dae::Event event) override;
	void CreateTunnel(bool horizontal);
	void Print() const;

	static constexpr int NUM_SUB_CELLS{ 8 };
private:
	void RevealSubcell(const dae::ColliderComponent& playerCollider);
	void DigCellCenter();
	void CleanSubCells();
	std::array<bool, NUM_SUB_CELLS> m_verticalSubCells, m_horizontalSubCells;
	dae::ColliderComponent* m_pColliderComponent;
	glm::vec3 m_previousPlayerDirection;
};