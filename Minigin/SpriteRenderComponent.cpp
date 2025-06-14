#include "SpriteRenderComponent.h"
#include "DaeTime.h"
#include "Texture2D.h"
#include "SDL_rect.h"
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
	, m_frameDelay(DEFAULT_FRAME_DELAY)
	, m_elapsedTime(0.0f)
	, m_isPaused(false)
	, m_isLooping(true)
{
	if (m_texture)
	{
		glm::ivec2 textureSize = m_texture->GetSize();
		m_frameWidth = textureSize.x / m_columns;
		m_frameHeight = textureSize.y / m_rows;
	}
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
			glm::ivec2 offset = GetOffset();
			Renderer::GetInstance().RenderSprite(*m_texture, pos.x + offset.x, pos.y + offset.y, sourceRect, scale.x, scale.y, GetAngle());
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

// Return true if there is a Next or Previous frame

bool dae::SpriteRenderComponent::NextFrame()
{
	++m_currentFrame;
	if (m_currentFrame >= m_rows * m_columns)
	{
		m_currentFrame = m_columns * m_rows - 1;
		return false;
	}
	return true;
}

bool dae::SpriteRenderComponent::PreviousFrame()
{
	--m_currentFrame;
	if (m_currentFrame < 0)
	{
		m_currentFrame = 0;
		return false;
	}
	return true;
}

void dae::SpriteRenderComponent::SetSpriteGrid(unsigned int rows, unsigned int columns)
{
	m_rows = rows;
	m_columns = columns;
	if (m_texture)
	{
		glm::ivec2 textureSize = m_texture->GetSize();
		m_frameWidth = textureSize.x / m_columns;
		m_frameHeight = textureSize.y / m_rows;
	}
	Reset();
}

void dae::SpriteRenderComponent::SetLooping(bool isLooping)
{
	m_isLooping = isLooping;
}
