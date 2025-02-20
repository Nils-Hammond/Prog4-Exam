#include <string>
#include "GameObject.h"
#include "RenderComponent.h"

dae::GameObject::GameObject()
	: m_parent{ nullptr }, m_transform{ std::make_unique<Transform>(this) }
{
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::RemoveChild(GameObject* object)
{
	if (object == nullptr || object == this)
		return;
	for (auto& child : m_pChildren)
	{
		if (child == object)
		{
			m_pChildren.erase(std::remove(m_pChildren.begin(), m_pChildren.end(), child), m_pChildren.end());
			return;
		}
	}
}

void dae::GameObject::AddChild(GameObject* object)
{
	if (object == nullptr || object == this)
		return;
	m_pChildren.push_back(object);
}

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
		component->Render();
	}
}

void dae::GameObject::CleanupComponents()
{
	for (unsigned int idx{}; idx < _uptrComponents.size(); ++idx)
	{
		if (_uptrComponents[idx]->IsToBeDestroyed())
		{
			_uptrComponents.erase(_uptrComponents.begin() + idx);
			--idx;
		}
	}
}

bool dae::GameObject::IsChild(GameObject* object) const
{
	for (auto& child : m_pChildren)
	{
		if (child == object)
			return true;
	}
	return false;
}

dae::Transform* dae::GameObject::GetTransform() const
{
	return m_transform.get();
}

void dae::GameObject::Destroy()
{
	m_toBeDestroyed = true;
	for (auto& child : m_pChildren)
	{
		child->Destroy();
	}
	if (m_parent)
		SetParent(nullptr);
}

void dae::GameObject::SetParent(GameObject* parent)
{
	if (IsChild(parent) || parent == this || m_parent == parent)
		return;
	if (parent == nullptr)
		m_transform->SetLocalPosition(m_transform->GetWorldPosition());
	else
	{
		//if (keepWorldPosition)
		//	SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		m_transform->SetPositionDirty();
	}
	if (m_parent) m_parent->RemoveChild(this);
	m_parent = parent;
	if (m_parent) m_parent->AddChild(this);
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