#pragma once
#ifndef CAR_CLASS_H
#define CAR_CLASS_H
#include <tiny_gltf.h>
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

class Car
{

public:
	Car();
	void drawCar(ShaderProgram* sp, GLuint& tex0, GLuint& tex1, float pos_x, float pos_y, float pos_z);
};
#endif