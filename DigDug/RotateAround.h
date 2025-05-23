#pragma once
#include <glm.hpp>
#include "BaseComponent.h"

class RotateAround final : public dae::BaseComponent
{
public:
	RotateAround(dae::GameObject* owner, float speed, float radius);
	virtual void Update() override;
	virtual ~RotateAround() = default;
	RotateAround(const RotateAround& other) = delete;
	RotateAround(RotateAround&& other) = delete;
	RotateAround& operator=(const RotateAround& other) = delete;
	RotateAround& operator=(RotateAround&& other) = delete;
private:
	float m_Speed;
	float m_Radius;
	float m_Angle;
};

