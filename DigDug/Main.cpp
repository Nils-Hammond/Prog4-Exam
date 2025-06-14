#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"

#include <SDL.h>
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextRenderComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "FPSComponent.h"
#include <iostream>
#include "RotateAround.h"
#include "SelfDestruct.h"
#include "Command.h"
#include "GameCommands.h"
#include "InputManager.h"
#include "Gamepad.h"
#include "MoveComponent.h"
#include "LivesAndPoints.h"
#include "LevelLoader.h"
#include "MoveCharacterCommand.h"
#include "SpriteRenderComponent.h"
#include "GridCell.h"
#include "ColliderComponent.h"
#include "MainMenu.h"
#include "GameConstants.h"

template <typename T>
void CheckComponent(const std::string& templateName, const dae::GameObject& object)
{
	if (object->HasComponent<T>())
	{
		std::cout << "Object has a " << templateName << " component" << std::endl;
	}
	else
	{
		std::cout << "Object does not have a " << templateName << " component" << std::endl;
	}
}

void LoadLevel(const std::string& levelFile, dae::Scene& scene)
{
	auto levelLoader = std::make_shared<dae::GameObject>();
	levelLoader->AddComponent(std::make_unique<LevelLoader>(levelLoader.get(), &scene));
	scene.Add(levelLoader);
	levelLoader->GetComponent<LevelLoader>()->LoadLevel(levelFile);
}

void LoadMenu(dae::Scene& scene)
{
	auto menu = std::make_shared<dae::GameObject>();
	dae::GameObject* menuPointer = menu.get();
	float menuScale = 2.6f;
	menu->AddComponent(std::make_unique<dae::RenderComponent>(menu.get(), "MainMenu/Background.png", menuScale));
	menu->AddComponent(std::make_unique<MainMenu>(menu.get()));
	scene.Add(menu);

	auto marker = std::make_shared<dae::GameObject>();
	marker->SetParent(menuPointer);
	marker->SetRenderLayer(1);
	marker->GetTransform()->SetLocalPosition(210.f, 265.f);
	marker->AddComponent(std::make_unique<dae::RenderComponent>(marker.get(), "MainMenu/Marker.png", menuScale));
	scene.Add(marker);
}


void static load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene(std::string(SCENE_NAME));

	LoadMenu(scene);

	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.BindKeyboardCommand(SDLK_F2, dae::InputManager::ButtonState::Down, std::make_unique<MuteAudioCommand>());
}

int main(int, char* []) {
	dae::Minigin engine("../Data/", 720, 624, "DigDug");
	engine.Run(load);
	return 0;
}