#include "Scene.h"

Scene::Scene()
{
}

void Scene::Setup(Shader& mainShader, Shader& lightCubeShader, Lighting& lighting)
{
	models.reserve(2);
	meshes.reserve(10);

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

	//add ground mesh
	meshes.emplace_back(Primitives::createQuad());
	std::unique_ptr<GameObject> groundObj = std::make_unique<GameObject>("ground", &mainShader, &meshes.back());
	groundObj->transform.position.y = -2;
	groundObj->transform.scale.x *= 10;
	groundObj->transform.scale.z *= 10;
	groundObj->color = glm::vec3(0.5, 0.5, 0.5);
	objects.push_back(std::move(groundObj));

	//add light cubes
	for (int i = 0; i < lighting.NR_POINT_LIGHTS; ++i)
	{
		meshes.emplace_back(Primitives::createCubeUnlit());
		std::unique_ptr<GameObject> lightObj = std::make_unique<GameObject>("point_light" + std::to_string(i + 1), &lightCubeShader, &meshes.back());
		lightObj->tag = "point_light";

		//set scale of light cubes
		lightObj->transform.scale = glm::vec3(0.2f);

		objects.push_back(std::move(lightObj));
	}
}

void Scene::Draw(Camera& camera, glm::mat4 projection, Lighting& lighting)
{
	int lightIndex = 0;
	for (auto& obj : objects)
	{
		obj->shader->use();

		//set projection matrix
		obj->shader->setMat4("projection", projection);

		//set view matrix
		glm::mat4 view = camera.GetViewMatrix();
		obj->shader->setMat4("view", view);

		if (obj->tag == "point_light")
		{
			obj->transform.position = lighting.pointLightPositions[lightIndex];
			obj->color = lighting.pointLightSourceCubeColors[lightIndex];
			obj->active = lighting.pointLightsActive[lightIndex];
			lightIndex++;
		}

		if (obj->active)
			obj->Draw();
	}
}