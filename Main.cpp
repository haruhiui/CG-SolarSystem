
#include <iostream>
#include <vector> 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Window.h"
#include "Object.h"
#include "Sphere.h"
#include "VaoManager.h"
#include "Planet.h"
#include "MaterialPlanet.h"

void frameBufferSizeCallback(GLFWwindow* glfwWindow, int width, int height); 
void mouseCallback(GLFWwindow* glfwWindow, double xpos, double ypos); 
void scrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset);
void processInput(GLFWwindow *glfwWindow);

// self-defined Window 
Window window;

int main()
{
	window.setFrameBufferSizeCallback(frameBufferSizeCallback);
	window.setCursorPosCallback(mouseCallback);
	window.setScrollCallback(scrollCallback);
	window.setProcessInput(processInput); 

	// ------------------------------------------------------------------------------------------
	Shader lightShader("Light.vs", "Light.fs"); 
	Shader materialShader("Material.vs", "Material.fs"); 

	Planet sun(0.6f); 
	//MaterialPlanet planets[4] = {
	//	// radius, starPos, orbit, orbitRadius, speed, initAngle
	//	MaterialPlanet(0.3f, sun.getTranslateVec(), glm::vec3(0.0f, 1.0f, 0.0f), 1.5f, 1.0f, 90.0f),
	//	MaterialPlanet(0.4f, sun.getTranslateVec(), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 1.5f, 10.0f),
	//	MaterialPlanet(0.2f, sun.getTranslateVec(), glm::vec3(0.0f, 1.0f, 0.0f), 3.4f, 2.4f, 20.0f)
	//}; 

	MaterialPlanet planets[4] = {
		// radius, starPos, orbit, orbitRadius, speed, initAngle
		MaterialPlanet(0.3f, sun.getTranslateVec(), glm::vec3(0.0f, 1.0f, 0.0f), 1.5f, 0.0f, 0.0f),
		MaterialPlanet(0.4f, sun.getTranslateVec(), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f, 0.0f, 0.0f),
		MaterialPlanet(0.2f, sun.getTranslateVec(), glm::vec3(0.0f, 1.0f, 0.0f), 3.4f, 0.0f, 0.0f)
	};

	MaterialPlanet satellite[2] = {
		MaterialPlanet(0.14f, planets[1].getTranslateVec(), glm::vec3(0.0f, -1.0f, 0.2f), 1.0f, 5.0f, 0.0f),
		MaterialPlanet(0.1f, planets[1].getTranslateVec(), glm::vec3(1.0f, -1.0f, 0.0f), 0.9f, 6.0f, 0.0f)
	}; 

	planets[1].addSatellite(&satellite[0]); 
	planets[1].addSatellite(&satellite[1]); 

	VaoManager vaoManager1, vaoManager; 
	//vaoManager.addBufferObject(&p1); 
	vaoManager1.addBufferObject(&sun); 
	vaoManager.addBufferObject(&planets[0]); 
	vaoManager.addBufferObject(&planets[1]); 
	vaoManager.addBufferObject(&planets[2]); 
	vaoManager.addBufferObject(&satellite[0]); 
	vaoManager.addBufferObject(&satellite[1]); 

	vaoManager1.genVao(); 
	vaoManager.genVao(); 
	
	// ------------------------------------------------------------------------------------------

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window.glfwWindow)) {
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		window.deltaTime = currentFrame - window.lastFrame;
		window.lastFrame = currentFrame;

		// input
		// -----
		processInput(window.glfwWindow);

		// render 
		// ------ 
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// ------------------------------------------------------------------------------------------
		// activate shader
		lightShader.use(); 
		vaoManager1.renderBufferObjects(lightShader, window.camera, window.SCR_WIDTH, window.SCR_HEIGHT);

		materialShader.use(); 
		vaoManager.renderBufferObjects(materialShader, window.camera, window.SCR_WIDTH, window.SCR_HEIGHT);

		// ------------------------------------------------------------------------------------------

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window.glfwWindow);
		glfwPollEvents();
	} 

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *glfwWindow) {
	if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glfwWindow, true);

	if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
		window.camera.processKeyboard(FORWARD, window.deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
		window.camera.processKeyboard(BACKWARD, window.deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
		window.camera.processKeyboard(LEFT, window.deltaTime);
	if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
		window.camera.processKeyboard(RIGHT, window.deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void frameBufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouseCallback(GLFWwindow* glfwWindow, double xpos, double ypos) {
	if (window.firstMouse) {
		window.lastX = xpos;
		window.lastY = ypos;
		window.firstMouse = false;
	}

	float xoffset = xpos - window.lastX;
	float yoffset = window.lastY - ypos; // reversed since y-coordinates go from bottom to top
	window.lastX = xpos;
	window.lastY = ypos;

	window.camera.processMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset) {
	window.camera.processMouseScroll(yoffset);
}
