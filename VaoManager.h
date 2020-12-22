#pragma once

#include <iostream>
#include <vector> 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.h"

class VaoManager {

	int vaoNum, vboNum, eboNum; 

	unsigned int *vao, *vbo, *ebo; 

	std::vector<Object*> bufferObjects; 

public: 
	VaoManager() { } 

	~VaoManager() {
		glDeleteVertexArrays(vaoNum, vao); 
		glDeleteBuffers(vboNum, vbo); 
		glDeleteBuffers(eboNum, ebo); 
	} 

	void addBufferObject(Object *obj) {
		bufferObjects.push_back(obj); 
		vaoNum = vboNum = eboNum = bufferObjects.size(); 
	}

	// generate vao 
	void genVao() {
		vao = new unsigned int[vaoNum]; 
		vbo = new unsigned int[vboNum]; 
		ebo = new unsigned int[eboNum]; 

		glGenVertexArrays(vaoNum, vao);
		glGenBuffers(vboNum, vbo);
		glGenBuffers(eboNum, ebo);

		int size = bufferObjects.size(); 
		for (int i = 0; i < size; i++) {
			std::vector<float>& vertices = bufferObjects[i]->getVertices();
			std::vector<int>& indices = bufferObjects[i]->getIndices();

			glBindVertexArray(vao[i]);
			glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
			if (indices.size() != 0) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
			}

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

	}

	void renderBufferObjects(Shader shader, Camera camera, int scrWidth, int scrHeight) {
		int size = bufferObjects.size();
		for (int i = 0; i < size; i++) {
			Object* obj = bufferObjects[i]; 
			obj->setShader(shader, camera, scrWidth, scrHeight); 
			obj->render(vao[i]); 
		}
	}
};
