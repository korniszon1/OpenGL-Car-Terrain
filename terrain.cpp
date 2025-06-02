#include "terrain.h"
#include <cmath>
#include "PerlinNoise.hpp"

Terrain::Terrain()
{
	const siv::PerlinNoise::seed_type seed = 123456u;

	const siv::PerlinNoise perlin{ seed };



	int x = 0;
	int z = 0;
	float y[_N + 2][_N + 2]{0.0f};
	float a = 1.0f;
	//WIP
	// Im mniejsze tym wieksze
	const float height = 2.0f;
	const float curve = 5.0f;
	const float heightFactor = (1.0f / (float)(_N * _N * _N * _N / (_N /( height))));
	for (int i = 0; i < _N +2; i++)
	{
		for (int j = 0; j < _N + 2; j++)
		{
			const double noise = perlin.octave2D_01((i * 0.01), (j * 0.01), 4);
			float zeroI = _N + 2 - i;
			float zeroJ = _N+2 - j;
			y[i][j] = noise * heightFactor * i * i * zeroI * zeroI * heightFactor * j * j * zeroJ * zeroJ - curve*10;

		}
	}
	
	
	/*for (int i = 0; i < _SIZE; i = i + 4)
	{
		if (i % (int)(24 * _N) == 0) { y++; x = 0; }
		_TerrainVertices[i] = x;
		_TerrainVertices[i + 1] = y;
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y;
		_TerrainNormals[i + 2] = z[x][y];

		i = i + 4;
		
		_TerrainVertices[i] = ++x;
		_TerrainVertices[i + 1] = y;
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y;
		_TerrainNormals[i + 2] = z[x][y];

		i = i + 4;

		_TerrainVertices[i] = --x;
		_TerrainVertices[i + 1] = ++y;
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y;
		_TerrainNormals[i + 2] = z[x][y];

		i = i + 4;
		
		_TerrainVertices[i] = _TerrainVertices[i - 4];
		_TerrainVertices[i + 1] = _TerrainVertices[i - 3];
		_TerrainVertices[i + 2] = _TerrainVertices[i - 2];
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = _TerrainNormals[i - 4];
		_TerrainNormals[i + 1] = _TerrainNormals[i - 3];
		_TerrainNormals[i + 2] = _TerrainNormals[i - 2];

		i = i + 4;

		_TerrainVertices[i] = _TerrainVertices[i - 12];
		_TerrainVertices[i + 1] = _TerrainVertices[i - 11];
		_TerrainVertices[i + 2] = _TerrainVertices[i - 10];
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = _TerrainNormals[i - 12];
		_TerrainNormals[i + 1] = _TerrainNormals[i - 11];
		_TerrainNormals[i + 2] = _TerrainNormals[i - 10];

		i = i + 4;

		_TerrainVertices[i] = ++x;
		_TerrainVertices[i + 1] = y;
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y;
		_TerrainNormals[i + 2] = z[x][y];
		y--;

	}*/

	for (int i = 0; i < _SIZE; i = i + 4)
	{
		if (i % (int)(24 * _N) == 0) { z++; x = 0; }
		_TerrainVertices[i] = x;
		_TerrainVertices[i + 1] = y[x][z];
		_TerrainVertices[i + 2] = z;
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y[x][z];
		_TerrainNormals[i + 2] = z;

		i = i + 4;

		_TerrainVertices[i] = ++x;
		_TerrainVertices[i + 1] = y[x][z];
		_TerrainVertices[i + 2] = z;
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y[x][z];
		_TerrainNormals[i + 2] = z;

		i = i + 4;

		_TerrainVertices[i] = --x;
		_TerrainVertices[i + 1] = y[x][++z];
		_TerrainVertices[i + 2] = z;
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y[x][z];
		_TerrainNormals[i + 2] = z;

		i = i + 4;

		_TerrainVertices[i] = _TerrainVertices[i - 4];
		_TerrainVertices[i + 1] = _TerrainVertices[i - 3];
		_TerrainVertices[i + 2] = _TerrainVertices[i - 2];
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = _TerrainNormals[i - 4];
		_TerrainNormals[i + 1] = _TerrainNormals[i - 3];
		_TerrainNormals[i + 2] = _TerrainNormals[i - 2];

		i = i + 4;

		_TerrainVertices[i] = _TerrainVertices[i - 12];
		_TerrainVertices[i + 1] = _TerrainVertices[i - 11];
		_TerrainVertices[i + 2] = _TerrainVertices[i - 10];
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = _TerrainNormals[i - 12];
		_TerrainNormals[i + 1] = _TerrainNormals[i - 11];
		_TerrainNormals[i + 2] = _TerrainNormals[i - 10];

		i = i + 4;

		_TerrainVertices[i] = ++x;
		_TerrainVertices[i + 1] = y[x][z];
		_TerrainVertices[i + 2] = z;
		_TerrainVertices[i + 3] = a;

		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y[x][z];
		_TerrainNormals[i + 2] = z;
		z--;
	}


	/*1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,*/
	const float cube_wall[12] = {	1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
									1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < _N * _N ; i++)
	{
		for (int j = 0; j < 12; j++)_TerrainTexCoords[i*12+j] = cube_wall[j];
	}


}

void Terrain::drawTerrain(ShaderProgram *sp, GLuint &tex0, GLuint &tex1, float angle_x, float angle_y)
{
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle_y , glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
	M = glm::rotate(M, angle_x, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::scale(M, glm::vec3(_SCALE, _SCALE, _SCALE));
	sp->use();//Aktywacja programu cieniuj¹cego
	//Przeslij parametry programu cieniuj¹cego do karty graficznej




	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glUniform4f(sp->u("lp"), 0, 0, -6, 1);

	//textura
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);


	glEnableVertexAttribArray(sp->a("vertex"));  //W³¹cz przesy³anie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, _TerrainVertices); //Wska¿ tablicê z danymi dla atrybutu vertex

	//glEnableVertexAttribArray(sp->a("color"));  //W³¹cz przesy³anie danych do atrybutu color
	//glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wska¿ tablicê z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, _TerrainNormals); //Wska¿ tablicê z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, _TerrainTexCoords);
	glEnableVertexAttribArray(sp->a("texCoord1"));
	glVertexAttribPointer(sp->a("texCoord1"), 2, GL_FLOAT, false, 0, _TerrainTexCoords);
	glUniform1i(sp->u("textureMap0"), 0);
	glUniform1i(sp->u("textureMap1"), 1);


	glDrawArrays(GL_TRIANGLES, 0, getVerticesCount()); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wy³¹cz przesy³anie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void Terrain::showVertices()
{
	printf("-------------------\n");
	for (int i = 0; i < _SIZE; i++)
	{
		printf("%f, ", _TerrainVertices[i]);
		if ((i+1) % 4 == 0)printf("\n");
		if ((i + 1) % 12 == 0)printf("o--------o\n");
	}
	printf("-------------------\n");

	printf("-------------------\n");
	for (int i = 0; i < _SIZE; i++)
	{
		printf("%f, ", _TerrainNormals[i]);
		if ((i + 1) % 4 == 0)printf("\n");
		if ((i + 1) % 12 == 0)printf("o--------o\n");
	}
	printf("-------------------\n");
}

float* Terrain::getVertices()
{
	return _TerrainVertices;
}
float* Terrain::getNormals()
{
	return _TerrainNormals;
}


int Terrain::getVerticesCount()
{
	return (int)_SIZE/4;
}

float Terrain::getHeight(float x, float y)
{
	return 0.0f;
}




float Terrain::_TerrainTexCoords[] = {
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
};