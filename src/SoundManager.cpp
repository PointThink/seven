#include "SoundManager.hpp"

std::unordered_map<std::string, Sound> SoundManager::sounds;

void SoundManager::Add(std::string id, std::string path)
{
	sounds.insert({id, LoadSound(path.c_str())});
}

void SoundManager::Play(std::string id)
{
	PlaySound(sounds[id]);
}