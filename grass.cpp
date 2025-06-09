#include "grass.h"
#include "quads.h"

Grass::Grass(int chunks_amount,int GC) : _CHUNKS(chunks_amount), _GRASS_COUNT(GC)
{
	gc = new GrassChunk[chunks_amount];
}

void Grass::init(Terrain &teren, GLuint &t0, GLuint &t0n, GLuint &t1, GLuint &t1n, int CHUNK_SIZE)
{

	int sizeChunks = sqrt(_CHUNKS);
	int offset = (1000 - (sizeChunks*CHUNK_SIZE))/2;

	_tex0 = t0;
	_tex0n = t0n;
	_tex1 = t1;
	_tex1n = t1n;
	printf("Generating chunks \n");
	for (int i = 0; i < sizeChunks; i++)
	{
		for (int j = 0; j < sizeChunks;j++)
		{
			gc[i * sizeChunks + j].generate(teren, offset + j * CHUNK_SIZE, offset + i * CHUNK_SIZE, _GRASS_COUNT, CHUNK_SIZE);
		}
	}
	printf("Generating chunks: Done \n");
}

bool IsChunkInViewCone(
	const glm::vec3& chunkCenter,
	const glm::vec3& cameraPos,
	const glm::vec3& cameraDir,
	float fovDegrees,
	float maxDistance,
	int chunk_size)
{
	glm::vec3 toChunk = chunkCenter - cameraPos;
	float distance = glm::length(toChunk);
	if (distance > maxDistance) return false;
	glm::vec3 dirToChunk = glm::normalize(toChunk);

	float angleToChunk = glm::degrees(acos(glm::dot(dirToChunk, glm::normalize(cameraDir))));
	return ( distance < chunk_size || angleToChunk < (fovDegrees / 2.0f));
}


void Grass::drawGrass(ShaderProgram *sp, Camera &camera, int maxDistance, int chunk_size)
{
	glm::vec3 cameraDir = glm::normalize(camera.getCameraFront());
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraDir, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 cameraUp = camera.getCameraUp();

	sp->use();
	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, qVertices);

	glEnableVertexAttribArray(sp->a("normal"));
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, qNormals);

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, qTexCoords);
	glEnableVertexAttribArray(sp->a("texCoord1"));
	glVertexAttribPointer(sp->a("texCoord1"), 2, GL_FLOAT, false, 0, qTexCoords);
	glUniform1i(sp->u("textureMap0"), 0);
	glUniform1i(sp->u("textureMap1"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _tex0n);

	int p = _GRASS_COUNT * _GRASS_COUNT * _CHUNKS;
	for (int i = 0; i < _CHUNKS; i++)
	{
		if (!IsChunkInViewCone(glm::vec3(gc[i].center), camera.getPos(), camera.getCameraFront(), 160.0f, maxDistance, chunk_size))
			continue;

		for (int j = 0; j < _GRASS_COUNT * _GRASS_COUNT; j++)
		{
			//Prawdopodobienstwo drzewa
			if (i < _CHUNKS -1 && j * i % (int)(p / 1.2f) == 0)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, _tex1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, _tex1n);
			}



			glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(gc[i].modelMatrices[j]));
			glDrawArrays(GL_TRIANGLES, 0, qVertexCount);

			if (j * i % (int)(p / 1.2f) == 0) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, _tex0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, _tex0n);
			}
		}
		
		
	}
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}