#pragma once
#include <string>
#include <memory>
#include "RenderComponent.h"
#include "SDL_pixels.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextRenderComponent final : public BaseComponent
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

		TextRenderComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font);
		TextRenderComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& color);
		~TextRenderComponent();
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;
	private:
		std::unique_ptr<Texture2D> m_texture;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		bool m_needsUpdate;
		SDL_Color m_color;
	};
}
