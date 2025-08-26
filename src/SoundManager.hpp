#pragma once

#include <string>
#include <unordered_map>
#include <raylib.h>

struct SoundManager
{
	static void Add(std::string id, std::string path);
	static void Play(std::string id);

private:
	static std::unordered_map<std::string, Sound> sounds;
};