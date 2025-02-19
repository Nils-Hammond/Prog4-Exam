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
		virtual void Render() const {};
		virtual void Destroy() { m_toBeDestroyed = true; };
		virtual bool IsToBeDestroyed() const { return m_toBeDestroyed; };
		GameObject* GetOwner() const { return m_pOwner; };

		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

	protected:
		explicit BaseComponent(GameObject* owner): m_pOwner(owner) {}
	private:
		GameObject* m_pOwner{};
		bool m_toBeDestroyed{ false };
	};
}