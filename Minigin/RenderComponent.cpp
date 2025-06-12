#include "RenderComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& filename, const float scaleX, const float scaleY)
	: BaseComponent(owner), m_texture(nullptr), m_scaleX(scaleX), m_scaleY(scaleY), m_angle(0.0)
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(GameObject* owner)
	: BaseComponent(owner), m_texture(nullptr), m_scaleX(1.f), m_scaleY(1.f), m_angle(0.0)
{
}

dae::RenderComponent::RenderComponent(GameObject* owner, const std::string& filename, const float scale)
	: RenderComponent(owner, filename, scale, scale)
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
			Renderer::GetInstance().RenderTextureEx(*m_texture, pos.x, pos.y, m_scaleX, m_scaleY, m_angle);
		}
	}
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::RenderComponent::SetScale(float scaleX, float scaleY)
{
	m_scaleX = scaleX;
	m_scaleY = scaleY;
}

void dae::RenderComponent::SetScale(float scale)
{
	m_scaleX = scale;
	m_scaleY = scale;
}

void dae::RenderComponent::SetScale(glm::vec2 scale)
{
	m_scaleX = scale.x;
	m_scaleY = scale.y;
}

glm::vec2 dae::RenderComponent::GetScale() const
{
	return glm::vec2(m_scaleX, m_scaleY);
}

void dae::RenderComponent::SetAngle(double angle)
{
	m_angle = angle;
}
