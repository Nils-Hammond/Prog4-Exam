#include "DaeTime.h"

void dae::Time::Start()
{
	m_LastTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = 0.0f;
}

void dae::Time::Update()
{
	m_CurrentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaTime = m_CurrentTime - m_LastTime;
	m_DeltaTime = deltaTime.count();
	m_LastTime = m_CurrentTime;
}

float dae::Time::GetDeltaTime() const
{
	return m_DeltaTime;
}

float dae::Time::GetFixedTimeStep() const
{
	return FIXED_TIME_STEP;
}

std::chrono::steady_clock::duration dae::Time::GetSleepTime() const
{
	return m_CurrentTime + std::chrono::milliseconds(MS_PER_FRAME) - std::chrono::high_resolution_clock::now();
}
