#pragma once
#include "BaseComponent.h"

namespace dae
{
	class FPSComponent : public BaseComponent
	{
	public:
		void Update() override;
		void FixedUpdate() override;

		FPSComponent(std::weak_ptr<GameObject> parentObject);
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;
	private:
		int m_frames{};
		float m_timeSinceLastFixedUpdate{};
	};
}