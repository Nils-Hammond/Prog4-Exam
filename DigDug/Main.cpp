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
#include "InputManager.h"
#include "Gamepad.h"
#include "MoveComponent.h"
#include "LivesAndPoints.h"

void LoadPlayer1(dae::Scene& scene);
void LoadPlayer2(dae::Scene& scene);

template <typename T>
void CheckComponent(const std::string& templateName, dae::GameObject* object)
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

// I should really split up this function into smaller functions
void static load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	//Background Image
	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "background.tga"));
	scene.Add(go);

	//Logo
	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "logo.tga"));
	go->GetTransform()->SetLocalPosition(280.f, 140.f);
	scene.Add(go);

	//Title Text
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 28);
	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "Programming 4 Assignment", font));
	go->GetTransform()->SetLocalPosition(200.f, 20.f);
	scene.Add(go);

	//FPS Counter
	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "0.0 FPS", font));
	go->AddComponent(std::make_unique<FPSComponent>(go.get()));
	go->GetTransform()->SetLocalPosition(20.f, 20.f);
	scene.Add(go);

	//Controls Text
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);
	go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetLocalPosition(20.f, 80.f, 0.f);
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "Use the D-Pad to move Pooka, X to inflict damage, A and B to kill enemies", font));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetLocalPosition(20.f, 100.f, 0.f);
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "Use WASD to move DigDug, C to inflict damage, Z and X to kill enemies", font));
	scene.Add(go);

	LoadPlayer1(scene);
	LoadPlayer2(scene);
}

void LoadPlayer1(dae::Scene& scene)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	//P1 Lives Display
	auto go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetLocalPosition(20.f, 140.f, 0.f);
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "# Lives: 3", font));
	auto livesDisplay = std::make_unique<LivesDisplayComponent>(go.get());
	LivesDisplayComponent* livesDisplayPtr = livesDisplay.get();
	go->AddComponent(std::move(livesDisplay));
	scene.Add(go);

	//Player 1
	auto player1 = std::make_shared<dae::GameObject>();
	player1->GetTransform()->SetLocalPosition(200.f, 300.f, 0.f);
	player1->AddComponent(std::make_unique<dae::RenderComponent>(player1.get(), "digdug.png"));
	player1->AddComponent(std::make_unique<MoveComponent>(player1.get(), 50.f));
	auto healthComponent = std::make_unique<HealthComponent>(player1.get(), 5);
	healthComponent->AddObserver(livesDisplayPtr);
	player1->AddComponent(std::move(healthComponent));
	scene.Add(player1);

	//P1 Score Display
	go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetLocalPosition(20.f, 180.f, 0.f);
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "Score: 0", font));
	auto scoreDisplay = std::make_unique<ScoreDisplayComponent>(go.get());
	ScoreDisplayComponent* scoreDisplayPtr = scoreDisplay.get();
	go->AddComponent(std::move(scoreDisplay));
	scene.Add(go);

	//Input Binding
	auto& inputManager = dae::InputManager::GetInstance();
	// These should probably be a "EnemyDied" event with event arguments for the score or enemy type but I don't know how the eventArg from the example work
	auto pookaCommand = std::make_unique<dae::TriggerEventCommand>(dae::Event(dae::make_sdbm_hash("PookaDied")));
	pookaCommand->AddObserver(scoreDisplayPtr);
	auto fygarCommand = std::make_unique<dae::TriggerEventCommand>(dae::Event(dae::make_sdbm_hash("FygarDied")));
	fygarCommand->AddObserver(scoreDisplayPtr);
	inputManager.BindKeyboardCommand(SDLK_z, dae::InputManager::ButtonState::Down, std::move(pookaCommand));
	inputManager.BindKeyboardCommand(SDLK_x, dae::InputManager::ButtonState::Down, std::move(fygarCommand));

	inputManager.BindKeyboardCommand(SDLK_s, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3(0.f, 1.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_w, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_a, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_d, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3(1.f, 0.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_c, dae::InputManager::ButtonState::Down, std::make_unique<dae::DieCommand>(player1.get()));

}

void LoadPlayer2(dae::Scene& scene)
{
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	//P2 Lives Display
	auto go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetLocalPosition(20.f, 160.f, 0.f);
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "# Lives: 3", font));
	auto livesDisplay = std::make_unique<LivesDisplayComponent>(go.get());
	LivesDisplayComponent* livesDisplayPtr = livesDisplay.get();
	go->AddComponent(std::move(livesDisplay));
	scene.Add(go);

	//Player 2
	auto player2 = std::make_shared<dae::GameObject>();
	player2->GetTransform()->SetLocalPosition(100.f, 300.f, 0.f);
	player2->AddComponent(std::make_unique<dae::RenderComponent>(player2.get(), "Pooka.png"));
	player2->AddComponent(std::make_unique<MoveComponent>(player2.get(), 100.f));
	auto healthComponent = std::make_unique<HealthComponent>(player2.get(), 5);
	healthComponent->AddObserver(livesDisplayPtr);
	player2->AddComponent(std::move(healthComponent));
	scene.Add(player2);

	//P2 Score Display
	go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetLocalPosition(20.f, 200.f, 0.f);
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "Score: 0", font));
	auto scoreDisplay = std::make_unique<ScoreDisplayComponent>(go.get());
	ScoreDisplayComponent* scoreDisplayPtr = scoreDisplay.get();
	go->AddComponent(std::move(scoreDisplay));
	scene.Add(go);

	//Input Binding
	auto& inputManager = dae::InputManager::GetInstance();

	auto pookaCommand = std::make_unique<dae::TriggerEventCommand>(dae::Event(dae::make_sdbm_hash("PookaDied")));
	pookaCommand->AddObserver(scoreDisplayPtr);
	auto fygarCommand = std::make_unique<dae::TriggerEventCommand>(dae::Event(dae::make_sdbm_hash("FygarDied")));
	fygarCommand->AddObserver(scoreDisplayPtr);
	inputManager.BindGamepadCommand(dae::Gamepad::Button::A, dae::InputManager::ButtonState::Down, std::move(pookaCommand));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::B, dae::InputManager::ButtonState::Down, std::move(fygarCommand));

	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadDown, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3(0.f, 1.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadUp, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadLeft, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadRight, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3(1.f, 0.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::X, dae::InputManager::ButtonState::Down, std::make_unique<dae::DieCommand>(player2.get()));
}


int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}