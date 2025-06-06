#pragma once
#include <string>
#include "BaseComponent.h"
#include "glm.hpp"

namespace dae
{
	class Texture2D;

	class RenderComponent : public BaseComponent
	{
	public:
		virtual void Update() override;
		virtual void Render() const;

		void SetTexture(const std::string& text);
		void SetScale(float scaleX, float scaleY);
		void SetScale(float scale);
		glm::vec2 GetScale() const;
		void SetAngle(double angle);

		RenderComponent(GameObject* owner);
		RenderComponent(GameObject* owner, const std::string& filename, float scaleX = 1.f, float scaleY = 1.f);
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	protected:
		std::shared_ptr<Texture2D> m_texture;
	private:
		float m_scaleX;
		float m_scaleY;
		double m_angle;
	};
}
