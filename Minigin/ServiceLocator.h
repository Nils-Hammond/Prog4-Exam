#pragma once
#include <memory>
#include "SoundSystem.h"

namespace dae
{
	class ServiceLocator
	{
	public:
		static void Initialize()
		{
			m_NullSoundSystem = std::make_unique<NullSoundSystem>();
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
	private:
		static inline std::unique_ptr<SoundSystem> m_SoundSystem = nullptr;
		static inline std::unique_ptr<NullSoundSystem> m_NullSoundSystem = nullptr;
	};
}