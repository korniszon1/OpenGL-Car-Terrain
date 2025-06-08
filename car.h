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
	void drawWheel(ShaderProgram* sp, glm::mat4 M, glm::vec3 posInScene, float car_speed, float rotation);
	float front_rotation = 0.0f;
	float wheelRot = 0.0f;
	float y = -200.0f;
	float gravity = 0.02;
	glm::mat4 M;
public:
	static MeshData CarBase;
	static MeshData CarWheel;

	static float t;

	Car();
	void drawCar(ShaderProgram* sp, glm::mat4 V, glm::mat4 P, GLuint& tex0, GLuint& tex1, GLuint& tex2, float angle_x, float pos_x, float pos_y, float pos_z,float car_speed, glm::vec3 terrainNormal, bool disco);
	void keyCallback(GLFWwindow* window);
	glm::vec3 getCameraPos();
};

#endif
