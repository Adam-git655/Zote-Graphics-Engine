#pragma once
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "ShaderClass.h"
#include "glm/glm.hpp"
#include "camera.h"
#include <array>

class Lighting
{
public:
	Lighting();
	void SetShaderParameters(Shader& mainShader, Shader& lightCubeShader, Camera& camera);
	void SetImGuiLightingParameters();

private:
	//Dirlight vars
	bool dirLightActive = true;
	glm::vec3 dirLightDirection = { -0.2f, -1.0f, -0.3f };
	glm::vec3 dirLightambient = { 0.1f, 0.1f, 0.1f };
	glm::vec3 dirLightdiffuse = { 0.4f, 0.4f, 0.4f };
	glm::vec3 dirLightspecular = { 0.5f, 0.5f, 0.5f };

	//PointLight vars
	static constexpr int NR_POINT_LIGHTS = 2;

	std::array<bool, NR_POINT_LIGHTS> pointLightsActive = { 
		true,
		true 
	};
	std::array<glm::vec3, NR_POINT_LIGHTS> pointLightPositions = {
		glm::vec3(0.7f,  0.2f,  2.0f), 
		glm::vec3(2.3f, -3.3f, -4.0f) 
	};
	std::array<glm::vec3, NR_POINT_LIGHTS> pointLightAmbients = {
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f)
	};
	std::array <glm::vec3, NR_POINT_LIGHTS> pointLightDiffuses = {
		glm::vec3(0.8f, 0.8f, 0.8f),
		glm::vec3(0.8f, 0.8f, 0.8f)
	};
	std::array<glm::vec3, NR_POINT_LIGHTS> pointLightSpeculars = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	};
	std::array<glm::vec3, NR_POINT_LIGHTS> pointLightSourceCubeColors = {
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	};

	//SpotLight vars
	bool spotLightActive = true;
	float spotLightInnerCutoff = glm::cos(glm::radians(12.5f));
	float spotLightOuterCutoff = glm::cos(glm::radians(17.5f));
	glm::vec3 spotLightAmbient = { 0.1f,0.1f, 0.1f };
	glm::vec3 spotLightDiffuse = { 0.6f, 0.6f, 0.6f };
	glm::vec3 spotLightSpecular = { 1.0f, 1.0f, 1.0f };
};