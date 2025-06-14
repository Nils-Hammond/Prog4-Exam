#include "PumpComponent.h"
#include "GameObject.h"
#include "DaeTime.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "SDL_Rect.h"
#include "Texture2D.h"
#include "Transform.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GridCell.h"

PumpComponent::PumpComponent(dae::GameObject* owner, const std::string& filename, float scale)
	: dae::RenderComponent(owner, filename, scale), m_isActive(false), m_isPumping(false), m_elapsedTime(0.f)
	, m_maxDuration(0.5f), m_direction(0.f, 0.f), m_pColliderComponent(nullptr), m_pPlayerComponent(nullptr), m_pEnemyCollider(nullptr)
{
	m_pColliderComponent = GetOwner()->GetComponent<dae::ColliderComponent>();
	if (!m_pColliderComponent)
	{
		assert(false && "PumpComponent requires a ColliderComponent to function properly.");
	}
	m_pColliderComponent->AddObserver(this);
	dae::GameObject* parent = GetOwner()->GetParent();
	if (parent)
	{
		m_pPlayerComponent = parent->GetComponent<PlayerComponent>();
	}
	else
	{
		assert(false && "Cannot find parent object");
	}
	if (!m_pPlayerComponent)
	{
		assert(false && "PumpComponent requires a PlayerComponent to function properly.");
	}
}

PumpComponent::~PumpComponent()
{
}

void PumpComponent::Update()
{
	if (m_isActive && !m_isPumping)
	{
		m_direction = m_pPlayerComponent->GetMoveComponent()->GetFacingDirection();
		m_elapsedTime += dae::Time::GetInstance().GetDeltaTime();
		UpdateCollider();
		if (m_elapsedTime >= m_maxDuration)
		{
			m_isActive = false;
			m_pColliderComponent->SetActive(false);
			m_elapsedTime = 0.f;
			m_pEnemyCollider = nullptr;
		}
	}
	else if (!m_isActive && !m_isPumping)
	{
		m_elapsedTime = 0.f;
	}
}

void PumpComponent::Render() const
{
	if (m_isActive || m_isPumping)
	{
		auto pos = GetOwner()->GetTransform()->GetWorldPosition();
		double angle{};
		pos.x += 24;
		const glm::ivec2& textureSize = m_texture.get()->GetSize();
		int sourceWidth = static_cast<int>(textureSize.x * (m_elapsedTime / m_maxDuration));
		SDL_Rect destRect{ static_cast<int>(pos.x), static_cast<int>(pos.y), sourceWidth, static_cast<int>(textureSize.y) };
		float scaleX = 3.f;

		if (m_direction.x < 0.f)
		{
			scaleX = -3.f;
			destRect.x = static_cast<int>(pos.x - sourceWidth * std::abs(scaleX));
			destRect.y = static_cast<int>(pos.y);
		}
		else if (m_direction.y > 0.f)
		{
			angle = 90.0;
			destRect.x = static_cast<int>(pos.x - (sourceWidth * std::abs(scaleX)) / 2);
			destRect.y = static_cast<int>(pos.y + 12 + (sourceWidth * std::abs(scaleX)) / 2);
		}
		else if (m_direction.y < 0.f)
		{
			angle = -90.0;
			destRect.x = static_cast<int>(pos.x - (sourceWidth * std::abs(scaleX)) / 2);
			destRect.y = static_cast<int>(pos.y - 12 - (sourceWidth * std::abs(scaleX)) / 2);
		}
		SDL_Rect sourceRect{ 0, 0, sourceWidth, textureSize.y};
		dae::Renderer::GetInstance().PartialRenderTexture(*m_texture, sourceRect, destRect, scaleX, 3.f, angle);
	}
}

void PumpComponent::Activate()
{
	m_direction = m_pPlayerComponent->GetMoveComponent()->GetFacingDirection();
	m_pEnemyCollider = nullptr;
	m_pColliderComponent->SetActive(true);
	m_isActive = true;
	m_isPumping = false;
	m_elapsedTime = 0.f;
	UpdateCollider();
}

void PumpComponent::Deactivate()
{
	m_isActive = false;
	m_isPumping = false;
	m_pColliderComponent->SetActive(false);
	m_pEnemyCollider = nullptr;
}

bool PumpComponent::IsActive() const
{
	return m_isActive;
}

void PumpComponent::OnNotify(dae::Event event)
{
	if (event.id == dae::make_sdbm_hash("OnCollision"))
	{
		try
		{
			auto* collider = std::any_cast<const dae::ColliderComponent*>(event.data);
			if (collider && (collider->GetTag() == dae::make_sdbm_hash("Enemy") || collider->GetTag() == dae::make_sdbm_hash("Balloon")))
			{
				m_isPumping = true;
				m_pEnemyCollider = collider;
				m_pColliderComponent->SetActive(false);
			}
			else if (collider && collider->GetTag() == dae::make_sdbm_hash("Dirt"))
			{
				//std::cout << "Checking pump vs Dirt" << std::endl;
				auto* gridCell = collider->GetOwner()->GetComponent<GridCell>();
				if (gridCell)
				{
					//std::cout << "Grid Cell at " << gridCell->GetOwner()->GetTransform()->GetWorldPosition().x << ", "
					//	<< gridCell->GetOwner()->GetTransform()->GetWorldPosition().y << std::endl;
					//gridCell->Print();
					// If colliding with more than 2 subcells go no further
					int blockingSubCells = 0;
					for (int subCell{}; subCell < GridCell::NUM_SUB_CELLS; ++subCell)
					{
						if (m_direction.x != 0 && !gridCell->IsSubCellDug(subCell, true))
						{
							SDL_Rect subCellRect{};
							const int subcellSize = GRID_SIZE / GridCell::NUM_SUB_CELLS;
							const glm::vec3& cellPos = collider->GetOwner()->GetTransform()->GetWorldPosition();

							subCellRect.x = static_cast<int>(cellPos.x) + subCell * subcellSize;
							subCellRect.y = static_cast<int>(cellPos.y) + GRID_SIZE / 4;
							subCellRect.w = subcellSize;
							subCellRect.h = GRID_SIZE / 2;
							//std::cout << "SubCellRect x: " << subCellRect.x << ", y: " << subCellRect.y
							//	<< ", w: " << subCellRect.w << ", h: " << subCellRect.h << std::endl;
							//std::cout << "Collider Rect x: " << m_pColliderComponent->GetColliderRect().x
							//	<< ", y: " << m_pColliderComponent->GetColliderRect().y
							//	<< ", w: " << m_pColliderComponent->GetColliderRect().w
							//	<< ", h: " << m_pColliderComponent->GetColliderRect().h << std::endl;
							if (SDL_HasIntersection(&m_pColliderComponent->GetColliderRect(), &subCellRect))
							{
								++blockingSubCells;
							}
						}
						else if (m_direction.y != 0 && !gridCell->IsSubCellDug(subCell, false))
						{
							SDL_Rect subCellRect{};
							const int subcellSize = GRID_SIZE / GridCell::NUM_SUB_CELLS;
							const glm::vec3& cellPos = collider->GetOwner()->GetTransform()->GetWorldPosition();
							subCellRect.x = static_cast<int>(cellPos.x) + GRID_SIZE / 4;
							subCellRect.y = static_cast<int>(cellPos.y) + subCell * subcellSize;
							subCellRect.w = GRID_SIZE / 2;
							subCellRect.h = subcellSize;
							if (SDL_HasIntersection(&m_pColliderComponent->GetColliderRect(), &subCellRect))
							{
								++blockingSubCells;
							}
						}
						//std::cout << "Blocking subcells: " << blockingSubCells << std::endl;
						if (blockingSubCells >= 2)
						{
							//std::cout << "Too many blocking subcells, stopping pump." << std::endl;
							m_isPumping = false;
							m_isActive = false;
							m_elapsedTime = 0.f;
							m_pColliderComponent->SetActive(false);
							m_pEnemyCollider = nullptr;
							return;
						}
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting event data: " << e.what() << std::endl;
			return;
		}
	}
}

const dae::ColliderComponent* PumpComponent::GetHitEnemy() const
{
	return m_pEnemyCollider;
}

void PumpComponent::UpdateCollider()
{
	const glm::ivec2& textureSize = m_texture.get()->GetSize();
	int sourceWidth = static_cast<int>(textureSize.x * (m_elapsedTime / m_maxDuration));

	glm::vec3 pos = GetOwner()->GetTransform()->GetWorldPosition();
	if (m_direction.x > 0.f)
	{
		m_pColliderComponent->OffsetColliderRect(sourceWidth * 3 + 24, 0);
		m_pColliderComponent->ResizeColliderRect(16, 48);
	}
	else if (m_direction.x < 0.f)
	{
		m_pColliderComponent->OffsetColliderRect(-sourceWidth * 3 + 16, 0);
		m_pColliderComponent->ResizeColliderRect(16, 48);
	}
	else if (m_direction.y > 0.f)
	{
		m_pColliderComponent->OffsetColliderRect(0, sourceWidth * 3 + 24);
		m_pColliderComponent->ResizeColliderRect(48, 16);
	}
	else if (m_direction.y < 0.f)
	{
		m_pColliderComponent->OffsetColliderRect(0, -sourceWidth * 3 + 16);
		m_pColliderComponent->ResizeColliderRect(48, 16);
	}
}
