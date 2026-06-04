#include "Scene.h"

Scene::Scene()
{
}

void Scene::Setup(Shader& mainShader, Shader& lightCubeShader)
{
	models.reserve(10);

	models.emplace_back(RESOURCES_PATH"objects/backpack/backpack.obj");
	objects.push_back(std::make_unique<GameObject>("backpack", &mainShader, &models.back()));
}

void Scene::Draw(Camera& camera, glm::mat4 projection)
{
	for (auto& obj : objects)
	{
		obj->shader->use();

		//set projection matrix
		obj->shader->setMat4("projection", projection);

		//set view matrix
		glm::mat4 view = camera.GetViewMatrix();
		obj->shader->setMat4("view", view);

		obj->Draw();
	}
}