#include "GameConstants.h"
#include "GridCell.h"
#include "SDL.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "MoveComponent.h"

GridCell::GridCell(dae::GameObject* owner)
	: dae::BaseComponent(owner), m_horizontalSubCells(), m_verticalSubCells()
{
	m_pColliderComponent = GetOwner()->GetComponent<dae::ColliderComponent>();
	if (m_pColliderComponent == nullptr)
	{
		assert(false && "GridCell needs a ColliderComponent to function");
	}
	m_pColliderComponent->AddObserver(this);
	m_pColliderComponent->ResizeColliderRect(GRID_SIZE - 2, GRID_SIZE - 2);
	m_pColliderComponent->OffsetColliderRect(1, 1);
}

GridCell::~GridCell()
{
}

void GridCell::Update()
{
}

void GridCell::FixedUpdate()
{
}

void GridCell::Render() const
{
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);

	const auto& pos = GetOwner()->GetTransform()->GetWorldPosition();
	SDL_Rect horizontalRectUp{ static_cast<int>(pos.x), static_cast<int>(pos.y) + 2 * SPRITE_SCALE, GRID_SIZE / 16, GRID_SIZE -3 * SPRITE_SCALE};
	SDL_Rect horizontalRectDown{ static_cast<int>(pos.x), static_cast<int>(pos.y) + 1 * SPRITE_SCALE, GRID_SIZE / 16, GRID_SIZE - 3 * SPRITE_SCALE };
	for (int i{}; static_cast<unsigned>(i) < m_horizontalSubCells.size(); ++i)
	{
		if (m_horizontalSubCells[i])
		{
			if (i % 2 == 0)
			{
				horizontalRectUp.x = static_cast<int>(pos.x) + GRID_SIZE / 16 * (i * 2);
				horizontalRectDown.x = static_cast<int>(pos.x) + GRID_SIZE / 16 * (i * 2 + 1);
			}
			else
			{
				horizontalRectUp.x = static_cast<int>(pos.x) + GRID_SIZE / 16 * (i * 2 + 1);
				horizontalRectDown.x = static_cast<int>(pos.x) + GRID_SIZE / 16 * (i * 2);
			}
			SDL_RenderFillRect(sdlRenderer, &horizontalRectUp);
			SDL_RenderFillRect(sdlRenderer, &horizontalRectDown);
		}
	}
	SDL_Rect verticalRectleft{ static_cast<int>(pos.x) + 1 * SPRITE_SCALE, static_cast<int>(pos.y), GRID_SIZE - 3 * SPRITE_SCALE, GRID_SIZE / 16 };
	SDL_Rect verticalRectright{ static_cast<int>(pos.x) + 2 * SPRITE_SCALE, static_cast<int>(pos.y), GRID_SIZE - 3 * SPRITE_SCALE, GRID_SIZE / 16 };
	for (int i{}; static_cast<unsigned>(i) < m_verticalSubCells.size(); ++i)
	{
		if (m_verticalSubCells[i])
		{
			if (i % 2 == 0)
			{
				verticalRectleft.y = static_cast<int>(pos.y) + GRID_SIZE / 16 * (i * 2);
				verticalRectright.y = static_cast<int>(pos.y) + GRID_SIZE / 16 * (i * 2 + 1);
			}
			else
			{
				verticalRectleft.y = static_cast<int>(pos.y) + GRID_SIZE / 16 * (i * 2 + 1);
				verticalRectright.y = static_cast<int>(pos.y) + GRID_SIZE / 16 * (i * 2);
			}
			SDL_RenderFillRect(sdlRenderer, &verticalRectleft);
			SDL_RenderFillRect(sdlRenderer, &verticalRectright);
		}
	}
}

void GridCell::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("OnCollision"))
	{
		try
		{
			auto* collider = std::any_cast<const dae::ColliderComponent*>(event.data);
			if (collider && collider->GetTag() == dae::make_sdbm_hash("Player"))
			{
				RevealSubcell(*collider);
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting collision data: " << e.what();
			return;
		}
	}
}

void GridCell::RevealSubcell(const dae::ColliderComponent& playerCollider)
{
	const auto& playerComponent = playerCollider.GetOwner()->GetComponent<PlayerComponent>();
	const glm::vec3& playerMovementDirection = playerComponent->GetMoveComponent()->GetFacingDirection();
	const glm::vec3& gridPos = GetOwner()->GetTransform()->GetWorldPosition();

	if (playerMovementDirection.x != 0 && playerMovementDirection.y == 0)
	{
		SDL_Rect horizontalRect{ static_cast<int>(gridPos.x), static_cast<int>(gridPos.y) + GRID_SIZE / 4, GRID_SIZE / NUM_SUB_CELLS - 4, GRID_SIZE / 2 };
		for (int i{}; static_cast<unsigned>(i) < m_horizontalSubCells.size(); ++i)
		{
			horizontalRect.x = static_cast<int>(gridPos.x) + GRID_SIZE / NUM_SUB_CELLS * i + 2;
			if (SDL_HasIntersection(&horizontalRect, &playerCollider.GetColliderRect()))
			{
				m_horizontalSubCells[i] = true;
			}
		}
	}
	else if (playerMovementDirection.x == 0 && playerMovementDirection.y != 0)
	{
		SDL_Rect verticalRect{ static_cast<int>(gridPos.x) + GRID_SIZE / 4, static_cast<int>(gridPos.y), GRID_SIZE / 2, GRID_SIZE / NUM_SUB_CELLS - 4};
		for (int i{}; static_cast<unsigned>(i) < m_verticalSubCells.size(); ++i)
		{
			verticalRect.y = static_cast<int>(gridPos.y) + GRID_SIZE / NUM_SUB_CELLS * i + 2;
			if (SDL_HasIntersection(&verticalRect, &playerCollider.GetColliderRect()))
			{
				m_verticalSubCells[i] = true;
			}
		}
	}
}