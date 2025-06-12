#include "LevelLoader.h"
#include <fstream>
#include <iostream>
#include "GameObject.h"
#include "RenderComponent.h"
#include "MoveComponent.h"
#include "SpriteRenderComponent.h"
#include "LivesAndPoints.h"
#include "MoveCharacterCommand.h"
#include "SDL.h"
#include "InputManager.h"
#include "SubjectObserver.h"
#include <algorithm>
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "PookaComponent.h"
#include "ServiceLocator.h"
#include "PumpComponent.h"
#include "GameConstants.h"
#include "GridCell.h"

LevelLoader::LevelLoader(dae::GameObject* owner, dae::Scene* scene)
	: BaseComponent(owner), m_currentScene(scene), m_gridCells()
{
}

LevelLoader::~LevelLoader() = default;

static bool IsVerticalHole(const std::string& line, int x, int y)
{
	char leftSide{}, rightSide{};
	if (x <= 0 || static_cast<unsigned>(x) >= line.size() - 1)
	{
		leftSide = 'd';
	}
	else
	{
		leftSide = line[x - 1];
	}
	if (y <= 0 || static_cast<unsigned>(y) >= line.size() - 1)
	{
		rightSide = 'd';
	}
	else
	{
		rightSide = line[x + 1];
	}
	if ((leftSide == 'd' || leftSide == 'r') && (rightSide == 'd' || rightSide == 'r'))
	{
		return true;
	}
	return false;
}

void LevelLoader::LoadLevel(const std::string& levelFile)
{
	std::ifstream file(levelFile);
	if (!file.is_open())
	{
		std::cerr << "Failed to open level file: " << levelFile << std::endl;
		return;
	}
	std::string data;

	// Initialize grid cells
	for (int i = 0; i < LEVEL_WIDTH * LEVEL_HEIGHT; ++i)
	{
		SpawnDiggableCell(i % LEVEL_WIDTH, i / LEVEL_WIDTH);
	}

	int y = 0;
	while (std::getline(file, data))
	{
		int x = 0;
		for (char& c : data)
		{
			//std::cout << c << " ";
			
			switch (c)
			{
			case '#':
				SpawnHole(data, x, y);
				break;
			case '1':
				SpawnHole(data, x, y);
				SpawnPlayer1(x, y);
				break;
			case '2':
				SpawnHole(data, x, y);
				SpawnPlayer2(x, y);
				break;
			case 'p':
				SpawnHole(data, x, y);
				SpawnPooka(x, y);
				break;
			case 'f':
				SpawnHole(data, x, y);
				SpawnFygar(x, y);
				break;
			case 'r':
				SpawnRock(x, y);
				break;
			}
			
			++x;
		}
		++y;
	}
	file.close();
	auto bg = std::make_shared<dae::GameObject>();
	bg->GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
	bg->SetRenderLayer(0);
	bg->AddComponent(std::make_unique<dae::RenderComponent>(bg.get(), "Tiles/Background.png", 3.f, 3.f));
	m_currentScene->Add(bg);
	
	MakeEnemiesAwareOfPlayer();
}

void LevelLoader::SpawnPooka(int x, int y)
{
	auto pooka = std::make_shared<dae::GameObject>();
	m_pEntities.emplace_back(pooka.get());
	pooka->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	pooka->SetRenderLayer(4);
	pooka->AddComponent(std::make_unique<dae::SpriteRenderComponent>(pooka.get(), "Pooka/Default.png", 1, 2, 3.f));
	pooka->AddComponent(std::make_unique<dae::ColliderComponent>(pooka.get(), dae::make_sdbm_hash("Enemy")));
	pooka->GetComponent<dae::ColliderComponent>()->ResizeColliderRect(GRID_SIZE - 12, GRID_SIZE - 12);
	pooka->GetComponent<dae::ColliderComponent>()->OffsetColliderRect(6, 6);
	pooka->AddComponent(std::make_unique<MoveComponent>(pooka.get(), m_gridCells));
	pooka->AddComponent(std::make_unique<PookaComponent>(pooka.get()));
	m_currentScene->Add(pooka);
}

void LevelLoader::SpawnFygar(int x, int y)
{
	auto fygar = std::make_shared<dae::GameObject>();
	m_pEntities.emplace_back(fygar.get());
	fygar->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	fygar->SetRenderLayer(4);
	fygar->AddComponent(std::make_unique<dae::SpriteRenderComponent>(fygar.get(), "Fygar/Default.png", 1, 2, 3.f));
	fygar->AddComponent(std::make_unique<dae::ColliderComponent>(fygar.get(), dae::make_sdbm_hash("Enemy")));
	fygar->GetComponent<dae::ColliderComponent>()->ResizeColliderRect(GRID_SIZE - 12, GRID_SIZE - 12);
	fygar->GetComponent<dae::ColliderComponent>()->OffsetColliderRect(6, 6);
	fygar->AddComponent(std::make_unique<MoveComponent>(fygar.get(), m_gridCells));
	m_currentScene->Add(fygar);
}

void LevelLoader::SpawnPlayer1(int x, int y)
{
	//dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/walkmusic.wav", 128, true, 0);
	auto player1 = std::make_shared<dae::GameObject>();
	m_pEntities.emplace_back(player1.get());
	player1->SetRenderLayer(4);
	player1->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	player1->AddComponent(std::make_unique<dae::SpriteRenderComponent>(player1.get(), "DigDug0/Walking.png", 1, 2, 3.f));
	player1->AddComponent(std::make_unique<dae::ColliderComponent>(player1.get(), dae::make_sdbm_hash("Player")));
	player1->GetComponent<dae::ColliderComponent>()->ResizeColliderRect(GRID_SIZE - 14, GRID_SIZE - 14);
	player1->GetComponent<dae::ColliderComponent>()->OffsetColliderRect(7, 7);
	player1->AddComponent(std::make_unique<MoveComponent>(player1.get(), m_gridCells, true));
	auto healthComponent = std::make_unique<HealthComponent>(player1.get(), 5);
	player1->AddComponent(std::move(healthComponent));
	player1->AddComponent(std::make_unique<PlayerComponent>(player1.get()));

	auto pumpObject = std::make_shared<dae::GameObject>();
	pumpObject->SetRenderLayer(3);
	pumpObject->SetParent(player1.get(), false);
	pumpObject->AddComponent(std::make_unique<dae::ColliderComponent>(pumpObject.get(), dae::make_sdbm_hash("Pump")));
	pumpObject->AddComponent(std::make_unique<PumpComponent>(pumpObject.get(), "Pump.png", 3.f));
	m_currentScene->Add(pumpObject);

	m_currentScene->Add(player1);

	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.BindKeyboardCommand(SDLK_s, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(0.f, 1.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_w, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(0.f, -1.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_a, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(-1.f, 0.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_d, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(1.f, 0.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_c, dae::InputManager::ButtonState::Down, std::make_unique<DieCommand>(player1.get()));
	inputManager.BindKeyboardCommand(SDLK_SPACE, dae::InputManager::ButtonState::Down, std::make_unique<AttackCommand>(player1.get()));
}

void LevelLoader::SpawnPlayer2(int x, int y)
{
	auto player2 = std::make_shared<dae::GameObject>();
	m_pEntities.emplace_back(player2.get());
	player2->SetRenderLayer(4);
	player2->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	player2->AddComponent(std::make_unique<dae::SpriteRenderComponent>(player2.get(), "DigDug1/Walking.png", 1, 2, 3.f));
	player2->AddComponent(std::make_unique<dae::ColliderComponent>(player2.get(), dae::make_sdbm_hash("Player2")));
	player2->GetComponent<dae::ColliderComponent>()->ResizeColliderRect(GRID_SIZE - 14, GRID_SIZE - 14);
	player2->GetComponent<dae::ColliderComponent>()->OffsetColliderRect(7, 7);
	player2->AddComponent(std::make_unique<MoveComponent>(player2.get(), m_gridCells));
	auto healthComponent = std::make_unique<HealthComponent>(player2.get(), 5);
	player2->AddComponent(std::move(healthComponent));
	player2->AddComponent(std::make_unique<PlayerComponent>(player2.get(), 1));

	auto pumpObject = std::make_shared<dae::GameObject>();
	pumpObject->SetRenderLayer(3);
	pumpObject->SetParent(player2.get(), false);
	pumpObject->AddComponent(std::make_unique<dae::ColliderComponent>(pumpObject.get(), dae::make_sdbm_hash("Pump")));
	pumpObject->AddComponent(std::make_unique<PumpComponent>(pumpObject.get(), "Pump.png", 3.f));
	m_currentScene->Add(pumpObject);

	m_currentScene->Add(player2);

	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.BindKeyboardCommand(SDLK_UP, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player2.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_DOWN, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player2.get(), glm::vec3(0.f, 1.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_LEFT, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player2.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_RIGHT, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player2.get(), glm::vec3(1.f, 0.f, 0.f)));
}

void LevelLoader::SpawnRock(int x, int y)
{
	auto rock = std::make_shared<dae::GameObject>();
	rock->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	rock->SetRenderLayer(2);
	rock->AddComponent(std::make_unique<dae::SpriteRenderComponent>(rock.get(), "Rock/Default.png", 1, 1, 3.f));
	rock->AddComponent(std::make_unique<dae::ColliderComponent>(rock.get(), dae::make_sdbm_hash("Rock")));
	rock->GetComponent<dae::ColliderComponent>()->ResizeColliderRect(48, 48);
	m_currentScene->Add(rock);
}

void LevelLoader::SpawnHole(const std::string& line, int x, int y)
{
	/*
	auto hole = std::make_shared<dae::GameObject>();
	hole->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	hole->SetRenderLayer(1);
	hole->AddComponent(std::make_unique<dae::RenderComponent>(hole.get(), "DiggedArea.png", 3.f, 3.f));
	if (IsVerticalHole(line, x, y))
		hole->GetComponent<dae::RenderComponent>()->SetAngle(90.f);
	m_currentScene->Add(hole);
	*/
	if (IsVerticalHole(line, x, y))
	{
		m_gridCells[x + y * LEVEL_WIDTH]->CreateTunnel(false);
	}
	else
	{
		m_gridCells[x + y * LEVEL_WIDTH]->CreateTunnel(true);
	}
}

void LevelLoader::SpawnDiggableCell(int x, int y)
{
	auto cell = std::make_shared<dae::GameObject>();
	cell->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), 0.f);
	cell->SetRenderLayer(1);
	cell->AddComponent(std::make_unique<dae::ColliderComponent>(cell.get(), dae::make_sdbm_hash("Dirt")));
	cell->AddComponent(std::make_unique<GridCell>(cell.get()));
	m_gridCells[x + y * LEVEL_WIDTH] = cell->GetComponent<GridCell>();
	m_currentScene->Add(cell);
}

void LevelLoader::MakeEnemiesAwareOfPlayer()
{
	std::vector<const dae::GameObject*> players;
	for (const auto& entity : m_pEntities)
	{
		if (entity->GetComponent<PlayerComponent>())
		{
			players.emplace_back(entity);
		}
	}
	for (const auto& entity : m_pEntities)
	{
		if (entity->GetComponent<PookaComponent>())
		{
			auto pookaComponent = entity->GetComponent<PookaComponent>();
			pookaComponent->SetPlayers(players);
		}
		//else if (entity->GetComponent<FygarComponent>())
		//{
		//	auto fygarComponent = entity->GetComponent<FygarComponent>();
		//	fygarComponent->SetPlayers(players);
		//}
	}
}
