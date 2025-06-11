#pragma once
#include <memory>
#include "SoundSystem.h"
#include "CollisionSystem.h"

namespace dae
{
	class ServiceLocator
	{
	public:
		static void Initialize()
		{
			m_NullSoundSystem = std::make_unique<NullSoundSystem>();
			m_NullCollisionSystem = std::make_unique<NullCollisionSysten>();
		}
		static void RegisterSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
		{
			m_SoundSystem = std::move(soundSystem);
		}
		static SoundSystem& GetSoundSystem()
		{
			if (!m_SoundSystem)
				return *m_NullSoundSystem;
			else
				return *m_SoundSystem;
		}
		static void RegisterCollisionSystem(std::unique_ptr<CollisionSystem> collisionSystem)
		{
			m_CollisionSystem = std::move(collisionSystem);
		}
		static CollisionSystem& GetCollisionSystem()
		{
			if (!m_CollisionSystem)
				return *m_NullCollisionSystem;
			else
				return *m_CollisionSystem;
		}
	private:
		static inline std::unique_ptr<SoundSystem> m_SoundSystem = nullptr;
		static inline std::unique_ptr<NullSoundSystem> m_NullSoundSystem = nullptr;
		static inline std::unique_ptr<CollisionSystem> m_CollisionSystem = nullptr;
		static inline std::unique_ptr<NullCollisionSysten> m_NullCollisionSystem = nullptr;
	};
}