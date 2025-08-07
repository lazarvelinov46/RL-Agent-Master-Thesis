#pragma once
#include "stdafx.h"

class TextureManager {
private:
	const static std::string TEXTURE_FOLDER_PATH; 
	static std::unordered_map<std::string, sf::Texture> textures;
public:
	static sf::Texture& getTexture(const std::string& filename);
	static std::string getTextureFolderPath();
};