#pragma once

#include <iostream> 
#include <vector> 

#include "Object.h"
#include "Window.h"

const float PI = 3.14159265358979323846f; 

class Sphere: public Object {

protected: 
	int xSeg; 
	int ySeg; 

public: 
	Sphere(int xSeg = 50, int ySeg = 50): xSeg(xSeg), ySeg(ySeg) {
		// vertices 
		for (int y = 0; y <= ySeg; y++) {
			for (int x = 0; x <= xSeg; x++) {
				float xSegment = (float)x / (float)xSeg;
				float ySegment = (float)y / (float)ySeg;
				float xPosition = std::sin(ySegment * PI) * std::cos(xSegment * 2.0f * PI);
				float yPosition = std::cos(ySegment * PI);
				float zPosition = std::sin(ySegment * PI) * std::sin(xSegment * 2.0f * PI);

				vertices.push_back(xPosition);
				vertices.push_back(yPosition);
				vertices.push_back(zPosition);
			}
		}
		// indices 
		for (int i = 0; i < ySeg; i++) {
			for (int j = 0; j < xSeg; j++) {
				indices.push_back(i * (xSeg + 1) + j);
				indices.push_back((i + 1) * (xSeg + 1) + j);
				indices.push_back((i + 1) * (xSeg + 1) + j + 1);

				indices.push_back(i * (xSeg + 1) + j);
				indices.push_back((i + 1) * (xSeg + 1) + j + 1);
				indices.push_back(i * (xSeg + 1) + j + 1);
			}
		}
	}

	~Sphere() {
	}

	int getXSeg() {
		return xSeg; 
	}

	int getYSeg() {
		return ySeg; 
	}

	void render(unsigned int vao) {
		//开启面剔除(只需要展示一个面，否则会有重合)
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBindVertexArray(vao);
		//使用线框模式绘制
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, xSeg * ySeg * 6, GL_UNSIGNED_INT, 0);
	}
};