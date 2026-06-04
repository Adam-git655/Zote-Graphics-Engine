#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "camera.h"
#include "Lighting.h"
#include "Primitives.h"

class Scene
{
public:
	Scene();
	void Setup(Shader& mainShader, Shader& lightCubeShader, Lighting& lighting);
	void Draw(Camera& camera, glm::mat4 projection);

private:
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<Model> models;
	std::vector<Mesh> meshes;
};