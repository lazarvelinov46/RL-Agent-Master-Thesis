#include "stdafx.h"
#include "TextureManager.h"


const std::string TextureManager::TEXTURE_FOLDER_PATH= "assets/Textures/";
std::unordered_map<std::string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::getTexture(const std::string& filename) 
{
	auto it = textures.find(filename);
	if (it != textures.end()) {
		return it->second;
	}
	sf::Texture texture;
	if (!texture.loadFromFile(filename)) {
		throw std::runtime_error("Could not load texture: " + filename);
	}
	textures[filename] = texture;
	return textures[filename];
}

std::string TextureManager::getTextureFolderPath()
{
	return TextureManager::TEXTURE_FOLDER_PATH;
}
