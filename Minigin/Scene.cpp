#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

void dae::Scene::CleanupObjects()
{
	for (unsigned int idx{}; idx < m_objects.size(); ++idx)
	{
		if (m_objects[idx]->IsToBeDestroyed())
		{
			Remove(m_objects[idx]);
			--idx;
			std::cout << "Object removed" << std::endl;
		}
	}
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
	CleanupObjects();
}

void dae::Scene::FixedUpdate()
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

