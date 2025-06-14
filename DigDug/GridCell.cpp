#include "GameConstants.h"
#include "GridCell.h"
#include "SDL.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "MoveComponent.h"

GridCell::GridCell(dae::GameObject* owner)
	: dae::BaseComponent(owner), m_horizontalSubCells(), m_verticalSubCells(), m_previousPlayerDirection(0.f, 0.f, 0.f)
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
	// Need some helper functions for manual pixel drawing (EngineUtils.h) but not enough time
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
	// Extra rendering for debugging purposes
	/*
	const glm::vec3& gridPos = GetOwner()->GetTransform()->GetWorldPosition();
	SDL_Rect horizontalRect{ static_cast<int>(gridPos.x), static_cast<int>(gridPos.y) + GRID_SIZE, GRID_SIZE / NUM_SUB_CELLS - 4, GRID_SIZE };
	for (int i{}; static_cast<unsigned>(i) < m_horizontalSubCells.size(); ++i)
	{
		horizontalRect.x = static_cast<int>(gridPos.x) + GRID_SIZE / NUM_SUB_CELLS * i + 2;
		SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(sdlRenderer, &horizontalRect);
	}
	SDL_Rect verticalRect{ static_cast<int>(gridPos.x) + GRID_SIZE, static_cast<int>(gridPos.y), GRID_SIZE, GRID_SIZE / NUM_SUB_CELLS - 4 };
	for (int i{}; static_cast<unsigned>(i) < m_verticalSubCells.size(); ++i)
	{
		verticalRect.y = static_cast<int>(gridPos.y) + GRID_SIZE / NUM_SUB_CELLS * i + 2;
		SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(sdlRenderer, &verticalRect);
	}
	*/
}

bool GridCell::IsSubCellDug(size_t index, bool horizontal)
{
	if (horizontal)
	{
		if (index < m_horizontalSubCells.size())
		{
			return m_horizontalSubCells[index];
		}
	}
	else
	{
		if (index < m_verticalSubCells.size())
		{
			return m_verticalSubCells[index];
		}
	}
	return false;
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

void GridCell::CreateTunnel(bool horizontal)
{
	if (horizontal)
	{
		for (auto& subCell : m_horizontalSubCells)
		{
			subCell = true;
		}
		for (size_t i = 1; i < m_verticalSubCells.size() - 1; ++i)
		{
			m_verticalSubCells[i] = true;
		}
	}
	else
	{
		for (auto& subCell : m_verticalSubCells)
		{
			subCell = true;
		}
		for (size_t i = 1; i < m_horizontalSubCells.size() - 1; ++i)
		{
			m_horizontalSubCells[i] = true;
		}
	}
}

void GridCell::Print() const
{
	std::cout << "Horizontal Subcells: ";
	for (size_t i{}; i < m_horizontalSubCells.size(); ++i)
	{
		std::cout << (m_horizontalSubCells[i] ? "1" : "0");
	}
	std::cout << std::endl;
	std::cout << "Vertical Subcells: ";
	for (size_t i{}; i < m_verticalSubCells.size(); ++i)
	{
		std::cout << (m_verticalSubCells[i] ? "1" : "0");
	}
	std::cout << std::endl;
}

void GridCell::RevealSubcell(const dae::ColliderComponent& playerCollider)
{
	const auto& playerComponent = playerCollider.GetOwner()->GetComponent<PlayerComponent>();
	const glm::vec3& playerMovementDirection = playerComponent->GetMoveComponent()->GetFacingDirection();
	const glm::vec3& gridPos = GetOwner()->GetTransform()->GetWorldPosition();

	
	if (playerMovementDirection.x != 0 && playerMovementDirection.y == 0 || playerMovementDirection.x == 0 && m_previousPlayerDirection.x != 0)
	{
		SDL_Rect horizontalRect{ static_cast<int>(gridPos.x), static_cast<int>(gridPos.y), GRID_SIZE / NUM_SUB_CELLS - 4, GRID_SIZE };
		for (int i{}; static_cast<unsigned>(i) < m_horizontalSubCells.size(); ++i)
		{
			horizontalRect.x = static_cast<int>(gridPos.x) + GRID_SIZE / NUM_SUB_CELLS * i + 2;
			if (!m_horizontalSubCells[i] && SDL_HasIntersection(&horizontalRect, &playerCollider.GetColliderRect()))
			{
				m_horizontalSubCells[i] = true;
			}
		}
	}
	if (playerMovementDirection.x == 0 && playerMovementDirection.y != 0 || playerMovementDirection.y == 0 && m_previousPlayerDirection.y != 0)
	{
		SDL_Rect verticalRect{ static_cast<int>(gridPos.x), static_cast<int>(gridPos.y), GRID_SIZE, GRID_SIZE / NUM_SUB_CELLS - 4};
		for (int i{}; static_cast<unsigned>(i) < m_verticalSubCells.size(); ++i)
		{
			verticalRect.y = static_cast<int>(gridPos.y) + GRID_SIZE / NUM_SUB_CELLS * i + 2;
			if (!m_verticalSubCells[i] && SDL_HasIntersection(&verticalRect, &playerCollider.GetColliderRect()))
			{
				m_verticalSubCells[i] = true;
			}
		}
	}
	CleanSubCells();
}

void GridCell::DigCellCenter()
{
	for (size_t i = 1; i < m_horizontalSubCells.size() - 1; ++i)
	{
		m_horizontalSubCells[i] = true;
	}
	for (size_t i = 1; i < m_verticalSubCells.size() - 1; ++i)
	{
		m_verticalSubCells[i] = true;
	}
}

// Band-aid fix for my subpar collision detection
void GridCell::CleanSubCells()
{
	// Fix corners
	bool centerFourHorizontalDug = true;
	for (size_t i = 2; i < m_horizontalSubCells.size() - 2; ++i)
	{
		if (!m_horizontalSubCells[i])
		{
			centerFourHorizontalDug = false;
			break;
		}
	}
	bool firstTwoHorizontalDug = m_horizontalSubCells[0] && m_horizontalSubCells[1];
	bool lastTwoHorizontalDug = m_horizontalSubCells[m_horizontalSubCells.size() - 1] && m_horizontalSubCells[m_horizontalSubCells.size() - 2];
	bool centerFourVerticalDug = true;
	for (size_t i = 2; i < m_verticalSubCells.size() - 2; ++i)
	{
		if (!m_verticalSubCells[i])
		{
			centerFourVerticalDug = false;
			break;
		}
	}
	bool firstTwoVerticalDug = m_verticalSubCells[0] && m_verticalSubCells[1];
	bool lastTwoVerticalDug = m_verticalSubCells[m_verticalSubCells.size() - 1] && m_verticalSubCells[m_verticalSubCells.size() - 2];

	if ((centerFourHorizontalDug && (firstTwoHorizontalDug ^ lastTwoHorizontalDug)) && centerFourVerticalDug && (firstTwoVerticalDug ^ lastTwoVerticalDug))
	{
		DigCellCenter();
	}

	// Dig out perpendicular subcells to connect future tunnels better
	bool centerHorizontalDug = true;
	for (size_t i = 1; i < m_horizontalSubCells.size() - 1; ++i)
	{
		if (!m_horizontalSubCells[i])
		{
			centerHorizontalDug = false;
			break;
		}
	}
	if (centerHorizontalDug)
	{
		for (size_t i = 1; i < m_verticalSubCells.size() - 1; ++i)
		{
			m_verticalSubCells[i] = true;
		}
	}
	bool centerVerticalDug = true;
	for (size_t i = 1; i < m_verticalSubCells.size() - 1; ++i)
	{
		if (!m_verticalSubCells[i])
		{
			centerVerticalDug = false;
			break;
		}
	}
	if (centerVerticalDug)
	{
		for (size_t i = 1; i < m_horizontalSubCells.size() - 1; ++i)
		{
			m_horizontalSubCells[i] = true;
		}
	}
}
