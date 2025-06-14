#include "LivesAndPoints.h"
#include "TextRenderComponent.h"
#include "SpriteRenderComponent.h"
#include "GameObject.h"
#include "PookaComponent.h"
#include "FygarComponent.h"
#include "GameConstants.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "SelfDestruct.h"
#include "ServiceLocator.h"

HealthComponent::HealthComponent(dae::GameObject* owner, int startingHealth)
	: dae::BaseComponent(owner)
	, m_CurrentHealth(startingHealth)
	, m_MaxHealth(startingHealth)
	, m_pSubject(std::make_unique<dae::Subject>())
{
}

int HealthComponent::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;
	m_pSubject->Notify(dae::Event{ dae::make_sdbm_hash("HealthChanged"), GetHealth()});
	return m_CurrentHealth;
}

int HealthComponent::GetHealth() const
{
	return m_CurrentHealth;
}

void HealthComponent::RegisterObserver(dae::Observer* observer)
{
	m_pSubject->AddObserver(observer);
}

void HealthComponent::UnregisterObserver(dae::Observer* observer)
{
	m_pSubject->RemoveObserver(observer);
}

LivesDisplayComponent::LivesDisplayComponent(dae::GameObject* owner, int startingLives)
	: dae::BaseComponent(owner)
{
	const auto& children = GetOwner()->GetChildren();
	assert(children.size() < startingLives + 1 && "Not enough subObjects to work with");
	for (const auto& child : children)
	{
		m_pLifeIcons.emplace_back(child->GetComponent<dae::RenderComponent>());
	}
#if _DEBUG
	for (const auto& renderComponent : m_pLifeIcons)
	{
		assert(renderComponent != nullptr && "Not all objects have a valid SpriteRenderComponent");
	}
#endif
}

void LivesDisplayComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("HealthChanged"))
	{
		try
		{
			int newHealth = std::any_cast<int>(event.data);
			if (newHealth >= 0)
			{
				m_pLifeIcons[newHealth]->GetOwner()->SetRenderLayer(-1);
				dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/DeathAnimation.wav", 128, false, -1);
			}
			else
			{
				dae::ServiceLocator::GetSoundSystem().PlaySound("Sounds/GameOver.wav", 128, false, -1);
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting event data: " << e.what() << std::endl;
			return;
		}
	}
}

ScoreDisplayComponent::ScoreDisplayComponent(dae::GameObject* owner)
	: dae::BaseComponent(owner)
{
	m_pTextRenderComponent = GetOwner()->GetComponent<dae::TextRenderComponent>();
	assert(m_pTextRenderComponent != nullptr && "ScoreDisplayComponent constructed before TextRenderComponent");
	m_pTextRenderComponent->SetText("0");
}

void ScoreDisplayComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("OnEnemyDeath"))
	{
		try
		{
			auto* enemyObject = std::any_cast<dae::GameObject*>(event.data);
			int multiplier = 1;
			if (enemyObject->HasComponent<PookaComponent>())
			{
			}
			else if (enemyObject->HasComponent<FygarComponent>())
			{
				multiplier = 2;
			}
			else
			{
				return;
			}
			const auto& enemyPos = enemyObject->GetTransform()->GetWorldPosition();
			int pointValue{ 200 };
			if (enemyPos.y > GRID_SIZE + (LEVEL_HEIGHT - 1) * GRID_SIZE / 4 * 3)
				pointValue = 500;
			else if (enemyPos.y > GRID_SIZE + (LEVEL_HEIGHT - 1) * GRID_SIZE / 2)
				pointValue = 400;
			else if (enemyPos.y > GRID_SIZE + (LEVEL_HEIGHT - 1) * GRID_SIZE / 4)
				pointValue = 300;
			m_score += pointValue * multiplier;
			m_pTextRenderComponent->SetText(std::to_string(m_score));

			auto scoreFont = dae::ResourceManager::GetInstance().LoadFont("../Data/Fonts/Arcade.ttf", 14);
			auto scorePopUp = std::make_shared<dae::GameObject>();
			const glm::vec3& offset{ GRID_SIZE / 4.f, GRID_SIZE / 4.f, 0.f };
			scorePopUp->GetTransform()->SetLocalPosition(enemyPos + offset);
			scorePopUp->SetRenderLayer(5);
			scorePopUp->AddComponent(std::make_unique<dae::TextRenderComponent>(scorePopUp.get(), std::to_string(pointValue * multiplier), scoreFont));
			scorePopUp->AddComponent(std::make_unique<SelfDestruct>(scorePopUp.get(), 2.f));
			auto& scene = dae::SceneManager::GetInstance().GetScene(std::string(SCENE_NAME));
			scene.Add(scorePopUp);
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting event data: " << e.what() << std::endl;
			return;
		}
	}
}
