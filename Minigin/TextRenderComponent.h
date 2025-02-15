#pragma once
#include <string>
#include <memory>
#include "RenderComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextRenderComponent final : public RenderComponent
	{
	public:
		void Update() override;
		//void Render() const;

		void SetText(const std::string& text);

		TextRenderComponent(std::weak_ptr<GameObject> parentObject, const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextRenderComponent() = default;
		TextRenderComponent(const TextRenderComponent& other) = delete;
		TextRenderComponent(TextRenderComponent&& other) = delete;
		TextRenderComponent& operator=(const TextRenderComponent& other) = delete;
		TextRenderComponent& operator=(TextRenderComponent&& other) = delete;
	private:
		std::string m_text;
		std::shared_ptr<Font> m_font;
		bool m_needsUpdate;
	};
}
