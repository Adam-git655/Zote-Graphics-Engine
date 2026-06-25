#include "Scene.h"

Scene::Scene()
{
}

void Scene::Setup(Shader& mainShader, Shader& lightCubeShader, Lighting& lighting, Camera& camera)
{
	models.reserve(10);
	meshes.reserve(100);

	//add sponza model
	//models.emplace_back(RESOURCES_PATH"objects/sponza/sponza.obj");
	//std::unique_ptr<GameObject> sponzaObj = std::make_unique<GameObject>("sponza", &mainShader, &models.back());
	//sponzaObj->transform.rotation.y = -90;
	//sponzaObj->transform.position.z = -25;
	//objects.push_back(std::move(sponzaObj));

	//add backpack model
	models.emplace_back(RESOURCES_PATH"objects/backpack/backpack.obj");
	std::unique_ptr<GameObject> backpackObj = std::make_unique<GameObject>("backpack", &mainShader, &models.back());
	backpackObj->transform.position.x = -2;
	backpackObj->transform.position.y = 1;
	objects.push_back(std::move(backpackObj));

	//add water monke model
	models.emplace_back(RESOURCES_PATH"objects/monke/waterMonke.obj");
	std::unique_ptr<GameObject> waterMonkeObj = std::make_unique<GameObject>("waterMonke", &mainShader, &models.back());
	waterMonkeObj->transform.position.x = 2;
	waterMonkeObj->transform.position.y = 1;
	objects.push_back(std::move(waterMonkeObj));

	//add ground mesh
	//GameObject* groundObj = AddPlane(mainShader);
	//groundObj->transform.position.y = -2;
	//groundObj->transform.scale.x *= 10;
	//groundObj->transform.scale.z *= 10;
	//groundObj->color = glm::vec3(0.5, 0.5, 0.5);

	//add green cube
	GameObject* cubeObj = AddCube(mainShader);
	cubeObj->transform.position = { 1.0, -1.4, 0.0 };
	cubeObj->color = glm::vec3(0, 1, 0);

	//add windows
	std::vector<glm::vec3> windows;
	windows.push_back(glm::vec3(-1.5f, 1.4f, -0.48f));
	windows.push_back(glm::vec3(1.5f, 1.4f, 0.51f  ));
	windows.push_back(glm::vec3(0.0f, 1.4f, 0.7f   ));
	windows.push_back(glm::vec3(-0.3f, 1.4f, -2.3f ));
	windows.push_back(glm::vec3(0.5f, 1.4f, -0.6f  ));

	for (int i = 0; i < windows.size(); ++i)
	{
		meshes.emplace_back(Primitives::createQuad(RESOURCES_PATH "textures/transparent_window.png"));
		std::unique_ptr<GameObject> windowObj = std::make_unique<GameObject>("window" + std::to_string(i + 1), &mainShader, &meshes.back());
		windowObj->transform.position = windows[i];
		windowObj->transform.rotation.x = -90;
		windowObj->transparent = true;
		objects.push_back(std::move(windowObj));
	}

	//add directional light (empty game object)
	std::unique_ptr<GameObject> directionalLightObj = std::make_unique<GameObject>("directional_light");
	directionalLightObj->tag = "directional_light";
	objects.push_back(std::move(directionalLightObj));

	//add light cubes
	for (int i = 0; i < lighting.pointLights.size(); ++i)
	{
		AddPointLightGameObject(lightCubeShader);
	}

	//add spot light (empty game object)
	std::unique_ptr<GameObject> spotLightObj = std::make_unique<GameObject>("spot_light");
	spotLightObj->tag = "spot_light";
	objects.push_back(std::move(spotLightObj));

	//add post processing (empty game object)
	std::unique_ptr<GameObject> postProcessingObj = std::make_unique<GameObject>("post_processing");
	postProcessingObj->tag = "post_processing";
	objects.push_back(std::move(postProcessingObj));
}

void Scene::Update(Lighting& lighting, Shader& mainShader)
{
	//update point light properties
	int lightIndex = 0;
	for (auto& obj : objects)
	{
		if (obj->tag == "point_light")
		{
			mainShader.setVec3("pointLights[" + std::to_string(lightIndex) + "].position", obj->transform.position);  //update position to the main shader for lighting calculations
			obj->active = lighting.pointLights[lightIndex].active;
			lightIndex++;
		}
	}
}

void Scene::DrawOpaque(Camera& camera, glm::mat4 projection)
{
	//render opaque objects
	for (auto& obj : objects)
	{
		if (!obj->active || obj->transparent || !obj->shader)
			continue;

		obj->shader->use();

		//set projection matrix
		obj->shader->setMat4("projection", projection);

		//set view matrix
		glm::mat4 view = camera.GetViewMatrix();
		obj->shader->setMat4("view", view);

		if (obj->tag == "point_light")
			glDisable(GL_CULL_FACE);

		obj->Draw();

		if (obj->tag == "point_light")
			glEnable(GL_CULL_FACE);
	}
}

void Scene::DrawTransparent(Camera& camera, glm::mat4 projection)
{
	//render transparent objects

	//sort first based on distance to camera (near->far in map)
	std::map<float, GameObject*> sortedWindows;

	for (auto& obj : objects)
	{
		if (!obj->active || !obj->transparent || !obj->shader)
			continue;

		float dist = glm::length(camera.Position - obj->transform.position);
		sortedWindows[dist] = obj.get();
	}

	glDisable(GL_CULL_FACE);
	//render in reverse order (far->near)
	for (std::map<float, GameObject*>::reverse_iterator it = sortedWindows.rbegin(); it != sortedWindows.rend(); ++it)
	{
		it->second->shader->use();
		it->second->shader->setMat4("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		it->second->shader->setMat4("view", view);

		it->second->Draw();
	}
	glEnable(GL_CULL_FACE);
}

const std::vector<std::unique_ptr<GameObject>>& Scene::getCurrentGameObjects()
{
	return objects;
}

GameObject* Scene::AddCube(Shader& mainShader)
{
	int cubeIndex = 0;
	for (auto& obj : objects)
	{
		if (obj->tag == "cube")
			cubeIndex += 1;
	}

	meshes.emplace_back(Primitives::createCube());
	std::unique_ptr<GameObject> cubeObj = std::make_unique<GameObject>("cube" + (cubeIndex != 0 ? std::to_string(cubeIndex) : ""), &mainShader, &meshes.back());
	cubeObj->transform.position = { 0.0, 0.0, 0.0 };
	cubeObj->tag = "cube";
	objects.push_back(std::move(cubeObj));
	return objects.back().get();
}

GameObject* Scene::AddPlane(Shader& mainShader)
{
	int planeIndex = 0;
	for (auto& obj : objects)
	{
		if (obj->tag == "plane")
			planeIndex += 1;
	}

	meshes.emplace_back(Primitives::createQuad());
	std::unique_ptr<GameObject> planeObj = std::make_unique<GameObject>("plane" + (planeIndex != 0 ? std::to_string(planeIndex) : ""), &mainShader, &meshes.back());
	planeObj->transform.position = { 0.0, 0.0, 0.0 };
	planeObj->tag = "plane";
	objects.push_back(std::move(planeObj));
	return objects.back().get();
}

GameObject* Scene::AddPointLightGameObject(Shader& lightCubeShader)
{
	int pointLightIndex = 0;
	for (auto& obj : objects)
	{
		if (obj->tag == "point_light")
			pointLightIndex += 1;
	}

	meshes.emplace_back(Primitives::createCubeUnlit());
	std::unique_ptr<GameObject> lightObj = std::make_unique<GameObject>("point_light" + (pointLightIndex != 0 ? std::to_string(pointLightIndex) : ""), &lightCubeShader, &meshes.back());
	lightObj->tag = "point_light";
	lightObj->transform.scale = glm::vec3(0.2f);
	objects.push_back(std::move(lightObj));
	return objects.back().get();
}

GameObject* Scene::LoadModel(const char* path, Shader& mainShader, bool flipUvs)
{
	int modelIndex = 0;
	for (auto& obj : objects)
	{
		if (obj->model)
			modelIndex++;
	}

	models.emplace_back(path, flipUvs);
	std::unique_ptr<GameObject> modelObj = std::make_unique<GameObject>("model" + (modelIndex != 0 ? std::to_string(modelIndex) : ""), &mainShader, &models.back());
	objects.push_back(std::move(modelObj));
	return objects.back().get();
}

void Scene::DeleteGameObject(GameObject* obj)
{
	if (!obj)
		return;

	auto it = std::find_if(objects.begin(), objects.end(),
		[obj](const std::unique_ptr<GameObject>& p) { return p.get() == obj; });

	selectedObject = nullptr;

	if (it != objects.end())
		objects.erase(it);
}

