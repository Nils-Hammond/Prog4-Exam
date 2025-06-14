#include "MoveComponent.h"
#include "GameObject.h"
#include "DaeTime.h"
#include "RenderComponent.h"
#include "GridCell.h"
#include "GameConstants.h"
#include "ColliderComponent.h"
#include "Renderer.h"
#include "ServiceLocator.h"
#include <algorithm>

bool IsOutOfBounds(const glm::vec3& pos);

MoveComponent::MoveComponent(dae::GameObject* owner, const std::array<GridCell*, LEVEL_WIDTH* LEVEL_HEIGHT>& grid, bool canRotate, bool canDig)
	: dae::BaseComponent(owner), m_direction(), m_oldDirection(),
	m_renderComponent(nullptr), m_collider(nullptr),
	m_isFacingRight(true), m_isMoving(false), m_isActive(true), m_canDig(canDig), m_grid(grid), m_hitWall(false), m_canRotate(canRotate)
{
	m_renderComponent = GetOwner()->GetComponent<dae::RenderComponent>();
	if (!m_renderComponent)
	{
		assert(false && "MoveComponent requires a RenderComponent to function properly.");
	}
	m_collider = GetOwner()->GetComponent<dae::ColliderComponent>();
	if (!m_collider)
	{
		assert(false && "MoveComponent requires a ColliderComponent to function properly.");
	}
}

MoveComponent::~MoveComponent() = default;

void MoveComponent::Update()
{
	if (m_direction == glm::vec3(0.f, 0.f, 0.f) || !m_isActive)
	{

		const glm::vec3& oldPosition = GetOwner()->GetTransform()->GetLocalPosition();
		m_hitWall = !CanMoveTo(glm::vec2(oldPosition.x, oldPosition.y));
		m_isMoving = false;
		//std::string debugMessage = m_hitWall ? "Inside a wall\n" : "In a free space\n";
		//std::cout << debugMessage;
		return;
	}
	glm::vec2 scale = m_renderComponent->GetScale();
	if (m_direction.x > 0.f)
	{
		m_isFacingRight = true;
		m_renderComponent->SetScale(scale.x > 0 ? scale.x : -scale.x, scale.y);
	}
	else if (m_direction.x < 0.f)
	{
		m_isFacingRight = false;
		m_renderComponent->SetScale(scale.x > 0 ? -scale.x : scale.x, scale.y);
	}
	if (m_canRotate)
	{
		if (m_direction.y > 0.f)
			m_renderComponent->SetAngle(scale.x > 0 ? 90.f : -90.f);
		else if (m_direction.y < 0.f)
			m_renderComponent->SetAngle(scale.x > 0 ? -90.f : 90.f);
		else
			m_renderComponent->SetAngle(0.f);
	}

	const glm::vec3& oldPosition = GetOwner()->GetTransform()->GetLocalPosition();
	const glm::vec3& newPosition = oldPosition + m_direction * dae::Time::GetInstance().GetDeltaTime();
	bool canMove = CanMoveTo(glm::vec2(newPosition.x, newPosition.y));
	m_hitWall = !canMove;
	//std::string debugMessage = canMove ? "Moving to new position\n" : "Cannot move to new position\n";
	//std::cout << debugMessage;
	if (((m_canDig && !CheckRockCollision(newPosition)) || canMove) && !IsOutOfBounds(newPosition))
	{
		GetOwner()->GetTransform()->SetLocalPosition(newPosition);
	}
	else
		m_isMoving = false;
	m_direction = glm::vec3(0.f, 0.f, 0.f);
}

bool IsOutOfBounds(const glm::vec3& pos)
{
	return pos.x < 0.f || pos.x >= (LEVEL_WIDTH - 1) * GRID_SIZE + 2 ||
		pos.y < 0.f || pos.y >= (LEVEL_HEIGHT - 1) * GRID_SIZE + 2;
}

void MoveComponent::SetMoveDirection(glm::vec3 direction)
{
	if (!m_isActive)
		return;
	m_isMoving = true;
	m_direction = direction;
	m_oldDirection = direction;
	m_oldDirection /= glm::length(m_oldDirection);
}

bool MoveComponent::IsMoving() const
{
	return m_isMoving;
}

bool MoveComponent::CanMoveTo(const glm::vec2& nextPos)
{
	if (IsOutOfBounds(glm::vec3(nextPos.x, nextPos.y, 0)))
		return false;
	SDL_Rect colliderRect = m_collider->GetColliderRect();
	const int xOffset = static_cast<int>(colliderRect.x - m_collider->GetOwner()->GetTransform()->GetWorldPosition().x);
	const int yOffset = static_cast<int>(colliderRect.y - m_collider->GetOwner()->GetTransform()->GetWorldPosition().y);

	colliderRect.x = static_cast<int>(nextPos.x) + xOffset;
	colliderRect.y = static_cast<int>(nextPos.y) + yOffset;

	// Math done by my best friend ChatGPT
	// Check which grid cells the collider is covering
	int cellX_start = colliderRect.x / GRID_SIZE;
	int cellX_end = std::min((colliderRect.x + colliderRect.w - 1) / GRID_SIZE, LEVEL_WIDTH - 1);
	int cellY_start = colliderRect.y / GRID_SIZE;
	int cellY_end = std::min((colliderRect.y + colliderRect.h - 1) / GRID_SIZE, LEVEL_HEIGHT - 1);

	const int subcellSize = GRID_SIZE / GridCell::NUM_SUB_CELLS;

	for (int cellY = cellY_start; cellY <= cellY_end; ++cellY)
	{
		for (int cellX = cellX_start; cellX <= cellX_end; ++cellX)
		{
			GridCell* cell = m_grid[cellY * LEVEL_WIDTH + cellX];

			if (GetFacingDirection().x != 0 && GetFacingDirection().y == 0)
			{
				// Horizontal movement
				for (int subCellX{}; subCellX < GridCell::NUM_SUB_CELLS; ++subCellX)
				{
					if (!cell->IsSubCellDug(subCellX, true))
					{
						SDL_Rect subCellRect{};
						subCellRect.x = cellX * GRID_SIZE + subCellX * subcellSize;
						subCellRect.y = cellY * GRID_SIZE + GRID_SIZE / 4;
						subCellRect.w = subcellSize;
						subCellRect.h = GRID_SIZE / 2;
						if (SDL_HasIntersection(&colliderRect, &subCellRect))
						{
							return false; // Collision detected
						}
					}
				}
			}
			else if (GetFacingDirection().y != 0 && GetFacingDirection().x == 0)
			{
				// Vertical movement
				for (int subCellY{}; subCellY < GridCell::NUM_SUB_CELLS; ++subCellY)
				{
					if (!cell->IsSubCellDug(subCellY, false))
					{
						SDL_Rect subCellRect{};
						subCellRect.x = cellX * GRID_SIZE + GRID_SIZE / 4;
						subCellRect.y = cellY * GRID_SIZE + subCellY * subcellSize;
						subCellRect.w = GRID_SIZE / 2;
						subCellRect.h = subcellSize;
						if (SDL_HasIntersection(&colliderRect, &subCellRect))
						{
							return false; // Collision detected
						}
					}
				}
			}
		}
	}
	return true;
}

bool MoveComponent::CheckRockCollision(const glm::vec2& nextPos)
{
	SDL_Rect colliderRect = m_collider->GetColliderRect();
	const int xOffset = static_cast<int>(colliderRect.x - m_collider->GetOwner()->GetTransform()->GetWorldPosition().x);
	const int yOffset = static_cast<int>(colliderRect.y - m_collider->GetOwner()->GetTransform()->GetWorldPosition().y);

	colliderRect.x = static_cast<int>(nextPos.x) + xOffset;
	colliderRect.y = static_cast<int>(nextPos.y) + yOffset;

	std::vector<dae::ColliderComponent*> collisions = dae::ServiceLocator::GetCollisionSystem().GetCollisions(colliderRect);
	for (const auto& collider : collisions)
	{
		if (collider->GetTag() == dae::make_sdbm_hash("Rock"))
		{
			return true;
		}
	}
	return false;
}

void MoveComponent::Render() const
{
	// A whole bunch of debug yap to make sure collision detection works
	/*
	const glm::vec3& oldPosition = GetOwner()->GetTransform()->GetLocalPosition();
	const glm::vec3& nextPos = oldPosition + m_direction * dae::Time::GetInstance().GetDeltaTime();
	SDL_Rect colliderRect = m_collider->GetColliderRect();
	const int xOffset = static_cast<int>(colliderRect.x - m_collider->GetOwner()->GetTransform()->GetWorldPosition().x);
	const int yOffset = static_cast<int>(colliderRect.y - m_collider->GetOwner()->GetTransform()->GetWorldPosition().y);

	colliderRect.x = static_cast<int>(nextPos.x) + xOffset;
	colliderRect.y = static_cast<int>(nextPos.y) + yOffset;

	// Math done by my best friend ChatGPT
	// Check which grid cells the collider is covering
	int cellX_start = colliderRect.x / GRID_SIZE;
	int cellX_end = (colliderRect.x + colliderRect.w - 1) / GRID_SIZE;
	int cellY_start = colliderRect.y / GRID_SIZE;
	int cellY_end = (colliderRect.y + colliderRect.h - 1) / GRID_SIZE;

	const int subcellSize = GRID_SIZE / GridCell::NUM_SUB_CELLS;

	//bool display = m_collider->GetTag() == dae::make_sdbm_hash("Player") ? true : false;
	bool display = false;

	if (display)
		std::cout << "---- Starting Prints ----" << std::endl;

	for (int cellY = cellY_start; cellY <= cellY_end; ++cellY)
	{
		for (int cellX = cellX_start; cellX <= cellX_end; ++cellX)
		{
			GridCell* cell = m_grid[cellY * LEVEL_WIDTH + cellX];
			if (display)
				std::cout << "----- Vertical Collisions -----" << std::endl;
			for (int subCellY = 0; subCellY < GridCell::NUM_SUB_CELLS; ++subCellY)
			{
				//if (!cell->IsSubCellDug(subCellY, false))
				//{
					SDL_Rect subCellRect{};
					subCellRect.x = cellX * GRID_SIZE + GRID_SIZE / 4;
					subCellRect.y = cellY * GRID_SIZE + subCellY * subcellSize;
					subCellRect.w = GRID_SIZE / 2;
					subCellRect.h = subcellSize;
					if (SDL_HasIntersection(&colliderRect, &subCellRect))
					{
						if (!cell->IsSubCellDug(subCellY, false))
						{
							SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 0, 255, 255);
							SDL_RenderFillRect(dae::Renderer::GetInstance().GetSDLRenderer(), &subCellRect);
						}
						else
						{
							SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255);
							SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &subCellRect);
						}
					}
					else
					{
						SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
						SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &subCellRect);
					}
				//}
			}
			if (display)
				std::cout << "----- Horizontal Collisions -----" << std::endl;
			for (int subCellX = 0; subCellX < GridCell::NUM_SUB_CELLS; ++subCellX)
			{
				//if (!cell->IsSubCellDug(subCellX, true))
				//{
				SDL_Rect subCellRect{};
				subCellRect.x = cellX * GRID_SIZE + subCellX * subcellSize;
				subCellRect.y = cellY * GRID_SIZE + GRID_SIZE / 4;
				subCellRect.w = subcellSize;
				subCellRect.h = GRID_SIZE / 2;
				if (SDL_HasIntersection(&colliderRect, &subCellRect))
				{
					SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255);
					if (!cell->IsSubCellDug(subCellX, true))
					{
						SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 0, 255, 255);
						SDL_RenderFillRect(dae::Renderer::GetInstance().GetSDLRenderer(), &subCellRect);
					}
					else
					{
						SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255);
						SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &subCellRect);
					}
				}
				else
				{
					SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255);
					SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &subCellRect);
				}
				//}
			}
		}
	}
	SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &colliderRect);
	if (display)
		std::cout << "------------------------------" << std::endl;
		*/
}