#pragma once

#include <iostream> 
#include <vector> 

#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Window.h"

// vertices of the Object(local space), and position of the Object(from local space to world space) 
class Object {

protected:
	// 
	std::vector<float> vertices;
	std::vector<int> indices;

	// 
	glm::vec3 scaleVec;
	glm::vec3 rotateVec;
	glm::vec3 translateVec;
	float rotateDeg;

public: 
	Object() {
		scaleVec = glm::vec3(1.0f, 1.0f, 1.0f); 
		rotateVec = glm::vec3(0.0f, 0.0f, 1.0f); 
		translateVec = glm::vec3(0.0f, 0.0f, 0.0f); 
		rotateDeg = 0.0f; 
	}

	~Object() {
	}

	std::vector<float>& getVertices() {
		return vertices;
	}

	std::vector<int>& getIndices() {
		return indices;
	}

	glm::vec3 getScaleVec() {
		return scaleVec; 
	}

	glm::vec3 getRotateVec() {
		return rotateVec; 
	}

	glm::vec3 getTranslateVec() {
		return translateVec; 
	}
	
	void setScaleVec(glm::vec3 svec) {
		scaleVec = svec; 
	}

	void setRotateVec(glm::vec3 rvec) {
		rotateVec = rvec; 
	}

	void setTranslateVec(glm::vec3 tvec) {
		translateVec = tvec; 
	}

	void setRotateDeg(float rdeg) {
		rotateDeg = rdeg;
	}

	virtual void setShader(const Shader& shader, const Camera& camera, int scrWidth, int scrHeight) {
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
	}

	virtual void render(unsigned int vao) = 0; 

};

