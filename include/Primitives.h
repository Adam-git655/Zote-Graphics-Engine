#pragma once
#include "mesh.h"

namespace Primitives
{
	Mesh createCubeUnlit();
	Mesh createCube(std::string texturePath = "");
	Mesh createQuad(std::string texturePath = "");

	unsigned int createScreenQuadVAO();
	unsigned int createCubeUnlitVAO();
}