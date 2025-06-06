#pragma once
#ifndef TERRAIN_CLASS_H
#define TERRAIN_CLASS_H
#include <GL/glew.h>
#include <vector>
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
	static constexpr int _SCALE = 5;
	static const unsigned int _N = 200;
	static const unsigned int _SIZE = 3*4*2* _N*_N;
	float _TerrainVertices[_SIZE] {0.0f};
	float _TerrainNormals[_SIZE]{ 0.0f };
	float y[_N + 2][_N + 2]{ 0.0f };

	static float _TerrainTexCoords[_N * _N * 12];
public:
	Terrain();
	void showVertices();
	float* getVertices();
	float* getNormals();
	int getVerticesCount();
	void drawTerrain(ShaderProgram* sp, GLuint& tex0, GLuint& tex1, float angle_x, float angle_y, glm::mat4 V, glm::mat4 P, glm::vec3 camPos);

	float getHeight(float, float);
	glm::vec3 getTerrainNormal(float, float);
	
};

#endif