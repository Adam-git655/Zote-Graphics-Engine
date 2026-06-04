#pragma once
#include <vector>
#include <memory>
#include "GameObject.h"
#include "camera.h"

class Scene
{
public:
	Scene();
	void Setup(Shader& mainShader, Shader& lightCubeShader);
	void Draw(Camera& camera, glm::mat4 projection);

private:
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<Model> models;
};