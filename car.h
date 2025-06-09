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
#include "terrain.h"

class Car
{
	void drawWheel(ShaderProgram* sp, glm::mat4 M, glm::vec3 posInScene, float car_speed, float rotation);

	float front_angle = 0.0f;
	float wheels_rot = 0.0f;

	glm::vec3 pos = glm::vec3(0, 0, 0);
	float angle = 0.0f;

	float rotation_speed = 0.0f;
	float speed = 0.0f;
	float acce = 0.0f;
	float drag = 0.96f;

	glm::mat4 M;

	glm::vec3 prev_pos;

public:
	static MeshData CarBase;
	static MeshData CarWheel;
	
	static GLuint baseTexture;
	static GLuint tintAreaTexture;
	static GLuint emissionTexture;

	static bool disco;
	static float disco_t;

	Car();

	void drawCar(ShaderProgram* sp, glm::mat4 V, glm::mat4 P);
	void updateCar(double t, Terrain& teren);
	void keyCallback(GLFWwindow* window);

	void setPos(glm::vec3 _pos);
	glm::vec3 getPos();
	float getAngle();

	void setAcceleration(float v);
	void setRotationSpeed(float v);
	float getRotationSpeed();
};

#endif
