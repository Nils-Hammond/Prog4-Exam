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

LevelLoader::LevelLoader(dae::GameObject* owner, dae::Scene* scene)
	: BaseComponent(owner), m_currentScene(scene)
{
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
				SpawnHole(x, y);
				break;
			case 'P':
				SpawnPlayer1(x, y);
				break;
			case 'p':
				SpawnPooka(x, y);
				break;
			case 'f':
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
}

void LevelLoader::SpawnPooka(int x, int y)
{
	SpawnHole(x, y);
	auto pooka = std::make_shared<dae::GameObject>();
	pooka->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	pooka->SetRenderLayer(2);
	pooka->AddComponent(std::make_unique<dae::SpriteRenderComponent>(pooka.get(), "Pooka/Default.png", 1, 2, 3.f));
	pooka->AddComponent(std::make_unique<MoveComponent>(pooka.get()));
	m_currentScene->Add(pooka);
}

void LevelLoader::SpawnFygar(int x, int y)
{
	SpawnHole(x, y);
	auto fygar = std::make_shared<dae::GameObject>();
	fygar->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	fygar->SetRenderLayer(2);
	fygar->AddComponent(std::make_unique<dae::SpriteRenderComponent>(fygar.get(), "Fygar/Default.png", 1, 2, 3.f));
	fygar->AddComponent(std::make_unique<MoveComponent>(fygar.get()));
	m_currentScene->Add(fygar);
}

static void LoadPlayer1(dae::Scene& scene, int x, int y)
{
	auto player1 = std::make_shared<dae::GameObject>();
	player1->SetRenderLayer(2);
	player1->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	player1->AddComponent(std::make_unique<dae::SpriteRenderComponent>(player1.get(), "DigDug0/Walking.png", 1, 2, 3.f));
	player1->AddComponent(std::make_unique<MoveComponent>(player1.get()));
	auto healthComponent = std::make_unique<HealthComponent>(player1.get(), 5);
	player1->AddComponent(std::move(healthComponent));
	scene.Add(player1);

	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.BindKeyboardCommand(SDLK_s, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(0.f, 1.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_w, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(0.f, -1.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_a, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(-1.f, 0.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_d, dae::InputManager::ButtonState::Pressed, std::make_unique<MoveCharacterCommand>(player1.get(), glm::vec3(1.f, 0.f, 0.f), 100.f));
	inputManager.BindKeyboardCommand(SDLK_c, dae::InputManager::ButtonState::Down, std::make_unique<DieCommand>(player1.get()));
}

void LevelLoader::SpawnPlayer1(int x, int y)
{
	SpawnHole(x, y);
	/*
	auto player1 = std::make_shared<dae::GameObject>();
	player1->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	player1->SetRenderLayer(2);
	player1->AddComponent(std::make_unique<dae::RenderComponent>(player1.get(), "DigDug0/Walking.png", 3.f, 3.f));
	player1->AddComponent(std::make_unique<MoveComponent>(player1.get()));
	m_currentScene->Add(player1);
	*/
	LoadPlayer1(*m_currentScene, x, y);
}

void LevelLoader::SpawnPlayer2(int x, int y)
{
	SpawnHole(x, y);
	auto player2 = std::make_shared<dae::GameObject>();
	player2->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	player2->SetRenderLayer(2);
	player2->AddComponent(std::make_unique<dae::SpriteRenderComponent>(player2.get(), "DigDug1/Walking.png", 1, 2, 3.f));
	player2->AddComponent(std::make_unique<MoveComponent>(player2.get()));
	m_currentScene->Add(player2);
}

void LevelLoader::SpawnRock(int x, int y)
{
	auto rock = std::make_shared<dae::GameObject>();
	rock->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	rock->SetRenderLayer(2);
	rock->AddComponent(std::make_unique<dae::SpriteRenderComponent>(rock.get(), "Rock/Default.png", 1, 1, 3.f));
	m_currentScene->Add(rock);
}

void LevelLoader::SpawnHole(int x, int y)
{
	auto hole = std::make_shared<dae::GameObject>();
	hole->GetTransform()->SetLocalPosition(static_cast<float>(x * 48), static_cast<float>(y * 48), 0.f);
	hole->SetRenderLayer(1);
	hole->AddComponent(std::make_unique<dae::RenderComponent>(hole.get(), "DiggedArea.png", 3.f, 3.f));
	m_currentScene->Add(hole);
}
