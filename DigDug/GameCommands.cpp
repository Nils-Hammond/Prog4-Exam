#include "GameCommands.h"
#include "GameObject.h"
#include "LivesAndPoints.h"
#include <algorithm>
#include "RenderComponent.h"
#include "DaeTime.h"
#include "ServiceLocator.h"
#include "PlayerComponent.h"
#include "MainMenu.h"
#include "SceneManager.h"
#include "GameConstants.h"
#include "LevelLoader.h"

GameActorCommand::GameActorCommand(dae::GameObject* actor) :
	m_actor{ actor }
{
}

MoveCommand::MoveCommand(dae::GameObject* actor, glm::vec3 direction, float speed)
	: GameActorCommand(actor), m_direction(direction), m_speed(speed)
{
}

void MoveCommand::Execute()
{
	GetGameActor()->GetTransform()->SetLocalPosition(GetGameActor()->GetTransform()->GetLocalPosition() + m_direction * m_speed * dae::Time::GetInstance().GetDeltaTime());
}

DieCommand::DieCommand(dae::GameObject* actor) :
	GameActorCommand(actor)
{
}

void DieCommand::Execute()
{
	HealthComponent* healthComponent = GetGameActor()->GetComponent<HealthComponent>();
	if (healthComponent != nullptr)
	{
		healthComponent->TakeDamage(1);
	}
}

PlaySoundCommand::PlaySoundCommand(const std::string& soundFile, int volume, bool loop, int channel)
	: m_soundFile(soundFile), m_volume(volume), m_loop(loop), m_channel(channel)
{
}

void PlaySoundCommand::Execute()
{
	dae::ServiceLocator::GetSoundSystem().PlaySound(m_soundFile, m_volume, m_loop, m_channel);
}

AttackCommand::AttackCommand(dae::GameObject* actor) :
	GameActorCommand(actor)
{
}

void AttackCommand::Execute()
{
	PlayerComponent* playerComponent = GetGameActor()->GetComponent<PlayerComponent>();
	if (playerComponent != nullptr)
	{
		playerComponent->Attack();
	}
}

MenuNavigateCommand::MenuNavigateCommand(dae::GameObject* actor, int yDirection) :
	GameActorCommand(actor), m_yDirection(yDirection)
{
}

void MenuNavigateCommand::Execute()
{
	MainMenu* menuComponent = GetGameActor()->GetComponent<MainMenu>();
	if (menuComponent != nullptr)
	{
		menuComponent->Navigate(m_yDirection);
	}
}

MenuSelectCommand::MenuSelectCommand(dae::GameObject* actor) :
	GameActorCommand(actor)
{
}

void LoadLevel(const std::string& levelFile, dae::Scene& scene, bool versus)
{
	scene.RemoveAll();
	auto levelLoader = std::make_shared<dae::GameObject>();
	levelLoader->AddComponent(std::make_unique<LevelLoader>(levelLoader.get(), &scene, versus));
	scene.Add(levelLoader);
	levelLoader->GetComponent<LevelLoader>()->LoadLevel(levelFile);
}

void MenuSelectCommand::Execute()
{
	MainMenu* menuComponent = GetGameActor()->GetComponent<MainMenu>();
	if (menuComponent != nullptr)
	{
		MainMenu::GameMode gameMode = menuComponent->GetSelectedMode();
		auto& scene = dae::SceneManager::GetInstance().GetScene(std::string(SCENE_NAME));
		if (gameMode == MainMenu::GameMode::SinglePlayer)
		{
			LoadLevel("../Data/Levels/Level1_1.txt", scene, false);
			// LoadSinglePlayerLevel
		}
		else if (gameMode == MainMenu::GameMode::Cooperative)
		{
			LoadLevel("../Data/Levels/Level1_2.txt", scene, false);
			// LoadMultiplayerLevel
		}
		else if (gameMode == MainMenu::GameMode::Versus)
		{
			LoadLevel("../Data/Levels/Level1_1.txt", scene, true);
			// LoadSinglePlayerLevel in versus mode
		}
	}
}

MuteAudioCommand::MuteAudioCommand()
{
}

void MuteAudioCommand::Execute()
{
	dae::ServiceLocator::GetSoundSystem().ToggleMute();
}

LoadNextLevelCommand::LoadNextLevelCommand(dae::GameObject* levelLoaderActor) :
	GameActorCommand(levelLoaderActor)
{
}

void LoadNextLevelCommand::Execute()
{
	LevelLoader* levelLoader = GetGameActor()->GetComponent<LevelLoader>();
	if (levelLoader != nullptr)
	{
		levelLoader->LoadNextLevel();
	}
}
