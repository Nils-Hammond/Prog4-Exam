#include "SpriteRenderComponent.h"
#include "DaeTime.h"
#include "Texture2D.h"
#include "SDL.h"
#include "Transform.h"
#include "GameObject.h"
#include "Renderer.h"

dae::SpriteRenderComponent::SpriteRenderComponent(dae::GameObject* owner, const std::string& filename, unsigned int rows, unsigned int columns, float scale)
	: dae::RenderComponent(owner, filename, scale)
	, m_rows(rows)
	, m_columns(columns)
	, m_currentFrame(0)
	, m_frameHeight(0)
	, m_frameWidth(0)
	, m_frameDelay(0.2f)
	, m_elapsedTime(0.0f)
	, m_isPaused(false)
	, m_isLooping(true)
{
	if (m_texture)
		SDL_QueryTexture(m_texture->GetSDLTexture(), nullptr, nullptr, &m_frameWidth, &m_frameHeight);
	m_frameWidth /= m_columns;
	m_frameHeight /= m_rows;
}

dae::SpriteRenderComponent::~SpriteRenderComponent() = default;

void dae::SpriteRenderComponent::Update()
{
	if (m_isPaused)
		return;
	m_elapsedTime += dae::Time::GetInstance().GetDeltaTime();
	if (m_elapsedTime >= m_frameDelay)
	{
		m_elapsedTime -= m_frameDelay;
		++m_currentFrame;
		if (m_currentFrame >= m_rows * m_columns)
		{
			if (m_isLooping)
			{
				m_currentFrame = 0;
			}
			else
			{
				m_currentFrame = m_rows * m_columns - 1;
			}
		}
	}
}

void dae::SpriteRenderComponent::Render() const
{
	if (m_texture)
	{
		Transform* transform = GetOwner()->GetTransform();
		if (transform)
		{
			const auto& pos = transform->GetWorldPosition();
			SDL_Rect sourceRect{};
			sourceRect.x = (m_currentFrame % m_columns) * m_frameWidth;
			sourceRect.y = (m_currentFrame / m_columns) * m_frameHeight;
			sourceRect.w = m_frameWidth;
			sourceRect.h = m_frameHeight;
			glm::vec2 scale = GetScale();
			Renderer::GetInstance().RenderSprite(*m_texture, pos.x, pos.y, sourceRect, scale.x, scale.y, GetAngle());
		}
	}
}

void dae::SpriteRenderComponent::Play()
{
	m_isPaused = false;
	m_elapsedTime = 0.0f;
}

void dae::SpriteRenderComponent::Pause()
{
	m_isPaused = true;
}

void dae::SpriteRenderComponent::Reset()
{
	m_currentFrame = 0;
	m_elapsedTime = 0.0f;
}
