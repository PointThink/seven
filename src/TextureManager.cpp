#include "TextureManager.h"

std::unordered_map<std::string, Texture> TextureManager::textures;

void TextureManager::Add(std::string id, std::string path)
{
    textures.insert({id, LoadTexture(path.c_str())});
}

Texture TextureManager::Get(std::string id)
{
    return textures[id];
}