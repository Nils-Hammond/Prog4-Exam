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
	
	// Unbind timing is tricky and I could not figure it out
	UnbindPending();
	return true;
}

void dae::InputManager::BindKeyboardCommand(SDL_Keycode key, ButtonState state, std::unique_ptr<Command> command)
{
	m_keyboardCommands.insert_or_assign(std::make_pair(key, state), std::move(command));
}

void dae::InputManager::Unbind(SDL_Keycode key)
{
	m_keysToUnbind.emplace_back(key);
}

void dae::InputManager::BindGamepadCommand(Gamepad::Button button, ButtonState state, std::unique_ptr<Command> command)
{
	m_gamepadCommands.insert_or_assign(std::make_pair(button, state), std::move(command));
}

void dae::InputManager::Unbind(Gamepad::Button button)
{
	m_buttonsToUnbind.emplace_back(button);
}

void dae::InputManager::UnbindPending()
{
	for (const auto& key : m_keysToUnbind)
	{
		std::erase_if(m_keyboardCommands, [key](const auto& pair) {return pair.first.first == key; });
	}
	for (const auto& button : m_buttonsToUnbind)
	{
		std::erase_if(m_gamepadCommands, [button](const auto& pair) {return pair.first.first == button; });
	}
	m_keysToUnbind.clear();
	m_buttonsToUnbind.clear();
}
