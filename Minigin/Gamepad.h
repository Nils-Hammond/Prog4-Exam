#pragma once
#include <memory>
namespace dae
{
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
	private:
		class XInputGamepadImpl;
		std::unique_ptr<XInputGamepadImpl> m_pImpl;
	};
}
