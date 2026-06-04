#include "Scene.h"

Scene::Scene()
{
}

void Scene::Setup(Shader& mainShader, Shader& lightCubeShader, Lighting& lighting)
{
	models.reserve(2);
	meshes.reserve(lighting.NR_POINT_LIGHTS);

	//add backpack model
	models.emplace_back(RESOURCES_PATH"objects/backpack/backpack.obj");
	std::unique_ptr<GameObject> backpackObj = std::make_unique<GameObject>("backpack", &mainShader, &models.back());
	backpackObj->transform.position.x = -2;
	objects.push_back(std::move(backpackObj));

	//add water monke model
	models.emplace_back(RESOURCES_PATH"objects/monke/waterMonke.obj");
	std::unique_ptr<GameObject> waterMonkeObj = std::make_unique<GameObject>("waterMonke", &mainShader, &models.back());
	waterMonkeObj->transform.position.x = 2;
	objects.push_back(std::move(waterMonkeObj));

	//add light cubes
	for (int i = 0; i < lighting.NR_POINT_LIGHTS; ++i)
	{
		meshes.emplace_back(Primitives::createCube());
		std::unique_ptr<GameObject> lightObj = std::make_unique<GameObject>("point_light" + std::to_string(i + 1), &lightCubeShader, &meshes.back());

		//set position of light cubes
		lightObj->transform.position = lighting.pointLightPositions[i];
		lightObj->transform.scale = glm::vec3(0.2f);

		objects.push_back(std::move(lightObj));
	}
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