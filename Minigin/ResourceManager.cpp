#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "AudioClip.h"
#include <iostream>

namespace fs = std::filesystem;

void dae::ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	if (m_loadedTextures.find(file) == m_loadedTextures.end())
	{
		m_loadedTextures.insert(std::pair(file, std::make_shared<Texture2D>(fullPath.string())));
	}
	return m_loadedTextures.at(file);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if(m_loadedFonts.find(key) == m_loadedFonts.end())
		m_loadedFonts.insert(std::pair(key,std::make_shared<Font>(fullPath.string(), size)));
	return m_loadedFonts.at(key);
}

std::shared_ptr<dae::AudioClip> dae::ResourceManager::LoadAudioClip(const std::string& file)
{
	const auto fullPath = m_dataPath / file;
	const auto filename = fs::path(fullPath).filename().string();
	if (m_loadedAudioClips.find(filename) == m_loadedAudioClips.end())
	{
		m_loadedAudioClips.insert(std::pair(filename, std::make_shared<AudioClip>(fullPath.string())));
	}
	return m_loadedAudioClips.at(filename);
}

void dae::ResourceManager::UnloadUnusedResources()
{
	for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedTextures.erase(it);
		else
			++it;
	}

	for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedFonts.erase(it);
		else
			++it;
	}
	/*
	for (auto it = m_loadedAudioClips.begin(); it != m_loadedAudioClips.end();)
	{
		if (it->second.use_count() == 1)
			it = m_loadedAudioClips.erase(it);
		else
			++it;
	}
	*/
}
