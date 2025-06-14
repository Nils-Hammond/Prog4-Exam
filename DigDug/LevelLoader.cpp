#include "LevelLoader.h"
#include <fstream>
#include <iostream>
#include <filesystem>
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
#include "FygarComponent.h"
#include "ServiceLocator.h"
#include "PumpComponent.h"
#include "GameConstants.h"
#include "GridCell.h"
#include "ResourceManager.h"
#include "TextRenderComponent.h"
#include "DaeTime.h"
#include <regex>

LevelLoader::LevelLoader(dae::GameObject* owner, dae::Scene* scene, bool versus)
	: BaseComponent(owner)
	, m_currentScene(scene)
	, m_gridCells()
	, m_versus(versus)
	, m_coop(false)
	, m_startResetTimer(false)
	, m_startNextTimer(false)
	, m_firstLoad(false)
	, m_levelNumber()
	, m_pEnemies()
	, m_pUIObjects()
	, m_pPlayer1()
	, m_pPlayer2()
	, m_timer()
	, m_levelFolderPath()
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

void LevelLoader::Update()
{
	if (m_startResetTimer || m_startNextTimer)
	{
		m_timer += dae::Time::GetInstance().GetDeltaTime();
	}
	if (m_timer >= RESET_TIMER)
	{
		if (m_startResetTimer)
		{
			ResetEntities();
			m_startResetTimer = false;
		}
		else if (m_startNextTimer)
		{
			LoadNextLevel();
			m_startNextTimer = false;
		}
		m_timer = 0.f;
	}
	std::erase_if(m_pEnemies, [](auto* enemy) {
		return enemy == nullptr || enemy->IsToBeDestroyed();});

	if (m_levelNumber < LEVEL_COUNT && m_pEnemies.size() == 0)
	{
		if (!m_startNextTimer)
			dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/Victory.wav", 128, false, -1);
		m_startNextTimer = true;
	}
}

void LevelLoader::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("HealthChanged"))
	{
		try
		{
			int newHealth = std::any_cast<int>(event.data);
			if (newHealth >= 0)
			{
				m_startResetTimer = true;
			}
			else
			{
				for (const auto& enemy : m_pEnemies)
				{
					enemy->Destroy();
				}
				std::cout << "Game Over!\n";
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting event data: " << e.what() << std::endl;
			return;
		}
	}
}

void LevelLoader::LoadLevel(const std::string& levelFile)
{
	GetLevelInfo(levelFile);

	CreateLivesDisplay(LEVEL_WIDTH * GRID_SIZE, 4 * GRID_SIZE);
	if (m_coop)
		CreateLivesDisplay(LEVEL_WIDTH * GRID_SIZE, 8 * GRID_SIZE);
	else
		m_pUIObjects.emplace_back(nullptr);
	CreateScoreDisplay(LEVEL_WIDTH * GRID_SIZE + 10, 2 * GRID_SIZE);

	SpawnLevelObjects(levelFile);
	auto bg = std::make_shared<dae::GameObject>();
	bg->GetTransform()->SetLocalPosition(0.f, 0.f, 0.f);
	bg->SetRenderLayer(0);
	bg->AddComponent(std::make_unique<dae::RenderComponent>(bg.get(), "Tiles/Background.png", 3.f, 3.f));
	m_currentScene->Add(bg);

	LinkGameObjects();
}

void LevelLoader::LoadNextLevel()
{
	int playerCount{ m_coop ? 2 : 1 };
	++m_levelNumber;
	std::string nextFileName{m_levelFolderPath + "/Level" + std::to_string(m_levelNumber) + "_" + std::to_string(playerCount) + ".txt"};
	ClearLevel();
	SpawnLevelObjects(nextFileName);
	LinkGameObjects();
}

void LevelLoader::SpawnLevelObjects(const std::string& levelFile)
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
}

void LevelLoader::ClearLevel()
{
	for (auto enemy : m_pEnemies)
	{
		enemy->Destroy();
	}
	m_pEnemies.clear();
	for (auto object : m_pNonPersistentObjects)
	{
		object->Destroy();
	}
	m_pNonPersistentObjects.clear();
}

void LevelLoader::SpawnPooka(int x, int y)
{
	auto pooka = std::make_shared<dae::GameObject>();
	m_pEnemies.emplace_back(pooka.get());
	pooka->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), 0.f);
	pooka->SetRenderLayer(3);
	pooka->AddComponent(std::make_unique<dae::SpriteRenderComponent>(pooka.get(), "Pooka/Default.png", 1, 2, static_cast<float>(SPRITE_SCALE)));
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
	m_pEnemies.emplace_back(fygar.get());
	fygar->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), 0.f);
	fygar->SetRenderLayer(3);
	fygar->AddComponent(std::make_unique<dae::SpriteRenderComponent>(fygar.get(), "Fygar/Default.png", 1, 2, static_cast<float>(SPRITE_SCALE)));
	fygar->AddComponent(std::make_unique<dae::ColliderComponent>(fygar.get(), dae::make_sdbm_hash("Enemy")));
	fygar->GetComponent<dae::ColliderComponent>()->ResizeColliderRect(GRID_SIZE - 12, GRID_SIZE - 12);
	fygar->GetComponent<dae::ColliderComponent>()->OffsetColliderRect(6, 6);
	fygar->AddComponent(std::make_unique<MoveComponent>(fygar.get(), m_gridCells));
	fygar->AddComponent(std::make_unique<FygarComponent>(fygar.get(), 80.f));
	m_currentScene->Add(fygar);
}

void LevelLoader::SpawnPlayer1(int x, int y)
{
	//dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/walkmusic.wav", 128, true, 0);
	if (m_pPlayer1)
	{
		m_pPlayer1->GetOwner()->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE));
		m_pPlayer1->UpdateRespawn();
		return;
	}
	auto player1 = std::make_shared<dae::GameObject>();
	player1->SetRenderLayer(3);
	player1->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), 0.f);
	player1->AddComponent(std::make_unique<dae::SpriteRenderComponent>(player1.get(), "DigDug0/Walking.png", 1, 2, static_cast<float>(SPRITE_SCALE)));
	player1->AddComponent(std::make_unique<dae::ColliderComponent>(player1.get(), dae::make_sdbm_hash("Player")));
	player1->GetComponent<dae::ColliderComponent>()->ResizeColliderRect(GRID_SIZE - 14, GRID_SIZE - 14);
	player1->GetComponent<dae::ColliderComponent>()->OffsetColliderRect(7, 7);
	player1->AddComponent(std::make_unique<MoveComponent>(player1.get(), m_gridCells, true, true));
	player1->AddComponent(std::make_unique<HealthComponent>(player1.get(), 3));
	player1->GetComponent<HealthComponent>()->RegisterObserver(this);
	player1->AddComponent(std::make_unique<PlayerComponent>(player1.get()));
	m_pPlayer1 = player1->GetComponent<PlayerComponent>();

	auto* livesDisplay = m_pUIObjects[P1_HEALTH_DISPLAY_INDEX]->GetComponent<LivesDisplayComponent>();
	player1->GetComponent<HealthComponent>()->RegisterObserver(livesDisplay);

	auto pumpObject = std::make_shared<dae::GameObject>();
	pumpObject->SetRenderLayer(2);
	pumpObject->SetParent(player1.get(), false);
	pumpObject->AddComponent(std::make_unique<dae::ColliderComponent>(pumpObject.get(), dae::make_sdbm_hash("Pump")));
	pumpObject->AddComponent(std::make_unique<PumpComponent>(pumpObject.get(), "Pump.png", static_cast<float>(SPRITE_SCALE)));
	m_currentScene->Add(pumpObject);

	m_currentScene->Add(player1);

	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.BindKeyboardCommand(SDLK_s, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(0.f, 1.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_w, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(0.f, -1.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_a, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(-1.f, 0.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_d, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(1.f, 0.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_SPACE, dae::InputManager::ButtonState::Down, std::make_unique<AttackCommand>(player1.get()));
}

void LevelLoader::SpawnPlayer2(int x, int y)
{
	if (m_pPlayer2)
	{
		m_pPlayer2->GetOwner()->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE));
		m_pPlayer2->UpdateRespawn();
		return;
	}
	auto player2 = std::make_shared<dae::GameObject>();
	player2->SetRenderLayer(3);
	player2->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), 0.f);
	player2->AddComponent(std::make_unique<dae::SpriteRenderComponent>(player2.get(), "DigDug1/Walking.png", 1, 2, static_cast<float>(SPRITE_SCALE)));
	player2->AddComponent(std::make_unique<dae::ColliderComponent>(player2.get(), dae::make_sdbm_hash("Player2")));
	player2->GetComponent<dae::ColliderComponent>()->ResizeColliderRect(GRID_SIZE - 14, GRID_SIZE - 14);
	player2->GetComponent<dae::ColliderComponent>()->OffsetColliderRect(7, 7);
	player2->AddComponent(std::make_unique<MoveComponent>(player2.get(), m_gridCells, true, true));
	player2->AddComponent(std::make_unique<HealthComponent>(player2.get(), 3));
	player2->GetComponent<HealthComponent>()->RegisterObserver(this);
	player2->AddComponent(std::make_unique<PlayerComponent>(player2.get(), 1));
	m_pPlayer1 = player2->GetComponent<PlayerComponent>();

	auto* livesDisplay = m_pUIObjects[P2_HEALTH_DISPLAY_INDEX]->GetComponent<LivesDisplayComponent>();
	player2->GetComponent<HealthComponent>()->RegisterObserver(livesDisplay);

	auto pumpObject = std::make_shared<dae::GameObject>();
	pumpObject->SetRenderLayer(2);
	pumpObject->SetParent(player2.get(), false);
	pumpObject->AddComponent(std::make_unique<dae::ColliderComponent>(pumpObject.get(), dae::make_sdbm_hash("Pump")));
	pumpObject->AddComponent(std::make_unique<PumpComponent>(pumpObject.get(), "Pump.png", static_cast<float>(SPRITE_SCALE)));
	m_currentScene->Add(pumpObject);

	m_currentScene->Add(player2);

	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadDown, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player2.get(), glm::vec3(0.f, 1.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadUp, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player2.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadLeft, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player2.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadRight, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player2.get(), glm::vec3(1.f, 0.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::X, dae::InputManager::ButtonState::Down, std::make_unique<DieCommand>(player2.get()));
}

void LevelLoader::SpawnRock(int x, int y)
{
	auto rock = std::make_shared<dae::GameObject>();
	m_pNonPersistentObjects.emplace_back(rock.get());
	rock->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), 0.f);
	rock->SetRenderLayer(4);
	rock->AddComponent(std::make_unique<dae::SpriteRenderComponent>(rock.get(), "Rock/Default.png", 1, 1, static_cast<float>(SPRITE_SCALE)));
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
	m_pNonPersistentObjects.emplace_back(cell.get());
	cell->GetTransform()->SetLocalPosition(static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), 0.f);
	cell->SetRenderLayer(1);
	cell->AddComponent(std::make_unique<dae::ColliderComponent>(cell.get(), dae::make_sdbm_hash("Dirt")));
	cell->AddComponent(std::make_unique<GridCell>(cell.get()));
	m_gridCells[x + y * LEVEL_WIDTH] = cell->GetComponent<GridCell>();
	m_currentScene->Add(cell);
}

void LevelLoader::LinkGameObjects()
{
	auto* scoreDisplay = m_pUIObjects[SCORE_DISPLAY_INDEX]->GetComponent<ScoreDisplayComponent>();

	std::vector<const dae::GameObject*> players;
	players.emplace_back(m_pPlayer1->GetOwner());
	if (m_pPlayer2)
		players.emplace_back(m_pPlayer2->GetOwner());

	for (const auto& enemy : m_pEnemies)
	{
		if (auto enemyComponent = enemy->GetComponent<EnemyComponent>())
		{
			enemyComponent->SetPlayers(players);
			enemyComponent->RegisterObserver(scoreDisplay);
		}
	}
}

void LevelLoader::CreateScoreDisplay(float posX, float posY)
{
	auto redFont = dae::ResourceManager::GetInstance().LoadFont("../Data/Fonts/Arcade.ttf", 20);
	auto ScoreNameText = std::make_shared<dae::GameObject>();
	ScoreNameText->GetTransform()->SetLocalPosition(posX, posY);
	ScoreNameText->SetParent(GetOwner());
	ScoreNameText->SetRenderLayer(1);
	ScoreNameText->AddComponent(std::make_unique<dae::TextRenderComponent>(ScoreNameText.get(), "Score", redFont, SDL_Color(255, 0, 0, 255)));

	auto scoreFont = dae::ResourceManager::GetInstance().LoadFont("../Data/Fonts/Arcade.ttf", 20);
	auto ScoreDisplay = std::make_shared<dae::GameObject>();
	ScoreDisplay->SetParent(ScoreNameText.get());
	ScoreDisplay->GetTransform()->SetLocalPosition(30, 30);
	ScoreDisplay->SetRenderLayer(1);
	ScoreDisplay->AddComponent(std::make_unique<dae::TextRenderComponent>(ScoreDisplay.get(), "0", scoreFont));
	ScoreDisplay->AddComponent(std::make_unique<ScoreDisplayComponent>(ScoreDisplay.get()));

	m_currentScene->Add(ScoreNameText);
	m_currentScene->Add(ScoreDisplay);

	m_pUIObjects.emplace_back(ScoreDisplay.get());
}

LivesDisplayComponent* LevelLoader::CreateLivesDisplay(float posX, float posY)
{
	auto lifeDisplay = std::make_shared<dae::GameObject>();
	lifeDisplay->GetTransform()->SetLocalPosition(posX, posY);
	lifeDisplay->SetParent(GetOwner());

	const int startingLives{ 3 };
	for (int count{}; count < startingLives; ++count)
	{
		auto life = std::make_shared<dae::GameObject>();
		life->SetParent(lifeDisplay.get());
		const float xSpacing{ GRID_SIZE };
		life->GetTransform()->SetLocalPosition(count * xSpacing, 30);
		life->AddComponent(std::make_unique<dae::RenderComponent>(life.get(), "HUD/Life.png", static_cast<float>(SPRITE_SCALE)));
		m_currentScene->Add(life);
	}

	lifeDisplay->AddComponent(std::make_unique<LivesDisplayComponent>(lifeDisplay.get(), startingLives));
	auto displayPointer = lifeDisplay->GetComponent<LivesDisplayComponent>();
	m_currentScene->Add(lifeDisplay);

	m_pUIObjects.emplace_back(lifeDisplay.get());

	return displayPointer;
}

void LevelLoader::ResetEntities()
{
	m_pPlayer1->Reset();
	if (m_pPlayer2) m_pPlayer2->Reset();
	for (const auto& enemy : m_pEnemies)
	{
		if (auto enemyComponent = enemy->GetComponent<EnemyComponent>())
		{
			enemyComponent->Reset();
		}
	}
}

void LevelLoader::GetLevelInfo(const std::string& filename)
{
	std::filesystem::path pathObj(filename);
	m_levelFolderPath = pathObj.parent_path().string();

	std::regex pattern(R"(Level(\d+)_(\d+))");
	std::smatch match;
	if (std::regex_search(filename, match, pattern) && match.size() == 3) {
		m_levelNumber = std::stoi(match[1]);
		int playerCount = std::stoi(match[2]);
		if (playerCount == 2)
			m_coop = true;
	}
}
