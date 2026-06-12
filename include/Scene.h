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
	void Setup(Shader& mainShader, Shader& lightCubeShader, Lighting& lighting, Camera& camera);
	void Draw(Camera& camera, glm::mat4 projection, Lighting& lighting, Shader& mainShader);
	const std::vector<std::unique_ptr<GameObject>>& getCurrentGameObjects();
	GameObject* selectedObject = nullptr;

	GameObject* AddCube(Shader& mainShader);
	GameObject* AddPlane(Shader& mainShader);
	GameObject* LoadModel(std::string& path);
private:
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<Model> models;
	std::vector<Mesh> meshes;
};