#include "stdafx.h"
#include "TextureManager.h"

void TextureManager::loadTexture(const std::string & name, const std::string & filename)
{
	// Load texture
	sf::Texture text;
	if(!text.loadFromFile(filename)){
		throw new std::exception("file was not found in call to TextureManager::loadTexture");
	}
	// Add to list of textures
	this->textures[name] = text;
}

sf::Texture & TextureManager::getRef(const std::string & texture)
{
	// TODO: insert return statement here
	return this->textures.at(texture);
}
