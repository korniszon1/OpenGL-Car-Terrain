#pragma once
#ifndef SKYBOX_CLASS_H
#define SKYBOX_CLASS_H
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

class Skybox
{
	GLuint skyboxVAO, skyboxVBO;
public:
	void drawSkybox(ShaderProgram* sp, GLuint cubemapTexture, glm::mat4 view, glm::mat4 projection);
	void initSkybox();

};
#endif


