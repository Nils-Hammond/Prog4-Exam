#pragma once
#include <glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class Transform final : public BaseComponent
	{
	public:
		Transform(GameObject* owner, float x = 0.f, float y = 0.f, float z = 0.f);
		~Transform() = default;
		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		void Update() override {};
		void SetLocalPosition(float x, float y, float z = 0);
		void SetLocalPosition(const glm::vec3& pos);
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();
		void SetPositionDirty();
	private:
		glm::vec3 m_localPosition;
		glm::vec3 m_worldPosition;
		bool m_positionIsDirty{ true };

	};
}
