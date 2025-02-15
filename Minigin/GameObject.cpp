#include <string>
#include "GameObject.h"
#include "RenderComponent.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (auto& component : _uptrComponents)
	{
		component->Update();
	}
	CleanupComponents();
}

void dae::GameObject::FixedUpdate()
{
	for (auto& component : _uptrComponents)
	{
		component->FixedUpdate();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& component : _uptrComponents)
	{
		if (const RenderComponent* castedComponent = dynamic_cast<RenderComponent*>(component.get()))
			castedComponent->Render();
	}
}

void dae::GameObject::CleanupComponents()
{
	for (int idx{}; idx < _uptrComponents.size(); ++idx)
	{
		if (_uptrComponents[idx]->ToBeDestroyed())
		{
			_uptrComponents.erase(_uptrComponents.begin() + idx);
			--idx;
		}
	}
}

/*
void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
*/