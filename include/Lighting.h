#pragma once
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "ShaderClass.h"
#include "glm/glm.hpp"
#include "camera.h"
#include <array>
#include <vector>

struct PointLight
{
	glm::vec3 ambient = glm::vec3(0.1f);
	glm::vec3 diffuse = glm::vec3(0.8f);
	glm::vec3 specular = glm::vec3(1.0f);
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	bool active = true;
};

class Lighting
{
public:
	Lighting();
	void SetShaderParameters(Shader& mainShader, Shader& lightCubeShader, Camera& camera);
	void SetImGuiLightingParametersDirectional();
	void SetImGuiLightingParametersPoint(int lightIndex);
	void SetImGuiLightingParametersSpot();

	std::vector<PointLight> pointLights;

	void AddPointLight();
	void RemovePointLight(int lightIndex);

private:
	//Dirlight vars
	bool dirLightActive = true;
	glm::vec3 dirLightDirection = { -0.2f, -1.0f, -0.3f };
	glm::vec3 dirLightambient = { 0.1f, 0.1f, 0.1f };
	glm::vec3 dirLightdiffuse = { 0.4f, 0.4f, 0.4f };
	glm::vec3 dirLightspecular = { 0.5f, 0.5f, 0.5f };

	//PointLight vars
	int intialNrPointLights = 1;

	//SpotLight vars
	bool spotLightActive = true;
	float spotLightInnerCutoff = glm::cos(glm::radians(12.5f));
	float spotLightOuterCutoff = glm::cos(glm::radians(17.5f));
	glm::vec3 spotLightAmbient = { 0.1f,0.1f, 0.1f };
	glm::vec3 spotLightDiffuse = { 0.6f, 0.6f, 0.6f };
	glm::vec3 spotLightSpecular = { 1.0f, 1.0f, 1.0f };
};