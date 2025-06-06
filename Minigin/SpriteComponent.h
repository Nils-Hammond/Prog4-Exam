#pragma once
#include <string>
#include "RenderComponent.h"

class SpriteComponent : public dae::RenderComponent
{
public:
	//SpriteComponent(dae::GameObject* owner, const std::string& filename, float scaleX = 1.f, float scaleY = 1.f);
	void Update() override {}
	void Render() const override { dae::RenderComponent::Render(); }
	SpriteComponent(const SpriteComponent& other) = delete;
	SpriteComponent(SpriteComponent&& other) = delete;
	SpriteComponent& operator=(const SpriteComponent& other) = delete;
	SpriteComponent& operator=(SpriteComponent&& other) = delete;
	virtual ~SpriteComponent() = default;
private:
	int m_rows;
	int m_columns;
	int m_currentFrame;
	bool m_isAnimating;
	bool m_isLooping;
	float m_frameDelay;
};

