#pragma once

#include <iostream>
#include <vector> 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Sphere.h"

class Planet : public Sphere {

private: 
	float lastTime = 0.0f, deltaTime = 0.0f; 
	// basePos is the base position(orbitAngle == 0.0f) of planet relative to starPos 
	// we can calculate basePos by: 'starPos + orbitRadius * orbit x (0, 0, 1)' ((1, 0, 0) if orbit is parallel to (0, 0, 1))
	glm::vec3 basePos;	

	std::vector<Planet*> satellites;

protected: 
	glm::vec3 starPos;

	float radius;

	glm::vec3 orbit; 
	float orbitAngle; 
	float orbitRadius; 
	float speed; 

public: 
	Planet(
		float radius = 1.0f, glm::vec3 starPos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 orbit = glm::vec3(0.0f, 0.0f, 1.0f),
		float orbitRadius = 0.0f, float speed = 1.0f, float initAngle = 0.0f) :
		Sphere(), radius(radius), starPos(starPos), orbit(orbit),
		orbitRadius(orbitRadius), speed(speed), orbitAngle(initAngle) {

		assert(orbit != glm::vec3(0.0f, 0.0f, 0.0f)); 

		// initialize position 
		basePos = glm::cross(orbit, glm::vec3(0.0f, 0.0f, 1.0f)); 
		if (basePos == glm::vec3(0.0f, 0.0f, 0.0f)) {
			basePos = glm::cross(orbit, glm::vec3(1.0f, 0.0f, 0.0f));	// in case orbit that orbit is parallel to z 
		}
		basePos = orbitRadius * glm::normalize(basePos);

		updatePosition(); 
	}

	~Planet() {

	}

	void addSatellite(Planet *obj) {
		satellites.push_back(obj); 
	}

	void setStarPos(glm::vec3 star) {
		starPos = star; 
	}

	void updatePosition() {
		float currTime = glfwGetTime(); 
		deltaTime = currTime - lastTime; 
		lastTime = currTime; 

		orbitAngle += speed * deltaTime; 
		glm::mat4 rotMat = glm::mat4(1.0f); 
		rotMat = glm::rotate(rotMat, orbitAngle, orbit); 
		glm::vec3 relativePos = rotMat * glm::vec4(basePos, 1.0f); 
		
		setTranslateVec(starPos + relativePos);

		setScaleVec(glm::vec3(radius)); 

		// update the starPos of the satellites 
		for (std::vector<Planet*>::iterator iter = satellites.begin(); iter != satellites.end(); iter++) {
			(*iter)->setStarPos(starPos + relativePos);	// ??? 
		}
	}

	void render(unsigned int vao) {
		glBindVertexArray(vao);
		// 使用线框模式绘制
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, xSeg * ySeg * 6, GL_UNSIGNED_INT, 0);

		updatePosition(); 
	}

};