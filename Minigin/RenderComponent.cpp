#include "RenderComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> parentObject, const std::string& filename)
	: BaseComponent(parentObject), m_texture(nullptr)
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(std::weak_ptr<GameObject> parentObject)
	: BaseComponent(parentObject), m_texture(nullptr)
{
}

void dae::RenderComponent::Update()
{
	
}

void dae::RenderComponent::Render() const
{
	if (m_texture != nullptr)
	{
		if (auto parent = _parentObjectPtr.lock())
		{
			const Transform* transform = parent->GetComponent<Transform>();
			if (transform != nullptr)
			{
				const auto& pos = transform->GetPosition();
				Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
			}
		}
	}
}

// This implementation uses the "dirty flag" pattern
void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}