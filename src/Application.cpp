#define STB_IMAGE_IMPLEMENTATION
#include "Application.h"

Application::Application(unsigned int windowWidth, unsigned int windowHeight, const char* appName)
	:windowWidth(windowWidth), windowHeight(windowHeight), appName(appName)
{
	Init();
}

Application::~Application()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void Application::Init()
{
	//GLFW initialization options
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create GLFW window
	window = glfwCreateWindow(windowWidth, windowHeight, appName, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //setting glViewport for Resizing window
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//Initializing GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //capture cursor

	//set shaders
	mainShader = std::make_unique<Shader>(RESOURCES_PATH"modelLoading.vert", RESOURCES_PATH"modelLoading.frag");
	lightCubeShader = std::make_unique<Shader>(RESOURCES_PATH"LightCubeShader.vert", RESOURCES_PATH"LightCubeShader.frag");

	//flip textures
	stbi_set_flip_vertically_on_load(true);

	//setup imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//setup scene
	scene.Setup(*mainShader, *lightCubeShader, lighting);
}

void Application::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		//set delta Time
		float currentframe = glfwGetTime();
		deltaTime = currentframe - lastFrame;
		lastFrame = currentframe;

		//Input
		processInput(window);

		//clear background
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//set lighting parameters in shaders
		lighting.SetShaderParameters(*mainShader, *lightCubeShader, camera);
		
		//calculate projection matrix
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)windowHeight,
			0.1f, 100.0f);

		//render scene
		scene.Draw(camera, projection);

		//draw ui using imgui
		RenderUI();

		//Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::RenderUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Tools");

	ImGui::ColorEdit3("clear color", &clearColor[0]);
	lighting.SetImGuiLightingParameters();

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::processInput(GLFWwindow* window)
{
	if (mouseCaptured)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(UP, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(DOWN, deltaTime);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressedLastFrame)
	{
		setMouseCaptured(window, !mouseCaptured);
	}
	spacePressedLastFrame = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void Application::setMouseCaptured(GLFWwindow* window, bool captured)
{
	if (captured)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	mouseCaptured = captured;
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Application::mouse_callback(GLFWwindow* window, double xposin, double yposin)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

	float xPos = static_cast<float>(xposin);
	float yPos = static_cast<float>(yposin);

	if (app->first_mouse)
	{
		app->lastx = xPos;
		app->lasty = yPos;
		std::cout << "ye\n";
		app->first_mouse = false;
	}

	float xoffset = xPos - app->lastx;
	float yoffset = app->lasty - yPos;
	app->lastx = xPos;
	app->lasty = yPos;

	if (app->mouseCaptured)
		app->camera.ProcessMouseMovement(xoffset, yoffset);
}

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	if (app->mouseCaptured)
		app->camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


