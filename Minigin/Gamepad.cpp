#include "Gamepad.h"
#include "Windows.h"
#include "Xinput.h"
#include <iostream>

class dae::Gamepad::XInputGamepadImpl
{
public:
	XInputGamepadImpl(int controllerIndex);
	~XInputGamepadImpl() = default;

	void UpdateInput();
	bool IsButtonPressed(Button button);
	bool IsButtonDownThisFrame(Button button);
	bool IsButtonUpThisFrame(Button button);
private:
	WORD GetButtonFlag(Button button) const;
	
	int m_controllerIndex;
	XINPUT_STATE m_previousState;
	XINPUT_STATE m_currentState;
	unsigned int m_buttonsPressedThisFrame;
	unsigned int m_buttonsReleasedThisFrame;
};

dae::Gamepad::XInputGamepadImpl::XInputGamepadImpl(int controllerIndex)
	: m_controllerIndex{ controllerIndex }, m_previousState{}, m_currentState{},
	m_buttonsPressedThisFrame{}, m_buttonsReleasedThisFrame{}
{
}

void dae::Gamepad::XInputGamepadImpl::UpdateInput()
{
	CopyMemory(&m_previousState, &m_currentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_currentState, sizeof(XINPUT_STATE));
	XInputGetState(m_controllerIndex, &m_currentState);

	//std::cout << m_currentState.Gamepad.wButtons << std::endl;

	auto buttonChanges = m_currentState.Gamepad.wButtons ^ m_previousState.Gamepad.wButtons;
	m_buttonsPressedThisFrame = buttonChanges & m_currentState.Gamepad.wButtons;
	m_buttonsReleasedThisFrame = buttonChanges & (~m_currentState.Gamepad.wButtons);
}

bool dae::Gamepad::XInputGamepadImpl::IsButtonPressed(Button button)
{
	return m_currentState.Gamepad.wButtons & GetButtonFlag(button);
}

bool dae::Gamepad::XInputGamepadImpl::IsButtonDownThisFrame(Button button)
{
	return m_buttonsPressedThisFrame & GetButtonFlag(button);
}

bool dae::Gamepad::XInputGamepadImpl::IsButtonUpThisFrame(Button button)
{
	return m_buttonsReleasedThisFrame & GetButtonFlag(button);
}

WORD dae::Gamepad::XInputGamepadImpl::GetButtonFlag(Button button) const
{
	switch (button)
	{
	case Button::A: return XINPUT_GAMEPAD_A;
	case Button::B: return XINPUT_GAMEPAD_B;
	case Button::X: return XINPUT_GAMEPAD_X;
	case Button::Y: return XINPUT_GAMEPAD_Y;
	case Button::DPadUp: return XINPUT_GAMEPAD_DPAD_UP;
	case Button::DPadDown: return XINPUT_GAMEPAD_DPAD_DOWN;
	case Button::DPadLeft: return XINPUT_GAMEPAD_DPAD_LEFT;
	case Button::DPadRight: return XINPUT_GAMEPAD_DPAD_RIGHT;
	case Button::Start: return XINPUT_GAMEPAD_START;
	case Button::Back: return XINPUT_GAMEPAD_BACK;
	case Button::LeftThumb: return XINPUT_GAMEPAD_LEFT_THUMB;
	case Button::RightThumb: return XINPUT_GAMEPAD_RIGHT_THUMB;
	case Button::LeftShoulder: return XINPUT_GAMEPAD_LEFT_SHOULDER;
	case Button::RightShoulder: return XINPUT_GAMEPAD_RIGHT_SHOULDER;
	default: return 0;
	}
}

dae::Gamepad::Gamepad(int controllerIndex)
	: m_pImpl{std::make_unique<XInputGamepadImpl>(controllerIndex)}
{
}

dae::Gamepad::~Gamepad() = default;

void dae::Gamepad::UpdateInput()
{
	m_pImpl->UpdateInput();
}

bool dae::Gamepad::IsButtonPressed(Button button)
{
	return m_pImpl->IsButtonPressed(button);
}

bool dae::Gamepad::IsButtonDownThisFrame(Button button)
{
	return m_pImpl->IsButtonDownThisFrame(button);
}

bool dae::Gamepad::IsButtonUpThisFrame(Button button)
{
	return m_pImpl->IsButtonUpThisFrame(button);
}

