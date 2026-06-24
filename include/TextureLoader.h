#pragma once
#include "glad/glad.h"

#include <iostream>
#include <vector>

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

namespace TextureLoader
{
	unsigned int TextureFromFile(const char* path, const std::string& directory);
	unsigned int LoadCubemapTexture(std::vector<std::string> facesTexPaths);
};