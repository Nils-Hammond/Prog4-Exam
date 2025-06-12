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

PumpComponent::PumpComponent(dae::GameObject* owner, const std::string& filename, float scale)
	: dae::RenderComponent(owner, filename, scale), m_isActive(false), m_isPumping(false), m_elapsedTime(0.f)
	, m_maxDuration(0.5f), m_direction(0.f, 0.f), m_pColliderComponent(nullptr), m_pPlayerComponent(nullptr)
{
	m_pColliderComponent = GetOwner()->GetComponent<dae::ColliderComponent>();
	if (!m_pColliderComponent)
	{
		assert(false && "PumpComponent requires a ColliderComponent to function properly.");
	}
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
	m_pColliderComponent->SetActive(true);
	m_isActive = true;
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
			if (collider && collider->GetTag() == dae::make_sdbm_hash("Enemy"))
			{
				m_isPumping = true;
				m_elapsedTime = 0.f;
				UpdateCollider();
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error getting event data: " << e.what() << std::endl;
			return;
		}
	}
}

dae::ColliderComponent* PumpComponent::GetHitEnemy() const
{
	return nullptr;
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
