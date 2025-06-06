#pragma once
#ifndef CAR_CLASS_H
#define CAR_CLASS_H

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

#include "model_loader.h"

class Car
{
private:
	void drawWheel(ShaderProgram* sp, glm::mat4 M, glm::vec3 posInScene, float rotation);

public:
	static MeshData CarBase;
	static MeshData CarWheel;

	static float t;

	Car();
	void drawCar(ShaderProgram* sp, glm::mat4 V, glm::mat4 P, GLuint& tex0, GLuint& tex1, float pos_x, float pos_y, float pos_z, glm::vec3 terrainNormal);
};

#endif
