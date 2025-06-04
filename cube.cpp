/*
Niniejszy program jest wolnym oprogramowaniem; mo¿esz go
rozprowadzaæ dalej i / lub modyfikowaæ na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundacjê Wolnego
Oprogramowania - wed³ug wersji 2 tej Licencji lub(wed³ug twojego
wyboru) którejœ z póŸniejszych wersji.

Niniejszy program rozpowszechniany jest z nadziej¹, i¿ bêdzie on
u¿yteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyœlnej
gwarancji PRZYDATNOŒCI HANDLOWEJ albo PRZYDATNOŒCI DO OKREŒLONYCH
ZASTOSOWAÑ.W celu uzyskania bli¿szych informacji siêgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnoœci¹ wraz z niniejszym programem otrzyma³eœ te¿ egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeœli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#include "cube.h"

namespace Models {

	Cube cube;

	Cube::Cube() {
		vertices = CubeInternal::vertices;
		normals = CubeInternal::normals;
		vertexNormals = CubeInternal::vertexNormals;
		texCoords = CubeInternal::texCoords;
		colors = CubeInternal::colors;
		vertexCount = CubeInternal::vertexCount;
	}

	Cube::~Cube() {
	}

	void Cube::drawSolid(bool smooth) {

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);


		glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, vertices);
		if (!smooth) glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, normals);
		else glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, vertexNormals);
		glVertexAttribPointer(2, 4, GL_FLOAT, false, 0, texCoords);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, 0, colors);

		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
	}

	namespace CubeInternal {
		unsigned int vertexCount = 36;

		float vertices[] = {
			1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			-1.0f,-1.0f,-1.0f,1.0f,

			1.0f,-1.0f,-1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,


			-1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,

			-1.0f,-1.0f, 1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,

			1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			1.0f,-1.0f,-1.0f,1.0f,

			1.0f,-1.0f, 1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,

			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,
			-1.0f,-1.0f, 1.0f,1.0f,

			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			-1.0f, 1.0f, 1.0f,1.0f,

			-1.0f,-1.0f,-1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,
			1.0f,-1.0f,-1.0f,1.0f,

			-1.0f,-1.0f,-1.0f,1.0f,
			-1.0f,-1.0f, 1.0f,1.0f,
			1.0f,-1.0f, 1.0f,1.0f,

			-1.0f, 1.0f, 1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,
			1.0f, 1.0f, 1.0f,1.0f,

			-1.0f, 1.0f, 1.0f,1.0f,
			-1.0f, 1.0f,-1.0f,1.0f,
			1.0f, 1.0f,-1.0f,1.0f,

		};

		float colors[] = {
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,

			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,
			1.0f,0.0f,0.0f,1.0f,

			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,

			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,
			0.0f,1.0f,0.0f,1.0f,

			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,

			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,
			0.0f,0.0f,1.0f,1.0f,

			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,

			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,
			1.0f,1.0f,0.0f,1.0f,

			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,

			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,
			0.0f,1.0f,1.0f,1.0f,

			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,

			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,1.0f,
		};

		float normals[] = {
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,

			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,
			0.0f, 0.0f,-1.0f,0.0f,

			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,

			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,
			0.0f, 0.0f, 1.0f,0.0f,

			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,

			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,
			1.0f, 0.0f, 0.0f,0.0f,

			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,

			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,
			-1.0f, 0.0f, 0.0f,0.0f,

			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,

			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,
			0.0f,-1.0f, 0.0f,0.0f,

			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,

			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,
			0.0f, 1.0f, 0.0f,0.0f,
		};

		float vertexNormals[] = {
			1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			-1.0f,-1.0f,-1.0f,0.0f,

			1.0f,-1.0f,-1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,


			-1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,

			-1.0f,-1.0f, 1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,

			1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			1.0f,-1.0f,-1.0f,0.0f,

			1.0f,-1.0f, 1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,

			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,
			-1.0f,-1.0f, 1.0f,0.0f,

			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			-1.0f, 1.0f, 1.0f,0.0f,

			-1.0f,-1.0f,-1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,
			1.0f,-1.0f,-1.0f,0.0f,

			-1.0f,-1.0f,-1.0f,0.0f,
			-1.0f,-1.0f, 1.0f,0.0f,
			1.0f,-1.0f, 1.0f,0.0f,

			-1.0f, 1.0f, 1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
			1.0f, 1.0f, 1.0f,0.0f,

			-1.0f, 1.0f, 1.0f,0.0f,
			-1.0f, 1.0f,-1.0f,0.0f,
			1.0f, 1.0f,-1.0f,0.0f,
		};

		float texCoords[] = {
			1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
			1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

			1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
			1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

			1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
			1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

			1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
			1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

			1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
			1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

			1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
			1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
		};
	}
}
