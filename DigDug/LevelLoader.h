#pragma once
#include "BaseComponent.h"
#include <string>
#include <array>
#include "Scene.h"
#include "GameConstants.h"
#include "SubjectObserver.h"

class LivesDisplayComponent;
class GridCell;
class PlayerComponent;
class LevelLoader final : public dae::BaseComponent, public dae::Observer
{
public:
	LevelLoader(dae::GameObject* owner, dae::Scene* scene, bool versus = false);
	~LevelLoader();
	LevelLoader(const LevelLoader& other) = delete;
	LevelLoader(LevelLoader&& other) = delete;
	LevelLoader& operator=(const LevelLoader& other) = delete;
	LevelLoader& operator=(LevelLoader&& other) = delete;

	void Update() override;
	void LoadLevel(const std::string& levelFile);
	void LoadNextLevel();
	void OnNotify(dae::Event event);
private:
	void SpawnLevelObjects(const std::string& levelFile);
	void ClearLevel();
	void SpawnPooka(int x, int y);
	void SpawnFygar(int x, int y);
	void SpawnPlayer1(int x, int y);
	void SpawnPlayer2(int x, int y);
	void SpawnRock(int x, int y);
	void SpawnHole(const std::string& line, int x, int y);
	void SpawnDiggableCell(int x, int y);
	void LinkGameObjects();
	void CreateScoreDisplay(float posX, float posY);
	LivesDisplayComponent* CreateLivesDisplay(float posX, float posY);
	void ResetEntities();
	void GetLevelInfo(const std::string& filename);
	std::array<GridCell*, LEVEL_WIDTH* LEVEL_HEIGHT> m_gridCells;
	PlayerComponent* m_pPlayer1;
	PlayerComponent* m_pPlayer2;
	std::vector<dae::GameObject*> m_pEnemies;
	std::vector<dae::GameObject*> m_pUIObjects;
	std::vector<dae::GameObject*> m_pNonPersistentObjects;
	dae::Scene* m_currentScene;
	bool m_versus;
	int m_levelNumber;
	bool m_coop;
	bool m_firstLoad;
	bool m_startResetTimer;
	bool m_startNextTimer;
	float m_timer;
	std::string m_levelFolderPath;

	const float RESET_TIMER{ 3.f };
	const size_t P1_HEALTH_DISPLAY_INDEX = { 0 }; //Hard coded indexing because I'm running out of time to finish
	const size_t P2_HEALTH_DISPLAY_INDEX = { 1 };
	const size_t SCORE_DISPLAY_INDEX = { 2 };
};

