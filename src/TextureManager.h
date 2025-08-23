#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>

struct TextureManager
{
    static void Add(std::string id, std::string path);
    static Texture Get(std::string id);

private:
    static std::unordered_map<std::string, Texture> textures;
};