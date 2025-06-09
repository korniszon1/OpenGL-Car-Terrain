#include "grass.h"
#include "quads.h"

Grass::Grass(int minXZ, int maxXZ,int GC) 
	:	_gen(_rd()), _dist(minXZ, maxXZ)
{
	_GRASS_COUNT = GC;
	_randomNumberX.resize(GC);
	_randomNumberZ.resize(GC);
	_randomNumberY.resize(GC);
	_modelMatrices.resize(GC, glm::mat4(1.0f));


}

void Grass::init(Terrain &teren, GLuint &t0, GLuint &t0n, GLuint &t1, GLuint &t1n)
{
	_tex0 = t0;
	_tex0n = t0n;

	_tex1 = t1;
	_tex1n = t1n;

	for (int i = 0; i < _GRASS_COUNT; i++)
	{
		glm::mat4 M = glm::mat4(1.0f);

		_randomNumberX[i] = _dist(_gen);
		_randomNumberZ[i] = _dist(_gen);
		_randomNumberY[i] = teren.getHeight(_randomNumberX[i], _randomNumberZ[i]);

		M = glm::translate(M, glm::vec3(_randomNumberX[i], _randomNumberY[i], _randomNumberZ[i]));
		glm::vec3 terrainNormal = teren.getTerrainNormal(_randomNumberX[i], _randomNumberZ[i]);
		glm::vec3 forward = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)); // np. z systemu pojazdu

		glm::vec3 right = glm::normalize(glm::cross(forward, terrainNormal));
		glm::vec3 adjustedForward = glm::normalize(glm::cross(terrainNormal, right)); // poprawiony Z

		glm::mat4 rotationMatrix = glm::mat4(
			glm::vec4(right, 0.0),
			glm::vec4(terrainNormal, 0.0),
			glm::vec4(adjustedForward, 0.0),
			glm::vec4(0.0, 0.0, 0.0, 1.0)
		);

		M = M * rotationMatrix;
		M = glm::translate(M, glm::vec3(0, 5.0f, 0));
		M = glm::scale(M, glm::vec3(6, 6, 6));
		_modelMatrices[i] = M;
	}
}

void Grass::drawGrass(ShaderProgram *sp,glm::vec3 cameraPos, int maxDistance)
{
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
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, tex6);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, tex7);
	
	for (int i = 0; i < _GRASS_COUNT; i++)
	{
		float distance = glm::distance(cameraPos, glm::vec3(_modelMatrices[i][3]));

		//Random teksturka
		if ((_randomNumberX[i] * _randomNumberZ[i]) % (int)(_GRASS_COUNT / 1.5) == 0)
		{

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _tex1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _tex1n);
		}
		else
		{
			if (distance > maxDistance)
				continue;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _tex0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, _tex0n);
		}

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(_modelMatrices[i]));
		glDrawArrays(GL_TRIANGLES, 0, qVertexCount);
	}
	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}