#pragma once
#include <memory>
#include "BaseComponent.h"
#include "Transform.h"
#include <vector>
#include <typeinfo>
#include <iostream>

namespace dae
{
	// todo: this should become final.
	class GameObject final
	{
	public:
		void Update();
		void FixedUpdate();
		void Render() const;
		void CleanupComponents();

		void SetParent(GameObject* parent, bool keepWorldPosition = true);
		GameObject* GetParent() const { return m_parent; }
		bool IsChild(GameObject* object) const;
		Transform* GetTransform() const;
		std::vector<GameObject*> GetChildren() { return m_pChildren; }
		void Destroy();
		bool IsToBeDestroyed() const { return m_toBeDestroyed; }

		// Component management
		// Q: Is using a concept to make sure types are derived from BaseComponent something I can do here?
		template <typename T>
		void AddComponent(std::unique_ptr<T> component)
		{
			if (component.get()->GetOwner() != this)
				return;
			for (unsigned int idx{}; idx < _uptrComponents.size(); ++idx)
			{
				if (typeid(T) == typeid(*_uptrComponents[idx].get()))
				{
					return;
				}
			}
			_uptrComponents.push_back(std::move(component));
		}
		template <typename T>
		void RemoveComponent()
		{
			for (unsigned int idx{}; idx < _uptrComponents.size(); ++idx)
			{
				if (typeid(T) == typeid(*_uptrComponents[idx].get()))
				{
					_uptrComponents[idx]->Destroy();
					return;
				}
			}
		}
		template <typename T>
		T* GetComponent()
		{
			for (unsigned int idx{}; idx < _uptrComponents.size(); ++idx)
			{
				if (T* castedComponent = dynamic_cast<T*>(_uptrComponents[idx].get()))
				{
					return castedComponent;
				}
			}
			return nullptr;
		}
		template <typename T>
		bool HasComponent()
		{
			for (unsigned int idx{}; idx < _uptrComponents.size(); ++idx)
			{
				if (typeid(T) == typeid(*_uptrComponents[idx].get()))
				{
					return true;
				}
			}
			return false;
		}

		// Transform specialization
		template<>
		Transform* GetComponent()
		{
			return m_transform.get();
		}
		template<>
		void AddComponent(std::unique_ptr<Transform>)
		{
			assert(false && "Should not be able to add Transform");
		}

		// Constructor & Destructor
		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void RemoveChild(GameObject* object);
		void AddChild(GameObject* object);

		std::unique_ptr<Transform> m_transform;

		std::vector<std::unique_ptr<BaseComponent>> _uptrComponents{};
		std::vector<GameObject*> m_pChildren{};
		GameObject* m_parent{};
		bool m_toBeDestroyed{ false };
	};
}