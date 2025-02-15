#pragma once
#include <memory>

namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:
		virtual void Update() = 0;
		virtual void FixedUpdate() {};
		virtual void Destroy() { _toBeDestroyed = true; };
		virtual bool ToBeDestroyed() const { return _toBeDestroyed; };

		BaseComponent(std::weak_ptr<GameObject> parentObject);
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		std::weak_ptr<GameObject> _parentObjectPtr{};
		bool _toBeDestroyed{ false };
	};
}