#pragma once
#ifndef TERRAIN_CLASS_H
#define TERRAIN_CLASS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "shaderprogram.h"
#include <glm/gtx/rotate_vector.hpp>


class Terrain
{
	static const int _N = 100;
	static const int _SIZE = 3*4*2* _N;
	float _TerrainVertices[_SIZE] {0.0f};
	float _TerrainNormals[_SIZE]{ 0.0f };

public:
	Terrain();
	void showVertices();
	float* getVertices();
	float* getNormals();
	int getVerticesCount();

protected:
	//void LoadHeightMapFile(const char* pFilename);

};

#endif