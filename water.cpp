#include "water.h"
#include <cmath>


Water::Water()
{

	calculateVert();
}
void Water::calculateVert()
{

	const siv::PerlinNoise& perlin = _perlin;
	int x = 0;
	int z = 0;

	float a = 1.0f;
	const float height = 2.0f;
	const float curve = 5.0f;
	//waves += 200.0 * glfwGetTime();
	////const float heightFactor = (1.0f / (float)(_N * _N * _N * _N / (_N /( height))));
	//for (int i = 0; i < _N + 2; i++)
	//{
	//	for (int j = 0; j < _N + 2; j++)
	//	{
	//		const double noise = perlin.octave2D_01((i * 0.1 + waves), (j * 0.1 + waves), 40);
	//		y[i][j] = noise - 5.0f;

	//	}
	//}
	float tileRepeat = _N;
	for (int i = 0; i < _SIZE;)
	{
		if (i % (int)(24 * _N) == 0) { z++; x = 0; }

		_WaterVertices[i] = x;
		_WaterVertices[i + 1] = y[x][z];
		_WaterVertices[i + 2] = z;
		_WaterVertices[i + 3] = a;

		_WaterNormals[i] = x;
		_WaterNormals[i + 1] = y[x][z];
		_WaterNormals[i + 2] = z;

		i += 4;

		_WaterVertices[i] = ++x;
		_WaterVertices[i + 1] = y[x][z];
		_WaterVertices[i + 2] = z;
		_WaterVertices[i + 3] = a;

		_WaterNormals[i] = x;
		_WaterNormals[i + 1] = y[x][z];
		_WaterNormals[i + 2] = z;

		i += 4;

		_WaterVertices[i] = --x;
		_WaterVertices[i + 1] = y[x][++z];
		_WaterVertices[i + 2] = z;
		_WaterVertices[i + 3] = a;

		_WaterNormals[i] = x;
		_WaterNormals[i + 1] = y[x][z];
		_WaterNormals[i + 2] = z;

		i += 4;

		_WaterVertices[i] = _WaterVertices[i - 4];
		_WaterVertices[i + 1] = _WaterVertices[i - 3];
		_WaterVertices[i + 2] = _WaterVertices[i - 2];
		_WaterVertices[i + 3] = a;

		_WaterNormals[i] = _WaterNormals[i - 4];
		_WaterNormals[i + 1] = _WaterNormals[i - 3];
		_WaterNormals[i + 2] = _WaterNormals[i - 2];

		i += 4;

		_WaterVertices[i] = _WaterVertices[i - 12];
		_WaterVertices[i + 1] = _WaterVertices[i - 11];
		_WaterVertices[i + 2] = _WaterVertices[i - 10];
		_WaterVertices[i + 3] = a;

		_WaterNormals[i] = _WaterNormals[i - 12];
		_WaterNormals[i + 1] = _WaterNormals[i - 11];
		_WaterNormals[i + 2] = _WaterNormals[i - 10];


		i += 4;

		_WaterVertices[i] = ++x;
		_WaterVertices[i + 1] = y[x][z];
		_WaterVertices[i + 2] = z;
		_WaterVertices[i + 3] = a;

		_WaterNormals[i] = x;
		_WaterNormals[i + 1] = y[x][z];
		_WaterNormals[i + 2] = z;


		i += 4;

		z--;
	}
	for (int v = 0; v < _SIZE / 4; v++)
	{
		float vx = _WaterVertices[v * 4 + 0];
		float vz = _WaterVertices[v * 4 + 2];

		_WaterTexCoords[v * 2 + 0] = (vx / _N) * tileRepeat;
		_WaterTexCoords[v * 2 + 1] = (vz / _N) * tileRepeat;
	}
}
glm::vec3 computeNormal2(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	glm::vec3 u = b - a;
	glm::vec3 v = c - a;
	return glm::normalize(glm::cross(u, v));
}

void Water::drawWater(ShaderProgram *sp, GLuint &tex0, GLuint &tex1, GLuint& tex2, float angle_x, float angle_y, float car_rot_angle, glm::mat4 V, glm::mat4 P, glm::vec3 camPos)
{
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, 0.0f , glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
	M = glm::rotate(M, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::translate(M, glm::vec3(-(float)(_N*_SCALE/2), -250.0f, -(float)(_N * _SCALE / 2)));
	M = glm::scale(M, glm::vec3(_SCALE, _SCALE, _SCALE));
	sp->use();//Aktywacja programu cieniuj¹cego
	//Przeslij parametry programu cieniuj¹cego do karty graficznej



	
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	//glUniform4f(sp->u("lp"), 10, 0, 0, 1);

	//textura
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, tex2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex2);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	

	glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, _WaterVertices); //Wska¿ tablicê z danymi dla atrybutu vertex


	//glEnableVertexAttribArray(sp->a("color"));  //W³¹cz przesy³anie danych do atrybutu color
	//glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wska¿ tablicê z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, _WaterNormals); //Wska¿ tablicê z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, _WaterTexCoords);
	glEnableVertexAttribArray(sp->a("texCoord1"));
	glVertexAttribPointer(sp->a("texCoord1"), 2, GL_FLOAT, false, 0, _WaterTexCoords);
	glEnableVertexAttribArray(sp->a("texCoord2"));
	glVertexAttribPointer(sp->a("texCoord2"), 2, GL_FLOAT, false, 0, _WaterTexCoords);
	
	float time = (float)glfwGetTime();
	waves += 50.0f * time;
	//printf("time %f\n", time / 5);
	glUniform1f(sp->u("time"),waves);
	
	glUniform1f(sp->u("waveStrength"), 2.0f);


	glUniform1i(sp->u("textureMap0"), 0);
	glUniform1i(sp->u("textureMap1"), 1);
	glUniform1i(sp->u("skybox"), 2);

	//uniform vec3 pointLightPos;
	//uniform vec3 pointLightColor;
	glm::vec3 lightDirection = glm::normalize(glm::vec3((float)(_N * _SCALE) , 100.0f, -(float)(_N * _SCALE) )); // sun direction
	glUniform3fv(sp->u("lightDir"), 1, glm::value_ptr(lightDirection));

	glm::vec3 cameraPos = camPos;
	glUniform3fv(sp->u("viewPos"), 1, glm::value_ptr(cameraPos));

	//x -0.449507 y 0.238786 z -1.072379
	glm::vec3 lightOffset = glm::vec3(-5.0f, 7.5f, 0.0f);
	glm::vec3 carPos = glm::vec3(angle_x, getHeight(angle_x, angle_y), angle_y);
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), car_rot_angle, glm::vec3(0, 1, 0));
	glm::vec3 rotatedOffset = glm::vec3(rotation * glm::vec4(lightOffset, 1.0f));
	glm::vec3 carLightPos = carPos + rotatedOffset;
	//glm::vec3 carLightPos = glm::vec3(angle_x - 6.0f, getHeight(angle_x - 6.0f, angle_y - 3.0f) + 5.0f, angle_y - 3.0f);
	glUniform3fv(sp->u("pointLightPos"), 1, glm::value_ptr(carLightPos));
	glUniform3fv(sp->u("pointLightColor"), 1, glm::value_ptr(glm::vec3(245.0/255.0, 197.0/255.0, 66.0/255.0)));

	glm::mat4 lightView = V;
	glm::mat4 lightProjection = P; // lub perspective
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	glUniformMatrix4fv(sp->u("lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	glDrawArrays(GL_TRIANGLES, 0, getVerticesCount()); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wy³¹cz przesy³anie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
}



int Water::getVerticesCount()
{
	return (int)_SIZE/4;
}

float barryCentric2(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}


float Water::getHeight(float pos_x, float pos_z)
{
	
	float result;

	float gridX = pos_x / _SCALE;
	float gridZ = pos_z / _SCALE;

	int x = (int)floor(gridX);
	int z = (int)floor(gridZ);

	float blockX = gridX - x;
	float blockZ = gridZ - z;

	if (blockX <= 1 - blockZ)
	{
		result = barryCentric2(
			glm::vec3(0.0f, y[x][z], 0.0f),
			glm::vec3(1.0f, y[x + 1][z], 0.0f),
			glm::vec3(0.0f, y[x][z + 1], 1.0f),
			glm::vec2(blockX, blockZ)
		);
	}
	else
	{
		result = barryCentric2(
			glm::vec3(1.0f, y[x + 1][z], 0.0f),
			glm::vec3(1.0f, y[x + 1][z + 1], 1.0f),
			glm::vec3(0.0f, y[x][z + 1], 1.0f),
			glm::vec2(blockX, blockZ)
		);
	}

	//printf("h: %f %f\n", result, pos_x - (int)floor(pos_x));
	return result * _SCALE;
}

glm::vec3 Water::getTerrainNormal(float x, float z) {
	float dx = 0.5f;
	float dz = 0.5f;

	float hL = getHeight(x - dx, z);
	float hR = getHeight(x + dx, z);
	float hD = getHeight(x, z - dz);
	float hU = getHeight(x, z + dz);

	glm::vec3 dxVec = glm::vec3(2.0f * dx, hR - hL, 0.0f);
	glm::vec3 dzVec = glm::vec3(0.0f, hU - hD, 2.0f * dz);

	glm::vec3 normal = glm::normalize(glm::cross(dzVec, dxVec));
	return normal;
}

float Water::_WaterTexCoords[] = {
		3.0f, 3.0f, 0.0f, 0.0f, 0.0f, 3.0f,
		3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f
};
