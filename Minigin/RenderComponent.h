#pragma once
#include <string>
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	class RenderComponent : public BaseComponent
	{
	public:
		virtual void Update() override;
		virtual void Render() const;

		void SetTexture(const std::string& text);

		RenderComponent(GameObject* owner);
		RenderComponent(GameObject* owner, const std::string& filename);
		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;
	protected:
		std::shared_ptr<Texture2D> m_texture;
	};
}
