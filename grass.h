#pragma once
#ifndef GRASS_CLASS_H
#define GRASS_CLASS_H
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
#include "terrain.h"
#include <iostream>
#include <cmath>
#include <random>

class Grass
{
	std::random_device _rd;
	std::mt19937 _gen;
	std::uniform_int_distribution<> _dist;

	int _GRASS_COUNT;
	std::vector<int> _randomNumberX;
	std::vector<int> _randomNumberZ;
	std::vector<int> _randomNumberY;
	std::vector<glm::mat4> _modelMatrices;
	GLuint _tex0, _tex0n;
	GLuint _tex1, _tex1n;
public:
	Grass(int minXZ, int maxXZ, int GC = 100);
	void init(Terrain &teren, GLuint& t0, GLuint& t0n, GLuint& t1, GLuint& t1n);
	void drawGrass(ShaderProgram* sp, glm::vec3 cameraPos, int maxDistance = 50);
};



#endif