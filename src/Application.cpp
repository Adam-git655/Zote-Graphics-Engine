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

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	//set shaders
	mainShader = std::make_unique<Shader>(RESOURCES_PATH"modelLoading.vert", RESOURCES_PATH"modelLoading.frag");
	lightCubeShader = std::make_unique<Shader>(RESOURCES_PATH"LightCubeShader.vert", RESOURCES_PATH"LightCubeShader.frag");
	screenShader = std::make_unique<Shader>(RESOURCES_PATH"BasicScreenShader.vert", RESOURCES_PATH"BasicScreenShader.frag");

	//flip textures
	stbi_set_flip_vertically_on_load(true);

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//generate frame buffer info and screen quad to render scene onto
	frameBufferInfo = generateFrameBuffer();
	screenQuadVAO = Primitives::createScreenQuadVAO();

	//setup imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	//setup scene
	scene.Setup(*mainShader, *lightCubeShader, lighting, camera);
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
		
		//first pass on offscreen framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferInfo.frameBuffer);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//render scene
		scene.Draw(camera, projection, lighting, *mainShader);

		//second pass on screen (default main window framebuffer, render imgui on here)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		//draw ui using imgui
		RenderUI();

		//Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::SetupDefaultDockLayout(ImGuiID dockID)
{
	if (dockLayoutInitialized) return;
	dockLayoutInitialized = true;

	//clear any existing layout
	ImGui::DockBuilderRemoveNode(dockID);
	ImGui::DockBuilderAddNode(dockID, ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::DockBuilderSetNodeSize(dockID, ImGui::GetMainViewport()->Size);

	//split right
	ImGuiID dockRight;
	ImGuiID dockViewport;
	ImGui::DockBuilderSplitNode(dockID, ImGuiDir_Right, 0.2f, &dockRight, &dockViewport);

	//split right into two
	ImGuiID dockRightUp;
	ImGuiID dockRightBottom;
	ImGui::DockBuilderSplitNode(dockRight, ImGuiDir_Down, 0.6f, &dockRightBottom, &dockRightUp);

	//assign windows
	ImGui::DockBuilderDockWindow("Scene", dockRightUp);
	ImGui::DockBuilderDockWindow("Viewport", dockViewport);
	ImGui::DockBuilderDockWindow("Inspector", dockRightBottom);

	ImGui::DockBuilderFinish(dockID);
}


void Application::RenderUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//make transparent background "viewport" covering whole window as a base
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("DockSpace", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground
	);
	ImGui::PopStyleVar();

	//create main dockspace where windows will be docked on top of transparent window
	ImGuiID dockID = ImGui::GetID("MainDockSpace");
	ImGui::DockSpace(dockID, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();

	SetupDefaultDockLayout(dockID);

	//if not full screen render scene and inspector
	if (!viewportFullscreen)
	{
		ImGui::Begin("Scene");

		for (auto& obj : scene.getCurrentGameObjects())
		{
			bool selected = false;
			if (scene.selectedObject == obj.get())
				selected = true;

			if (ImGui::Selectable(obj->name.c_str(), selected))
				scene.selectedObject = obj.get();
		}

		ImGui::End();

		ImGui::Begin("Inspector");
		ImGui::ColorEdit3("clear color", &clearColor[0]);

		if (scene.selectedObject)
		{
			std::string objectName = scene.selectedObject->name;

			ImGui::Text(objectName.c_str());
			ImGui::Separator();

			//properties of light objects
			if (scene.selectedObject->tag == "directional_light")
			{
				lighting.SetImGuiLightingParametersDirectional();
			}

			else if (scene.selectedObject->tag == "point_light")
			{
				GeneralPropertiesUI(); //show general properties like transform data for point light as well
				ImGui::Separator();

				int lightIndex = objectName[objectName.size() - 1] - '0';
				lighting.SetImGuiLightingParametersPoint(lightIndex);
			}

			else if (scene.selectedObject->tag == "spot_light")
			{
				lighting.SetImGuiLightingParametersSpot();
			}

			//properties of all other objects
			else
			{
				GeneralPropertiesUI();
			}
		}
		else
		{
			ImGui::Text("No object selected");
		}

		//lighting.SetImGuiLightingParameters();
		ImGui::End();
	}

	//always render viewport
	if (viewportFullscreen)
	{
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(vp->Pos);
		ImGui::SetNextWindowSize(vp->Size);
		ImGui::SetNextWindowViewport(vp->ID);
		ImGui::Begin("Viewport", nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoDocking
		);
	}
	else
	{
		ImGui::Begin("Viewport");
	}

	ImVec2 size = ImGui::GetContentRegionAvail();
	ImGui::Image((ImTextureID)(intptr_t)frameBufferInfo.textureColorBuffer, size, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::GeneralPropertiesUI()
{
	ImGui::DragFloat3("Position", &scene.selectedObject->transform.position[0], 0.1f);
	ImGui::DragFloat3("Rotation", &scene.selectedObject->transform.rotation[0], 0.1f);
	ImGui::DragFloat3("Scale", &scene.selectedObject->transform.scale[0], 0.1f);
	ImGui::Separator();
	ImGui::ColorEdit3("Color", &scene.selectedObject->color[0]);
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

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fPressedLastFrame)
	{
		viewportFullscreen = !viewportFullscreen;
		if (!viewportFullscreen)
			dockLayoutInitialized = false;
	}
	fPressedLastFrame = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
}

FrameBufferInfo Application::generateFrameBuffer()
{
	//create frame buffer
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	//generate texture to store color buffer 
	unsigned textureColorBuffer;
	glGenTextures(1, &textureColorBuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//attach texture to frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	//create render buffer to store depth buffer
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attach render buffer object to frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

	//check if frame buffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR: Frame buffer is not complete\n";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FrameBufferInfo frameBufferInfo{ fbo, textureColorBuffer };
	return frameBufferInfo;
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


