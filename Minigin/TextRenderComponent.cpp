#include <stdexcept>
#include <SDL_ttf.h>
#include <memory>
#include "GameObject.h"
#include "TextRenderComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Transform.h"

dae::TextRenderComponent::TextRenderComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font)
	: m_text(text), m_font(std::move(font)), m_needsUpdate{true}, RenderComponent(owner)
{ }

void dae::TextRenderComponent::Update()
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_texture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextRenderComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}