#include "MainMenu.h"
#include "SDL_keycode.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "InputManager.h"
#include "GameCommands.h"
#include <iostream>

MainMenu::MainMenu(dae::GameObject* owner)
	: BaseComponent(owner), m_selectedMode(MainMenu::GameMode::SinglePlayer), m_pMarkerObject(nullptr)
{
	auto& inputManager = dae::InputManager::GetInstance();

	inputManager.BindKeyboardCommand(SDLK_w, dae::InputManager::ButtonState::Down, std::make_unique<MenuNavigateCommand>(GetOwner(), -1));
	inputManager.BindKeyboardCommand(SDLK_s, dae::InputManager::ButtonState::Down, std::make_unique<MenuNavigateCommand>(GetOwner(), 1));
	inputManager.BindKeyboardCommand(SDLK_SPACE, dae::InputManager::ButtonState::Down, std::make_unique<MenuSelectCommand>(GetOwner()));

	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadUp, dae::InputManager::ButtonState::Down, std::make_unique<MenuNavigateCommand>(GetOwner(), -1));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::DPadDown, dae::InputManager::ButtonState::Down, std::make_unique<MenuNavigateCommand>(GetOwner(), 1));
	inputManager.BindGamepadCommand(dae::Gamepad::Button::A, dae::InputManager::ButtonState::Down, std::make_unique<MenuSelectCommand>(GetOwner()));
}

MainMenu::~MainMenu()
{
	//auto& inputManager = dae::InputManager::GetInstance();
	//
	//inputManager.Unbind(SDLK_w);
	//inputManager.Unbind(SDLK_s);
	//inputManager.Unbind(SDLK_z);
	//inputManager.Unbind(dae::Gamepad::Button::DPadUp);
	//inputManager.Unbind(dae::Gamepad::Button::DPadDown);
	//inputManager.Unbind(dae::Gamepad::Button::A);
}

void MainMenu::Update()
{
}

void MainMenu::Navigate(int direction)
{
	int mode = static_cast<int>(m_selectedMode);
	const int last = static_cast<int>(GameMode::Cooperative);
	if (direction > 0)
	{
		++mode;
	}
	else if (direction < 0)
	{
		--mode;
	}

	if (mode < 0)
		mode = last;
	else if (mode > last)
		mode = 0;

	m_selectedMode = static_cast<GameMode>(mode);
	if (m_pMarkerObject == nullptr)
	{
		m_pMarkerObject = GetOwner()->GetChildren().front();
	}
	if (m_pMarkerObject)
	{
		const float initialMarkerX{ 265.f };
		const float menuSpacing{ 42.f };
		const glm::vec3 newMarkerPos{ 210.f, initialMarkerX + mode * menuSpacing, 0.f };
		m_pMarkerObject->GetTransform()->SetLocalPosition(newMarkerPos);
	}
}

MainMenu::GameMode MainMenu::GetSelectedMode() const
{
	return m_selectedMode;
}
