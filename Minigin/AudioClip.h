#pragma once
#include <string>

struct Mix_Chunk;
namespace dae
{
	class AudioClip final
	{
	public:
		/**
		 * Simple RAII wrapper for an SDL_Mix_Chunk
		 */
		Mix_Chunk* GetSDLChunk() const;
		explicit AudioClip(const std::string& filePath);
		~AudioClip();

		void SetVolume(uint8_t volume) const;

		AudioClip(const AudioClip&) = delete;
		AudioClip(AudioClip&&) = delete;
		AudioClip& operator=(const AudioClip&) = delete;
		AudioClip& operator=(AudioClip&&) = delete;
	private:
		Mix_Chunk* m_chunk;
	};
}