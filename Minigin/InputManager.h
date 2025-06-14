#pragma once
#include <map>
#include <memory>
#include "Singleton.h"
#include "Gamepad.h"
#include "Command.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class ButtonState
		{
			Down,
			Up,
			Pressed
		};
		~InputManager();

		void Init();
		bool ProcessInput();
		void BindKeyboardCommand(SDL_Keycode key, ButtonState state, std::unique_ptr<Command> command);
		void BindGamepadCommand(Gamepad::Button button, ButtonState state, std::unique_ptr<Command> command);
		void Unbind(SDL_Keycode key);
		void Unbind(Gamepad::Button button);
	private:
		void UnbindPending();
		std::vector<SDL_Keycode> m_keysToUnbind;
		std::vector<Gamepad::Button> m_buttonsToUnbind;
		std::map<std::pair<SDL_Keycode, ButtonState>, std::unique_ptr<Command>> m_keyboardCommands;
		std::map<std::pair<Gamepad::Button, ButtonState>, std::unique_ptr<Command>> m_gamepadCommands;
		std::unique_ptr<Gamepad> m_gamepad;

		const Uint8* m_keyState{};
	};

}
