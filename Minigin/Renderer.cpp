#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"

static int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();
	
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::PartialRenderTexture(const Texture2D& texture, SDL_Rect sourceRect, SDL_Rect destRect, float scaleX, float scaleY, double angle) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(destRect.x);
	dst.y = static_cast<int>(destRect.y);
	dst.w = static_cast<int>(destRect.w * std::abs(scaleX));
	dst.h = static_cast<int>(destRect.h * std::abs(scaleY));
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (scaleX < 0.f && scaleY < 0.f)
	{
		flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	}
	else if (scaleX < 0.f)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else if (scaleY < 0.f)
	{
		flip = SDL_FLIP_VERTICAL;
	}
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &sourceRect, &dst, angle, nullptr, flip);
}

void dae::Renderer::RenderTextureEx(const Texture2D& texture, const float x, const float y, const float scaleX, const float scaleY, const double angle) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	dst.w = static_cast<int>(dst.w * std::abs(scaleX));
	dst.h = static_cast<int>(dst.h * std::abs(scaleY));
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (scaleX < 0.f && scaleY < 0.f)
	{
		flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	}
	else if (scaleX < 0.f)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else if (scaleY < 0.f)
	{
		flip = SDL_FLIP_VERTICAL;
	}
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, angle, nullptr, flip);
}

void dae::Renderer::RenderSprite(const Texture2D& texture, float x, float y, SDL_Rect sourceRect, float scaleX, float scaleY, double angle) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(sourceRect.w * std::abs(scaleX));
	dst.h = static_cast<int>(sourceRect.h * std::abs(scaleY));
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (scaleX < 0.f && scaleY < 0.f)
	{
		flip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
	}
	else if (scaleX < 0.f)
	{
		flip = SDL_FLIP_HORIZONTAL;
	}
	else if (scaleY < 0.f)
	{
		flip = SDL_FLIP_VERTICAL;
	}
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &sourceRect, &dst, angle, nullptr, flip);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
