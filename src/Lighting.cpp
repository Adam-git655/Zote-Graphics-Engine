#include "Lighting.h"

Lighting::Lighting()
{

}

void Lighting::SetShaderParameters(Shader& mainShader, Shader& lightCubeShader, Camera& camera)
{
	mainShader.use();

	mainShader.setVec3("viewPos", camera.Position);

	mainShader.setVec3("dirLight.direction", dirLightDirection);
	if (dirLightActive)
	{
		mainShader.setVec3("dirLight.ambient", dirLightambient);
		mainShader.setVec3("dirLight.diffuse", dirLightdiffuse);
		mainShader.setVec3("dirLight.specular", dirLightspecular);
	}
	else
	{
		mainShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
		mainShader.setVec3("dirLight.diffuse", 0.0f, 0.0f, 0.0f);
		mainShader.setVec3("dirLight.specular", 0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		std::string index = std::to_string(i);

		mainShader.setFloat("pointLights[" + index + "].constant", 1.0f);
		mainShader.setFloat("pointLights[" + index + "].linear", 0.09f);
		mainShader.setFloat("pointLights[" + index + "].quadratic", 0.032f);
		if (pointLightsActive[i])
		{
			mainShader.setVec3("pointLights[" + index + "].ambient", pointLightAmbients[i]);
			mainShader.setVec3("pointLights[" + index + "].diffuse", pointLightDiffuses[i]);
			mainShader.setVec3("pointLights[" + index + "].specular", pointLightSpeculars[i]);
		}
		else
		{
			mainShader.setVec3("pointLights[" + index + "].ambient", 0.0f, 0.0f, 0.0f);
			mainShader.setVec3("pointLights[" + index + "].diffuse", 0.0f, 0.0f, 0.0f);
			mainShader.setVec3("pointLights[" + index + "].specular", 0.0f, 0.0f, 0.0f);
		}
	}

	mainShader.setVec3("spotLight.position", camera.Position);
	if (spotLightActive)
	{
		mainShader.setVec3("spotLight.direction", camera.Front);
		mainShader.setVec3("spotLight.ambient", spotLightAmbient);
		mainShader.setVec3("spotLight.diffuse", spotLightDiffuse);
		mainShader.setVec3("spotLight.specular", spotLightSpecular);
		mainShader.setFloat("spotLight.innerCutoff", spotLightInnerCutoff);
		mainShader.setFloat("spotLight.outerCutoff", spotLightOuterCutoff);
	}
	else
	{
		mainShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		mainShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
		mainShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
	}
}

void Lighting::SetImGuiLightingParametersDirectional()
{
	ImGui::Checkbox("active", &dirLightActive);
	ImGui::DragFloat3("direction", &dirLightDirection[0], 0.1f);
	ImGui::ColorEdit3("ambient", &dirLightambient[0]);
	ImGui::ColorEdit3("diffuse", &dirLightdiffuse[0]);
	ImGui::ColorEdit3("specualar", &dirLightspecular[0]);
}

void Lighting::SetImGuiLightingParametersPoint(int lightIndex)
{
	std::string activeId = "active##" + std::to_string(lightIndex);
	std::string cubeColorId = "cube color##" + std::to_string(lightIndex);
	std::string positionId = "position##" + std::to_string(lightIndex);
	std::string ambientId = "ambient##" + std::to_string(lightIndex);
	std::string diffuseId = "diffuse##" + std::to_string(lightIndex);
	std::string specularId = "specular##" + std::to_string(lightIndex);

	ImGui::Checkbox(activeId.c_str(), &pointLightsActive[lightIndex - 1]);
	ImGui::ColorEdit3(ambientId.c_str(), &pointLightAmbients[lightIndex - 1][0]);
	ImGui::ColorEdit3(diffuseId.c_str(), &pointLightDiffuses[lightIndex - 1][0]);
	ImGui::ColorEdit3(specularId.c_str(), &pointLightSpeculars[lightIndex - 1][0]);
}

void Lighting::SetImGuiLightingParametersSpot()
{
	ImGui::Checkbox("active##spot", &spotLightActive);
	ImGui::SliderFloat("inner radius##spot", &spotLightInnerCutoff, spotLightOuterCutoff, 1.0f);
	ImGui::SliderFloat("outer radius##spot", &spotLightOuterCutoff, 0.0f, spotLightInnerCutoff);
	ImGui::ColorEdit3("ambient##spot", &spotLightAmbient[0]);
	ImGui::ColorEdit3("diffuse##spot", &spotLightDiffuse[0]);
	ImGui::ColorEdit3("specular##spot", &spotLightSpecular[0]);
}