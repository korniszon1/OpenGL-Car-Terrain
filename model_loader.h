#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <iostream> // debug purposes

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct MeshData {
	std::vector<std::vector<glm::vec4>> verts;
	std::vector<std::vector<glm::vec4>> norms;
	std::vector<std::vector<glm::vec2>> tcoords;
	std::vector<std::vector<unsigned int>> indices;
	glm::vec3 center;
};

MeshData loadModel(std::string filename);
