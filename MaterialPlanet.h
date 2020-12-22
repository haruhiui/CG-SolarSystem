#pragma once

#include <iostream>
#include <vector> 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Planet.h"

class MaterialPlanet : public Planet {

public: 
	MaterialPlanet(
		float radius = 1.0f, glm::vec3 starPos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 orbit = glm::vec3(0.0f, 0.0f, 1.0f),
		float orbitRadius = 0.0f, float speed = 1.0f, float initAngle = 0.0f) :
		Planet(radius, starPos, orbit, orbitRadius, speed, initAngle) {

	}

	void setShader(const Shader& shader, const Camera& camera, int scrWidth, int scrHeight) {
		// vertex shader 
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		// camera/view transformation
		// glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 view = camera.getViewMatrix();
		shader.setMat4("view", view);

		// calculate the model matrix
		glm::mat4 model = glm::mat4(1.0f);		// make sure to initialize matrix to identity matrix first
		model = glm::translate(model, translateVec);
		model = glm::rotate(model, glm::radians(rotateDeg), rotateVec);
		model = glm::scale(model, scaleVec);
		shader.setMat4("model", model);

		// shader.setVec3("centerPos", getTranslateVec()); 

		// fragment shader 
		shader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.3f));
		shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		shader.setVec3("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("viewPos", camera.getPosition());
	}
};