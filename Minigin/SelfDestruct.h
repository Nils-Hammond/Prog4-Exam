#pragma once
#include "BaseComponent.h"

namespace dae
{
	class SelfDestruct final : public BaseComponent
	{
	public:
		SelfDestruct(GameObject* owner, float timeToLive);
		virtual ~SelfDestruct() = default;
		void Update() override;
	private:
		float m_TimeToLive;
		float m_CurrentTime;
	};
}
