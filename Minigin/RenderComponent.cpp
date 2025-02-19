#include "RenderComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& filename)
	: BaseComponent(owner), m_texture(nullptr)
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(GameObject* owner)
	: BaseComponent(owner), m_texture(nullptr)
{
}

void dae::RenderComponent::Update()
{
	
}

void dae::RenderComponent::Render() const
{
	if (m_texture != nullptr)
	{
		Transform* transform = GetOwner()->GetTransform();
		if (transform != nullptr)
		{
			const auto& pos = transform->GetWorldPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
		}
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}