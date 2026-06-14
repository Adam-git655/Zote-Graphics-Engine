#pragma once
#include "imgui.h"
#include "ImGuizmo.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <imgui_internal.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderClass.h"
#include "camera.h"
#include "model.h"
#include "Scene.h"
#include "Lighting.h"
#include "tinyfiledialogs.h"

#include <iostream>
#include <memory>

struct FrameBufferInfo
{
	unsigned int frameBuffer;
	unsigned int textureColorBuffer;
};

class Application
{
public:
	Application(unsigned int windowWidth, unsigned int windowHeight, const char* appName);
	~Application();

	void Run();
private:

	GLFWwindow* window;

	unsigned int windowWidth;
	unsigned int windowHeight;
	float viewportWidth;
	float viewportHeight;
	const char* appName;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	bool first_mouse = true;
	bool mouseCaptured = true;
	bool spacePressedLastFrame = false;
	bool fPressedLastFrame = false;
	bool qPressedLastFrame = false;
	bool wPressedLastFrame = false;
	bool ePressedLastFrame = false;
	bool rPressedLastFrame = false;
	bool delPressedLastFrame = false;

	float lastx = 400;
	float lasty = 300;

	glm::vec3 clearColor = { 0.0f, 0.0f, 0.0f };

	Camera camera = { glm::vec3(0.0f, 0.0f, 3.0f) };
	Scene scene;
	Lighting lighting;

	FrameBufferInfo frameBuffer1Info;
	FrameBufferInfo frameBuffer2Info;
	unsigned int screenQuadVAO;

	//Shaders
	std::unique_ptr<Shader> mainShader;
	std::unique_ptr<Shader> lightCubeShader;
	std::unique_ptr<Shader> screenShader;

	glm::mat4 projection = glm::mat4(1.0f);

	//ImGuizmo
	bool gizmoEnabled = true;
	ImGuizmo::OPERATION currentTransformOperation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE currentGizmoMode = ImGuizmo::LOCAL;

	bool isRenamingObject = false;
	char renameBuffer[128] = {};

	int selectedPostProcessEffect = 0;
	const char* effects[6] = {"None", "inversion", "grayscale", "sharpen", "blur", "edgeDetection"};

	void Init();
	void Update();
	void RenderUI();
	void GeneralPropertiesUI();
	FrameBufferInfo generateFrameBuffer();
	void DeletePointLightGameObject(GameObject* obj);
	void SetPostProcessEffect();

	bool defaultDockLayoutInitialized = false;
	void SetupDefaultDockLayout(ImGuiID dockID);
	std::string savedDockLayout;
	bool viewportFullscreen = false;

	//Input callback functions
	void processInput(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xposin, double yposin);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void setMouseCaptured(GLFWwindow* window, bool captured);
};