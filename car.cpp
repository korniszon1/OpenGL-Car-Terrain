#include <car.h>
#include "myTeapot.h"

float* vertices = myTeapotVertices;
float* normals = myTeapotVertexNormals;
float* texCoords = myTeapotTexCoords;
float* colors = myTeapotColors;
int vertexCount = myTeapotVertexCount;

Car::Car()
{
	
}

void Car::drawCar(ShaderProgram* sp, GLuint& tex0, GLuint& tex1, float pos_x, float pos_y, float pos_z)
{
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(pos_x, pos_y, pos_z));
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
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wska¿ tablicê z danymi dla atrybutu vertex

	//glEnableVertexAttribArray(sp->a("color"));  //W³¹cz przesy³anie danych do atrybutu color
	//glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wska¿ tablicê z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //W³¹cz przesy³anie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wska¿ tablicê z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(sp->a("texCoord1"));
	glVertexAttribPointer(sp->a("texCoord1"), 2, GL_FLOAT, false, 0, texCoords);
	glUniform1i(sp->u("textureMap0"), 0);
	glUniform1i(sp->u("textureMap1"), 1);


	glDrawArrays(GL_TRIANGLES, 0, vertexCount); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wy³¹cz przesy³anie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wy³¹cz przesy³anie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wy³¹cz przesy³anie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
}