#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void PartialRenderTexture(const Texture2D& texture, SDL_Rect sourceRect, SDL_Rect destRect, float scaleX = 1.f, float scaleY = 1.f, double angle = 0.f) const;
		void RenderTextureEx(const Texture2D& texture, float x, float y, float scaleX = 1.f, float scaleY = 1.f, double angle = 0.f) const;
		void RenderSprite(const Texture2D& texture, float x, float y, SDL_Rect sourceRect, float scaleX = 1.f, float scaleY = 1.f, double angle = 0.f) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}