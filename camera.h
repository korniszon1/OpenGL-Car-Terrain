#pragma once
#ifndef CAMERA_CLASS_H_
#define CAMERA_CLASS_H_

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


class Camera
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, -6.0f);
	glm::vec3 rotation = glm::vec3(4.0f, 0.0f, 1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 V = glm::mat4(1.0f);
	glm::mat4 P;
	float speed = 1.20f;
	float sensitivity = 0.2f;

	int width;
	int height;

	float distance = 20.0f;               // Distance from the target
	float yaw = 0.0f;                     // Horizontal angle (in degrees)
	float pitch = 20.0f;

public:
	glm::vec3 target = glm::vec3(0.0f);
	Camera(int, int, glm::vec3);
	void update_camera(float fovy, float aspect, ShaderProgram* sp, float near, float far);
	void update_position(float, float, float);
	void keyCallback(GLFWwindow* window);
	glm::mat4 getCameraView();
	glm::mat4 getCameraProj();
	glm::vec3 getPos();
	glm::vec3 getCameraFront();
	glm::vec3 getCameraUp();
	void updateOrbit();
	~Camera();


};



#endif

