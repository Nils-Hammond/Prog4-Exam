#pragma once
#include "BaseComponent.h"
#include <string>
#include "Scene.h"

class LevelLoader final : public dae::BaseComponent
{
public:
	LevelLoader(dae::GameObject* owner, dae::Scene* scene);
	~LevelLoader() = default;
	LevelLoader(const LevelLoader& other) = delete;
	LevelLoader(LevelLoader&& other) = delete;
	LevelLoader& operator=(const LevelLoader& other) = delete;
	LevelLoader& operator=(LevelLoader&& other) = delete;

	void Update() override {};
	void LoadLevel(const std::string& levelFile);
private:
	void SpawnPooka(int x, int y);
	void SpawnFygar(int x, int y);
	void SpawnPlayer1(int x, int y);
	void SpawnPlayer2(int x, int y);
	void SpawnRock(int x, int y);
	void SpawnHole(const std::string& line, int x, int y);
	void SpawnDiggableCell(int x, int y);
	dae::Scene* m_currentScene;
};

