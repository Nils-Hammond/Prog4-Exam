#pragma once
#include <string>
#include <memory>
#include "RenderComponent.h"

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

		TextRenderComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font);
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
	};
}
