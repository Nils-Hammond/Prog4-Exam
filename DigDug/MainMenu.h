#pragma once
#include "BaseComponent.h"

class MainMenu : public dae::BaseComponent
{
public:
	enum class GameMode {
		SinglePlayer,
		Versus,
		Cooperative
	};

	MainMenu(dae::GameObject* owner);
	~MainMenu();
	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) = delete;
	MainMenu& operator=(const MainMenu& other) = delete;
	MainMenu& operator=(MainMenu&& other) = delete;

	void Update() override;
	void Navigate(int direction);
	GameMode GetSelectedMode() const;
private:
	GameMode m_selectedMode;
	dae::GameObject* m_pMarkerObject;
};

