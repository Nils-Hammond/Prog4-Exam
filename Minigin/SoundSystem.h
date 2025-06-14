#pragma once
#include <string>
#include <memory>

namespace dae
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() {};
		virtual void PlaySound(const std::string& filename, const int volume, bool loop, const int channel) = 0;
		virtual void StopChannel(const int channel) = 0;
		virtual void PauseChannel(const int channel) = 0;
		virtual void ResumeChannel(const int channel) = 0;
		virtual void ToggleMute() = 0;
		virtual bool CheckChannel(const int channel) = 0;
	};

	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem();
		void PlaySound(const std::string& filename, const int volume, bool loop = false, const int channel = -1) override;
		void StopChannel(const int channel) override;
		void PauseChannel(const int channel) override;
		void ResumeChannel(const int channel) override;
		void ToggleMute() override;
		bool CheckChannel(const int channel) override;
	private:
		class SDLSoundSystemImpl;
		std::unique_ptr<SDLSoundSystemImpl> m_pImpl;
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		void PlaySound(const std::string& , const int, bool, const int) override {}
		void StopChannel(const int) override {}
		void PauseChannel(const int) override {}
		void ResumeChannel(const int) override {}
		void ToggleMute() override {}
		bool CheckChannel(const int ) { return false; }
	};
}