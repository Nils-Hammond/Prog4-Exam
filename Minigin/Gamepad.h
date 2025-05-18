#pragma once
#include <memory>
namespace dae
{
	// Will implement the rest of the controller when I need it
	class Gamepad
	{
	public:
		enum class Button
		{
			A,
			B,
			X,
			Y,
			DPadUp,
			DPadDown,
			DPadLeft,
			DPadRight,
			Start,
			Back,
			LeftThumb,
			RightThumb,
			LeftShoulder,
			RightShoulder
		};

		Gamepad(int controllerIndex);
		~Gamepad();

		void UpdateInput();
		bool IsButtonPressed(Button button);
		bool IsButtonDownThisFrame(Button button);
		bool IsButtonUpThisFrame(Button button);
		//glm::vec2 GetLeftJoystick() const;
		//glm::vec2 GetRightJoystick() const;
		//float GetLeftTrigger() const;
		//float GetRightTrigger() const;
	private:
		class XInputGamepadImpl;
		std::unique_ptr<XInputGamepadImpl> m_pImpl;
	};
}
