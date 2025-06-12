#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Time : public Singleton<Time>
	{
	public:
		void Start();
		void Update();

		float GetDeltaTime() const;
		float GetFixedTimeStep() const;
		std::chrono::steady_clock::duration GetSleepTime() const;
	private:
		std::chrono::steady_clock::time_point m_LastTime;
		std::chrono::steady_clock::time_point m_CurrentTime;
		float m_DeltaTime{};
		const long MS_PER_FRAME{ 16 };
		const float FIXED_TIME_STEP{ 0.05f };
	};
}
