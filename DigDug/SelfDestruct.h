#pragma once
#include "BaseComponent.h"


class SelfDestruct final : public dae::BaseComponent
{
public:
	SelfDestruct(dae::GameObject* owner, float timeToLive);
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