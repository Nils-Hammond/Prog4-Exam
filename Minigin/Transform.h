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
		void SetLocalPosition(const glm::vec3& pos); // Too lazy to do maths but this probably shouldn't exist
		const glm::vec3& GetLocalPosition();
		const glm::vec3& GetWorldPosition();
		void SetPositionDirty();
	private:
		void UpdateWorldPosition();
		glm::vec3 m_localPosition;
		glm::vec3 m_worldPosition;
		bool m_positionIsDirty{ true };

	};
}
