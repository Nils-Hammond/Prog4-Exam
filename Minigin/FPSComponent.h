#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextRenderComponent;
	class FPSComponent : public BaseComponent
	{
	public:
		void Update() override;
		void FixedUpdate() override;

		FPSComponent(GameObject* owner);
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;
	private:
		const float TEXT_UPDATE_DELAY{ 0.1f };
		float m_textUpdateTimer;
		TextRenderComponent* m_pTextRenderComponent{};
	};
}