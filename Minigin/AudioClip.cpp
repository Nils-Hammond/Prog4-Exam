#include "AudioClip.h"
#include <SDL_mixer.h>
#include <stdexcept>
#include <iostream>

Mix_Chunk* dae::AudioClip::GetSDLChunk() const
{
    return m_chunk;
}

dae::AudioClip::AudioClip(const std::string& filePath)
	: m_chunk(nullptr)
{
	m_chunk = Mix_LoadWAV(filePath.c_str());
	if (m_chunk == nullptr)
	{
		std::cerr << "Failed to load audio clip: " << filePath << " - " << Mix_GetError() << std::endl;
	}
}

dae::AudioClip::~AudioClip()
{
	Mix_FreeChunk(m_chunk);
}

void dae::AudioClip::SetVolume(uint8_t volume) const
{
	Mix_VolumeChunk(m_chunk, volume); // Mix_VolumeChunk does it's own error checking
}
