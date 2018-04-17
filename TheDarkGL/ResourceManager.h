#pragma once
#include <SFML/Graphics.hpp>
#include <hash_map>
#include <string>

class ResourceManager
{
public:
	ResourceManager(void);
	~ResourceManager(void);

private:
	std::hash_map<std::string, sf::Texture*> Textures;
//	std::hash_map<std::string, sf::Texture*> Textures;
};

