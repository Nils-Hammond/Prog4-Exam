#include "Scene.h"
#include "GameObject.h"
#include "ServiceLocator.h"

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
		}
	}
}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	assert(object && "Adding Nullptr in m_objects!");
	if (object) m_objects.emplace_back(std::move(object));
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
	int iterations{};
	//for(auto& object : m_objects)
	size_t initialSize = m_objects.size();
	for (size_t idx{}; idx < initialSize; ++idx)
	{
		++iterations;
		//assert(object && "Nullptr in m_objects!");
		m_objects[idx]->Update();
	}
	CleanupObjects();
}

void dae::Scene::FixedUpdate()
{
	ServiceLocator::GetCollisionSystem().FixedUpdate();
	for (auto& object : m_objects)
	{
		object->FixedUpdate();
	}
}

void Scene::Render() const
{
	for (int layer = 0; layer < MAX_RENDER_LAYERS; ++layer)
	{
		for (const auto& object : m_objects)
		{
			if (object->GetRenderLayer() == layer)
			{
				object->Render();
			}
		}
	}
}

