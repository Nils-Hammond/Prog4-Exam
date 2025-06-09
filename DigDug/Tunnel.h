#pragma once
#include "RenderComponent.h"
#include "glm.hpp"
#include "SDL_rect.h"

class Tunnel : public dae::RenderComponent
{
public:
	enum class TunnelDirection
	{
		Left,
		Right,
		Up,
		Down
	};

	Tunnel(dae::GameObject* owner, int x, int y, TunnelDirection direction);
	~Tunnel() = default;
	Tunnel(const Tunnel& other) = delete;
	Tunnel(Tunnel&& other) = delete;
	Tunnel& operator=(const Tunnel& other) = delete;
	Tunnel& operator=(Tunnel&& other) = delete;
	void Update() override {};
	void Render() const override;
	void Dig(float percent);
private:
	TunnelDirection m_direction;
	float m_digPercent;
	glm::vec2 m_position;
	SDL_Rect m_sourceRect;
	SDL_Rect m_destinationRect;
};

