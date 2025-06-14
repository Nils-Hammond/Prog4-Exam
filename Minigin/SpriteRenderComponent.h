#pragma once
#include "RenderComponent.h"
#include "glm.hpp"
#include <string>

struct SDL_Rect;
namespace dae
{
	class Texture2D;
	class SpriteRenderComponent final : public dae::RenderComponent
	{
	public:
		SpriteRenderComponent(dae::GameObject* owner, const std::string& filename, unsigned int rows, unsigned int columns, float scale);
		~SpriteRenderComponent() override;
		SpriteRenderComponent(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent(SpriteRenderComponent&& other) = delete;
		SpriteRenderComponent& operator=(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent& operator=(SpriteRenderComponent&& other) = delete;

		void Update() override;
		void Render() const override;
		void Play();
		void Pause();
		void Reset();
		bool NextFrame();
		bool PreviousFrame();
		void SetSpriteGrid(unsigned int rows, unsigned int columns);
		void SetLooping(bool isLooping);
	private:
		const float DEFAULT_FRAME_DELAY{ 0.2f };
		unsigned int m_rows;
		unsigned int m_columns;
		unsigned int m_currentFrame;
		int m_frameHeight;
		int m_frameWidth;
		float m_frameDelay;
		float m_elapsedTime;
		bool m_isPaused;
		bool m_isLooping;
	};
}