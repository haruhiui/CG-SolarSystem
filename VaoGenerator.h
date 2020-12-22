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
#include "Sphere.h"

class VaoGenerator {

	static int sVaoNum; 
	static int sVboNum; 
	static int sEboNum; 

	int vaoNum, vboNum, eboNum; 

	unsigned int vao, vbo, ebo; 

	std::vector<Object*> bufferObjects; 

public: 

	VaoGenerator() {
		vaoNum = sVaoNum++; 
		vboNum = sVboNum++; 
		eboNum = sEboNum++; 
		glGenVertexArrays(vaoNum, &vao); 
		glGenBuffers(vboNum, &vbo); 
		glGenBuffers(eboNum, &ebo); 
	}

	~VaoGenerator() {
		glDeleteVertexArrays(vaoNum, &vao);
		glDeleteBuffers(vboNum, &vbo);
		glDeleteBuffers(eboNum, &ebo);
	}

	void addBufferObject(Object* obj) {
		bufferObjects.push_back(obj); 
	}

	unsigned int genObjectsVao(int vsLoc) {
		glBindVertexArray(vao); 

		for (std::vector<Object*>::iterator iter = bufferObjects.begin(); iter != bufferObjects.end(); iter++) {
			const std::vector<float>& vertices = (*iter)->getVertices(); 
			const std::vector<int>& indices = (*iter)->getIndices();
			
			glBindBuffer(GL_ARRAY_BUFFER, vbo); 
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW); 
			if (indices.size() != 0) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); 
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW); 
			}

			glVertexAttribPointer(vsLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
			glEnableVertexAttribArray(0); 

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0); 
		}

		return vao; 
	}
};

int VaoGenerator::sVaoNum = 1;
int VaoGenerator::sVboNum = 1;
int VaoGenerator::sEboNum = 1; 