#include <SDL.h>
#include "InputManager.h"
#include <iostream>

dae::InputManager::~InputManager() = default;

void dae::InputManager::Init()
{
	m_gamepad = std::make_unique<Gamepad>(0);
	m_keyState = SDL_GetKeyboardState(NULL);
}

bool dae::InputManager::ProcessInput()
{
	// Keyboard Input
	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			for (const auto& [keyPair, command] : m_keyboardCommands)
			{
				if (keyPair.first == e.key.keysym.sym && keyPair.second == ButtonState::Down)
					command.get()->Execute();
			}
		}
		if (e.type == SDL_KEYUP) {
			for (const auto& [keyPair, command] : m_keyboardCommands)
			{
				if (keyPair.first == e.key.keysym.sym && keyPair.second == ButtonState::Up)
					command.get()->Execute();
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {

		}
	}

	for (const auto& [keyPair, command] : m_keyboardCommands)
	{
		if (m_keyState[SDL_GetScancodeFromKey(keyPair.first)] && keyPair.second == ButtonState::Pressed)
			command.get()->Execute();
	}

	// Gamepad Input
	m_gamepad->UpdateInput();
	for (const auto& [keyPair, command] : m_gamepadCommands)
	{
		if (m_gamepad->IsButtonDownThisFrame(keyPair.first) && keyPair.second == ButtonState::Down)
		{
			command.get()->Execute();
		}
	}
	for (const auto& [keyPair, command] : m_gamepadCommands)
	{
		if (m_gamepad->IsButtonUpThisFrame(keyPair.first) && keyPair.second == ButtonState::Up)
			command.get()->Execute();
	}
	for (const auto& [keyPair, command] : m_gamepadCommands)
	{
		if (m_gamepad->IsButtonPressed(keyPair.first) && keyPair.second == ButtonState::Pressed)
			command.get()->Execute();
	}

	return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Keycode key, ButtonState state, std::unique_ptr<Command> command)
{
	m_keyboardCommands.emplace(std::make_pair(key, state), std::move(command));
}

void dae::InputManager::BindGamepadCommand(Gamepad::Button button, ButtonState state, std::unique_ptr<Command> command)
{
	m_gamepadCommands.emplace(std::make_pair(button, state), std::move(command));
}