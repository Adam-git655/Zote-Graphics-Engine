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
	void Update(Lighting& lighting, Shader& mainShader);
	void Draw(Camera& camera, glm::mat4 projection);
	const std::vector<std::unique_ptr<GameObject>>& getCurrentGameObjects();
	GameObject* selectedObject = nullptr;

	GameObject* AddCube(Shader& mainShader);
	GameObject* AddPlane(Shader& mainShader);
	GameObject* AddPointLightGameObject(Shader& lightCubeShader);
	GameObject* LoadModel(std::string& path);
	void DeleteGameObject(GameObject* obj);
private:
	std::vector<std::unique_ptr<GameObject>> objects;
	std::vector<Model> models;
	std::vector<Mesh> meshes;
};