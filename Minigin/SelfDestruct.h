#pragma once
#include "BaseComponent.h"

namespace dae
{
	class SelfDestruct final : public BaseComponent
	{
	public:
		SelfDestruct(GameObject* owner, float timeToLive);
		virtual ~SelfDestruct() = default;
		SelfDestruct(const SelfDestruct& other) = delete;
		SelfDestruct(SelfDestruct&& other) = delete;
		SelfDestruct& operator=(const SelfDestruct& other) = delete;
		SelfDestruct& operator=(SelfDestruct&& other) = delete;
		void Update() override;
	private:
		float m_TimeToLive;
		float m_CurrentTime;
	};
}
