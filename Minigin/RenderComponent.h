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
		void SetScale(glm::vec2 scale);
		glm::vec2 GetScale() const;
		void SetAngle(double angle);
		void SetOffset(int x, int y);

		//RenderComponent(GameObject* owner);
		RenderComponent(GameObject* owner, const std::string& filename, float scale = 1.f);
		RenderComponent(GameObject* owner, const std::string& filename, float scaleX, float scaleY);
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	protected:
		double GetAngle() const { return m_angle; }
		glm::ivec2 GetOffset() const { return { m_offsetX, m_offsetY }; }
		std::shared_ptr<Texture2D> m_texture;
	private:
		float m_scaleX;
		float m_scaleY;
		double m_angle;
		int m_offsetX;
		int m_offsetY;
	};
}
