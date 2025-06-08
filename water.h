#pragma once
#ifndef WATER_CLASS_H
#define WATER_CLASS_H
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
#include "PerlinNoise.hpp"

class Water
{
	static constexpr int _SCALE = 20;
	static const unsigned int _N = 250;
	static const unsigned int _SIZE = 3*4*2* _N*_N;
	float _WaterVertices[_SIZE] {0.0f};
	float _WaterNormals[_SIZE]{ 0.0f };
	float y[_N + 2][_N + 2]{ -30.0f };

	static float _WaterTexCoords[_N * _N * 12];

	float waves = 0.0f;
	siv::PerlinNoise::seed_type seed = 123456u;
	siv::PerlinNoise _perlin{ seed };
public:
	Water();
	int getVerticesCount();
	void drawWater(ShaderProgram* sp, GLuint& tex0, GLuint& tex1, GLuint& tex2, float angle_x, float angle_y, float car_rot_angle, glm::mat4 V, glm::mat4 P, glm::vec3 camPos);

	float getHeight(float, float);
	glm::vec3 getTerrainNormal(float, float);
	void calculateVert();
};

#endif