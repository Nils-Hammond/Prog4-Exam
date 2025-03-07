#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextRenderComponent.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "FPSComponent.h"
#include <iostream>
#include "BaseComponent.h"
#include "RotateAround.h"
#include "SelfDestruct.h"

template <typename T>
void CheckComponent(const std::string& templateName, dae::GameObject *object)
{
	if (object->HasComponent<T>())
	{
		std::cout << "Object has a " << templateName << " component" << std::endl;
	}
	else
	{
		std::cout << "Object does not have a " << templateName << " component" << std::endl;
	}
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "background.tga"));
	//go->AddComponent(std::make_unique<dae::Transform>(go.get()));
	scene.Add(go);

	//std::cout << "Checking Object 1" << std::endl;
	//CheckComponent<dae::RenderComponent>("RenderComponent", go.get());
	//CheckComponent<dae::FPSComponent>("FPSComponent", go.get());
	//CheckComponent<dae::TextRenderComponent>("TextRenderComponent", go.get());

	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "logo.tga"));
	go->GetTransform()->SetLocalPosition(280.f, 140.f);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 28);
	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "Programming 4 Assignment", font));
	go->GetTransform()->SetLocalPosition(200.f, 20.f);
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "0.0 FPS", font));
	go->AddComponent(std::make_unique<dae::FPSComponent>(go.get()));
	go->GetTransform()->SetLocalPosition(20.f, 20.f);
	scene.Add(go);

	auto go2 = std::make_shared<dae::GameObject>();
	go2->GetTransform()->SetLocalPosition(180.f, 240.f);
	scene.Add(go2);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent(std::make_unique<dae::RenderComponent>(go.get(), "digdug.png"));
	go->SetParent(go2.get());
	go->AddComponent(std::make_unique<dae::RotateAround>(go.get(), 6.f, 50.f));
	//go->AddComponent(std::make_unique<dae::SelfDestruct>(go.get(), 5.f));
	scene.Add(go);

	go2 = std::make_shared<dae::GameObject>();
	go2->AddComponent(std::make_unique<dae::RenderComponent>(go2.get(), "digdug.png"));
	go2->SetParent(go.get());
	go2->AddComponent(std::make_unique<dae::RotateAround>(go2.get(), -10.f, 30.f));
	go2->AddComponent(std::make_unique<dae::TextRenderComponent>(go.get(), "Child", font));  // Will not get added as it belongs to the wrong object
	scene.Add(go2);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}