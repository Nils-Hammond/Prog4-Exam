#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
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

template <typename T>
void CheckComponent(const std::string& templateName, dae::GameObject *object)
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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "background.tga"));
	//go->AddComponent(std::make_unique<dae::Transform>(go.get()));
	scene.Add(go);

	//std::cout << "Checking Object 1" << std::endl;
	//CheckComponent<dae::RenderComponent>("RenderComponent", go.get());
	//CheckComponent<dae::FPSComponent>("FPSComponent", go.get());
	//CheckComponent<dae::TextRenderComponent>("TextRenderComponent", go.get());

	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "logo.tga"));
	go->GetTransform()->SetLocalPosition(280.f, 140.f);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 28);
	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "Programming 4 Assignment", font));
	go->GetTransform()->SetLocalPosition(200.f, 20.f);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "0.0 FPS", font));
	go->AddComponent(std::make_unique<dae::FPSComponent>(go.get()));
	go->GetTransform()->SetLocalPosition(20.f, 20.f);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetLocalPosition(200.f, 300.f, 0.f);
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "digdug.png"));
	go->AddComponent(std::make_unique<dae::MoveComponent>(go.get(), 50.f));
	//go->AddComponent(std::make_unique<dae::SelfDestruct>(go.get(), 5.f));
	scene.Add(go);

	auto go2 = std::make_shared<dae::GameObject>();
	go2->GetTransform()->SetLocalPosition(100.f, 300.f, 0.f);
	go2->AddComponent(std::make_unique<dae::RenderComponent>(go2.get(), "Pooka.png"));
	go2->AddComponent(std::make_unique<dae::MoveComponent>(go2.get(), 100.f));
	scene.Add(go2);

	auto& inputManager = dae::InputManager::GetInstance();
	inputManager.BindKeyboardCommand(SDLK_s, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0.f,1.f,0.f)));
	inputManager.BindKeyboardCommand(SDLK_w, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_a, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.BindKeyboardCommand(SDLK_d, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(go.get(), glm::vec3(1.f, 0.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadDown, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(go2.get(), glm::vec3(0.f, 1.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadUp, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(go2.get(), glm::vec3(0.f, -1.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadLeft, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(go2.get(), glm::vec3(-1.f, 0.f, 0.f)));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadRight, dae::InputManager::ButtonState::Pressed, std::make_unique<dae::MoveCommand>(go2.get(), glm::vec3(1.f, 0.f, 0.f)));
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}