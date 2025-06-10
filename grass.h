#pragma once

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
#include "camera.h"
#include <iostream>
#include <cmath>
#include <random>

struct GrassChunk {
	std::vector<glm::mat4> modelMatrices;
	std::vector<int> X;
	std::vector<int> Z;
	std::vector<int> Y;
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);

	void generate(Terrain& teren, int startX, int startZ, int GRASS_COUNT, int CHUNK_SIZE)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, CHUNK_SIZE);
		center = glm::vec3(startX + (CHUNK_SIZE / 2), teren.getHeight(startX + (CHUNK_SIZE / 2), startZ + (CHUNK_SIZE / 2)), startZ + (CHUNK_SIZE / 2));

		int gc2 = GRASS_COUNT * GRASS_COUNT;
		X.resize(gc2);
		Z.resize(gc2);
		Y.resize(gc2);
		modelMatrices.resize(gc2, glm::mat4(1.0f));
		for (int i = 0; i < GRASS_COUNT; i++)
		{
			
			for (int j = 0; j < GRASS_COUNT; j++)
			{
				X[i * GRASS_COUNT + j] = dist(gen) + startX;
				Z[i * GRASS_COUNT + j] = dist(gen) + startZ;
				Y[i * GRASS_COUNT + j] = teren.getHeight(X[i * GRASS_COUNT + j], Z[i * GRASS_COUNT + j]);
			}
		}
		// Generowanie Modelu
		for(int i = 0; i < gc2; i++){
			glm::mat4 M = glm::mat4(1.0f);
			M = glm::translate(M, glm::vec3(X[i], Y[i], Z[i]));
			glm::vec3 terrainNormal = teren.getTerrainNormal(X[i], Z[i]);
			glm::vec3 forward = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

			glm::vec3 right = glm::normalize(glm::cross(forward, terrainNormal));
			glm::vec3 adjustedForward = glm::normalize(glm::cross(terrainNormal, right));

			glm::mat4 rotationMatrix = glm::mat4(
				glm::vec4(right, 0.0),
				glm::vec4(terrainNormal, 0.0),
				glm::vec4(adjustedForward, 0.0),
				glm::vec4(0.0, 0.0, 0.0, 1.0)
			);

			M = M * rotationMatrix;
			M = glm::translate(M, glm::vec3(0, 5.0f, 0));
			M = glm::scale(M, glm::vec3(6, 6, 6));
			modelMatrices[i] = M;
		}
	}
	
};


class Grass
{
	int _GRASS_COUNT;
	GLuint _tex0, _tex0n;
	GLuint _tex1, _tex1n;
	int _CHUNKS;
	GrassChunk *gc;
	
public:
	Grass(int chunks_amount, int GC = 100);
	void init(Terrain& teren, GLuint& t0, GLuint& t0n, GLuint& t1, GLuint& t1n, int CHUNK_SIZE);
	void drawGrass(ShaderProgram* sp, Camera &camera, int maxDistance, int chunk_size);
};
