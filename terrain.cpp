#include "terrain.h"
#include <cmath>

Terrain::Terrain()
{
	int x = 0;
	int y = 0;
	float z[12][12]{0.0f};
	float a = 1.0f;
	//WIP
	/*for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			z[i][j] = i;
		}
	}*/
	

	for (int i = 0; i < _SIZE; i = i + 4)
	{
		if (i % (int)(24 * std::sqrt(_N)) == 0) { y++; x = 0; }
		//printf("I = %i \n", i);
		_TerrainVertices[i] = x;
		_TerrainVertices[i + 1] = y;
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;
		i = i + 4;
		
		_TerrainVertices[i] = ++x;
		_TerrainVertices[i + 1] = y;
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;
		i = i + 4;
		_TerrainVertices[i] = --x;
		_TerrainVertices[i + 1] = ++y;
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;
		i = i + 4;
		
		_TerrainVertices[i] = _TerrainVertices[i - 4];
		_TerrainVertices[i + 1] = _TerrainVertices[i - 3];
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;
		i = i + 4;
		_TerrainVertices[i] = _TerrainVertices[i - 12];
		_TerrainVertices[i + 1] = _TerrainVertices[i - 11];
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;
		i = i + 4;
		//printf("--I = %i \n", i);
		_TerrainVertices[i] = ++x;
		_TerrainVertices[i + 1] = y--;
		_TerrainVertices[i + 2] = z[x][y];
		_TerrainVertices[i + 3] = a;
		//printf("I = %i \n", i);
		//showVertices();
	}
	
	x = 0;
	y = 0;
	a = 0.0f;
	printf("S: %i", (int)(12 * std::sqrt(_N)));
	for (int i = 0; i < _SIZE; i = i + 4)
	{
		if (i % (int)(24 * std::sqrt(_N) ) == 0) { y++; x = 0; }
		//printf("I = %i \n", i);
		_TerrainNormals[i] = x;
		_TerrainNormals[i + 1] = y;
		_TerrainNormals[i + 2] = z[x][y];
		_TerrainNormals[i + 3] = a;
		i = i + 4;
		_TerrainNormals[i] = ++x;
		_TerrainNormals[i + 1] = y;
		_TerrainNormals[i + 2] = z[x][y];
		_TerrainNormals[i + 3] = a;
		i = i + 4;
		_TerrainNormals[i] = --x;
		_TerrainNormals[i + 1] = ++y;
		_TerrainNormals[i + 2] = z[x][y];
		_TerrainNormals[i + 3] = a;
		i = i + 4;
		_TerrainNormals[i] = _TerrainNormals[i - 4];
		_TerrainNormals[i + 1] = _TerrainNormals[i - 3];
		_TerrainNormals[i + 2] = z[x][y];
		_TerrainNormals[i + 3] = a;
		i = i + 4;
		_TerrainNormals[i] = _TerrainNormals[i - 12];
		_TerrainNormals[i + 1] = _TerrainNormals[i - 11];
		_TerrainNormals[i + 2] = z[x][y];
		_TerrainNormals[i + 3] = a;
		i = i + 4;
		//printf("--I = %i \n", i);
		_TerrainNormals[i] = ++x;
		_TerrainNormals[i + 1] = y--;
		_TerrainNormals[i + 2] = z[x][y];
		_TerrainNormals[i + 3] = a;
		//printf("I = %i \n", i);
		//showVertices();
	}

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

