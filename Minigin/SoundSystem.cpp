#include "SoundSystem.h"
#include "SDL_mixer.h"
#include "ResourceManager.h"
#include "AudioClip.h"
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>

class dae::SDLSoundSystem::SDLSoundSystemImpl
{
public:
	SDLSoundSystemImpl();
	~SDLSoundSystemImpl();
	void PlaySound(const std::string& filename, const int volume, bool loop, const int channel);
	void StopChannel(const int channel);
	void PauseChannel(const int channel);
	void ResumeChannel(const int channel);
	void StopAllSounds();
private:
	struct AudioData
	{
		explicit AudioData(const std::string& file, int vol, int chan, bool looping)
			: filename(file), volume(vol), channel(chan), isLooping(looping) {
		}
		std::string filename;
		int volume;
		int channel;
		bool isLooping;
	};

	void ProcessSounds();
	std::vector<std::shared_ptr<AudioClip>> m_loadedSounds;
	std::thread m_soundThread;
	std::queue<std::unique_ptr<AudioData>> m_soundQueue;
	std::condition_variable m_cv;
	std::mutex m_mutex;
	bool m_isRunning;
};

dae::SDLSoundSystem::SDLSoundSystemImpl::SDLSoundSystemImpl()
	: m_isRunning(true)
{
	Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
	m_soundThread = std::thread(&SDLSoundSystemImpl::ProcessSounds, this);
}

dae::SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
{
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_isRunning = false;
	}
	m_cv.notify_all();
	if (m_soundThread.joinable())
	{
		m_soundThread.join();
	}
	Mix_CloseAudio();
	Mix_Quit();
}

void dae::SDLSoundSystem::SDLSoundSystemImpl::PlaySound(const std::string& filename, const int volume, bool loop, const int channel)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_soundQueue.emplace(std::make_unique<AudioData>(filename, volume, channel, loop));
	m_cv.notify_one();
}

void dae::SDLSoundSystem::SDLSoundSystemImpl::StopChannel(const int channel)
{
	if (channel >= 0)
	{
		Mix_HaltChannel(channel);
	}
	else
	{
		Mix_HaltChannel(-1);
	}
}

void dae::SDLSoundSystem::SDLSoundSystemImpl::PauseChannel(const int channel)
{
	if (channel >= 0)
	{
		Mix_Pause(channel);
	}
	else
	{
		Mix_Pause(-1);
	}
}

void dae::SDLSoundSystem::SDLSoundSystemImpl::ResumeChannel(const int channel)
{
	if (channel >= 0)
	{
		Mix_Resume(channel);
	}
	else
	{
		Mix_Resume(-1);
	}
}

void dae::SDLSoundSystem::SDLSoundSystemImpl::StopAllSounds()
{
}

void dae::SDLSoundSystem::SDLSoundSystemImpl::ProcessSounds()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	while (m_isRunning)
	{
		m_cv.wait(lock, [this] { return !m_soundQueue.empty() || !m_isRunning; });
		if (!m_isRunning) break;
		while (!m_soundQueue.empty())
		{
			std::unique_ptr<AudioData> soundData = std::move(m_soundQueue.front());
			m_soundQueue.pop();
			lock.unlock();
			auto audioClip = ResourceManager::GetInstance().LoadAudioClip(soundData.get()->filename);
			if (audioClip)
			{
				audioClip->SetVolume(static_cast<uint8_t>(soundData.get()->volume));
				Mix_PlayChannel(soundData.get()->channel, audioClip->GetSDLChunk(), soundData.get()->isLooping ? -1 : 0);
			}
			lock.lock();
		}
	}
}

dae::SDLSoundSystem::SDLSoundSystem()
	: m_pImpl(std::make_unique<SDLSoundSystemImpl>())
{
}

dae::SDLSoundSystem::~SDLSoundSystem() = default;

void dae::SDLSoundSystem::PlaySound(const std::string& filename, const int volume, bool loop, const int channel)
{
	m_pImpl->PlaySound(filename, volume, loop, channel);
}

void dae::SDLSoundSystem::StopChannel(const int channel)
{
	m_pImpl->StopChannel(channel);
}

void dae::SDLSoundSystem::PauseChannel(const int channel)
{
	m_pImpl->PauseChannel(channel);
}

void dae::SDLSoundSystem::ResumeChannel(const int channel)
{
	m_pImpl->ResumeChannel(channel);
}
