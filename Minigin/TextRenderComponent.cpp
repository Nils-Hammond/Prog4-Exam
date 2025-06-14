#include <stdexcept>
#include <SDL_ttf.h>
#include <memory>
#include "Texture2D.h"
#include "GameObject.h"
#include "TextRenderComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Transform.h"

dae::TextRenderComponent::TextRenderComponent(GameObject* owner, const std::string& text, std::shared_ptr<Font> font)
	: TextRenderComponent(owner, text, std::move(font), SDL_Color{ 255, 255, 255, 255 })
{ }

dae::TextRenderComponent::TextRenderComponent(GameObject * owner, const std::string & text, std::shared_ptr<Font> font, const SDL_Color & color)
	: m_text(text), m_font(std::move(font)), m_needsUpdate{ true }, BaseComponent(owner), m_texture(nullptr), m_color(color)
{ }

dae::TextRenderComponent::~TextRenderComponent() = default;

void dae::TextRenderComponent::Update()
{
	if (m_needsUpdate)
	{
		//const SDL_Color color = { 255,255,255,255 }; 
		const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_color);
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
		m_texture = std::make_unique<Texture2D>(texture);
		m_needsUpdate = false;
	}
}

void dae::TextRenderComponent::Render() const
{
	if (m_texture != nullptr)
	{
		Transform* transform = GetOwner()->GetTransform();
		if (transform != nullptr)
		{
			const auto& pos = transform->GetWorldPosition();
			Renderer::GetInstance().RenderTextureEx(*m_texture, pos.x, pos.y);
		}
	}
}

void dae::TextRenderComponent::SetText(const std::string& text)
{
	m_text = text;
	m_needsUpdate = true;
}

void dae::TextRenderComponent::SetColor(const SDL_Color& color)
{
	m_color = color;
	m_needsUpdate = true;
}
