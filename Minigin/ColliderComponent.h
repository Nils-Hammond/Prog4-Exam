#pragma once
#include "BaseComponent.h"
#include <string>
#include "SDL_rect.h"
#include "SubjectObserver.h"

namespace dae
{
	class ColliderComponent : public BaseComponent
	{
	public:
		ColliderComponent(dae::GameObject* owner, const unsigned int tag);
		~ColliderComponent() override;
		ColliderComponent(const ColliderComponent& other) = delete;
		ColliderComponent(ColliderComponent&& other) = delete;
		ColliderComponent& operator=(const ColliderComponent& other) = delete;
		ColliderComponent& operator=(ColliderComponent&& other) = delete;

		void Update() override;
		//void FixedUpdate() override;
		void Render() const override;
		unsigned int GetTag() const { return m_tag; }
		void SetActive(bool isActive) { m_isActive = isActive; }
		bool IsActive() const { return m_isActive; }
		const SDL_Rect& GetColliderRect() const;
		void OffsetColliderRect(int x, int y);
		void ResizeColliderRect(int width, int height);
		bool IsOverlapping(const ColliderComponent& other) const;
		void OnCollision(const ColliderComponent& other);
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
	private:
		unsigned int m_tag;
		bool m_isActive;
		int m_xOffset;
		int m_yOffset;
		SDL_Rect m_colliderRect;
		Subject m_subject;
	};
}