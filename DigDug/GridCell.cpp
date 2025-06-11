#include "GridCell.h"
#include "SDL.h"
#include "Renderer.h"
#include "GameObject.h"

GridCell::GridCell(dae::GameObject* owner)
	: dae::BaseComponent(owner), m_horizontalSubCells(), m_verticalSubCells()
{
}

GridCell::GridCell(dae::GameObject* owner, std::array<bool, 8> verticalCells, std::array<bool, 8> horiontalCells)
	: dae::BaseComponent(owner), m_verticalSubCells(verticalCells), m_horizontalSubCells(horiontalCells)
{
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
	int scale{ 3 };
	int cellSize{ 16 * scale };
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();

	SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);

	const auto& pos = GetOwner()->GetTransform()->GetWorldPosition();
	SDL_Rect horizontalRectUp{ static_cast<int>(pos.x), static_cast<int>(pos.y) + 2 * scale, cellSize / 16, cellSize -3 * scale};
	SDL_Rect horizontalRectDown{ static_cast<int>(pos.x), static_cast<int>(pos.y) + 1 * scale, cellSize / 16, cellSize - 3 * scale };
	for (int i{}; i < m_horizontalSubCells.size(); ++i)
	{
		if (m_horizontalSubCells[i])
		{
			if (i % 2 == 0)
			{
				horizontalRectUp.x = static_cast<int>(pos.x) + cellSize / 16 * (i * 2);
				horizontalRectDown.x = static_cast<int>(pos.x) + cellSize / 16 * (i * 2 + 1);
			}
			else
			{
				horizontalRectUp.x = static_cast<int>(pos.x) + cellSize / 16 * (i * 2 + 1);
				horizontalRectDown.x = static_cast<int>(pos.x) + cellSize / 16 * (i * 2);
			}
			SDL_RenderFillRect(sdlRenderer, &horizontalRectUp);
			SDL_RenderFillRect(sdlRenderer, &horizontalRectDown);
		}
	}
	SDL_Rect verticalRectleft{ static_cast<int>(pos.x) + 1 * scale, static_cast<int>(pos.y), cellSize - 3 * scale, cellSize / 16 };
	SDL_Rect verticalRectright{ static_cast<int>(pos.x) + 2 * scale, static_cast<int>(pos.y), cellSize - 3 * scale, cellSize / 16 };
	for (int i{}; i < m_verticalSubCells.size(); ++i)
	{
		if (m_verticalSubCells[i])
		{
			if (i % 2 == 0)
			{
				verticalRectleft.y = static_cast<int>(pos.y) + cellSize / 16 * (i * 2);
				verticalRectright.y = static_cast<int>(pos.y) + cellSize / 16 * (i * 2 + 1);
			}
			else
			{
				verticalRectleft.y = static_cast<int>(pos.y) + cellSize / 16 * (i * 2 + 1);
				verticalRectright.y = static_cast<int>(pos.y) + cellSize / 16 * (i * 2);
			}
			SDL_RenderFillRect(sdlRenderer, &verticalRectleft);
			SDL_RenderFillRect(sdlRenderer, &verticalRectright);
		}
	}
	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);
	SDL_Rect cellRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), 48, 48 };
	SDL_RenderDrawRect(sdlRenderer, &cellRect);
}