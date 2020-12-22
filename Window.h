#pragma once

#include <iostream>
#include <vector> 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Object.h" 

class Window {

public:
	GLFWwindow *glfwWindow; 
	Camera camera; 

	// screen 
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	// mouse move 
	bool firstMouse = true;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	// process input function 
	void(*processInput)(GLFWwindow *); 

public: 
	Window(): camera(Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f))) {

		// glfw: initialize and configure
		// ------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		// glfw window creation
		// --------------------
		glfwWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
		if (glfwWindow == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(-1);
		}
		// 
		glfwMakeContextCurrent(glfwWindow);

		// tell GLFW to capture our mouse
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD" << std::endl;
			exit(-1); 
		}

		// configure global opengl state
		// -----------------------------
		glEnable(GL_DEPTH_TEST);
	}

	~Window() {
		// glfw: terminate, clearing all previously allocated GLFW resources.
		// ------------------------------------------------------------------
		glfwTerminate();
	} 

	void setFrameBufferSizeCallback(GLFWframebuffersizefun frameBufferSizeCallback) const {
		glfwSetFramebufferSizeCallback(glfwWindow, frameBufferSizeCallback);
	}

	void setCursorPosCallback(GLFWcursorposfun mouseCallback) const {
		glfwSetCursorPosCallback(glfwWindow, mouseCallback); 
	}

	void setScrollCallback(GLFWscrollfun scrollCallback) const {
		glfwSetScrollCallback(glfwWindow, scrollCallback); 
	}

	void setProcessInput(void(*processInput)(GLFWwindow *)) {
		this->processInput = processInput; 
	}



};
