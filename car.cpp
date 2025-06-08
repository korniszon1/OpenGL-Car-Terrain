#include <car.h>

MeshData Car::CarBase;
MeshData Car::CarWheel;

float Car::t = 0.0f;

Car::Car()
{
	
}

void Car::drawWheel(ShaderProgram* sp, glm::mat4 M, glm::vec3 posInScene, float car_speed, float rotation) {
	glm::mat4 WMM = M;
	wheelRot += car_speed * glfwGetTime() * 2;
	if (wheelRot > 2 * PI) wheelRot = 0;
	glm::mat4 T = glm::translate(glm::mat4(1.0f), posInScene);
	glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 RS = glm::rotate(glm::mat4(1.0f), wheelRot, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 toCenter = glm::translate(glm::mat4(1.0f), -CarWheel.center); // center obliczony przy ³adowaniu modelu
	WMM *= T * R * RS * toCenter;
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(WMM));
	for (int i = 0; i < CarWheel.verts.size(); ++i) {
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, CarWheel.verts[i].data());
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, CarWheel.norms[i].data());
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, CarWheel.tcoords[i].data());
		glDrawElements(GL_TRIANGLES, CarWheel.indices[i].size(), GL_UNSIGNED_INT, CarWheel.indices[i].data());
	}
}

void Car::drawCar(ShaderProgram* sp, glm::mat4 V, glm::mat4 P, GLuint& tex0, GLuint& tex1, GLuint& tex2, float angle, float pos_x, float pos_y, float pos_z, float car_speed, glm::vec3 terrainNormal, bool disco)
{
	//TODO grawitacja
	//if (y < pos_y + CarBase.center.y * 4) y = pos_y;
	//else y = y - gravity;

	y = pos_y;
	sp->use();
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::vec3(pos_x, y, pos_z));

	glm::vec3 forward = glm::normalize(glm::vec3(sin(angle), 0.0f, cos(angle))); // np. z systemu pojazdu

	glm::vec3 right = glm::normalize(glm::cross(forward, terrainNormal));
	glm::vec3 adjustedForward = glm::normalize(glm::cross(terrainNormal, right)); // poprawiony Z

	glm::mat4 rotationMatrix = glm::mat4(
		glm::vec4(right, 0.0),
		glm::vec4(terrainNormal, 0.0),
		glm::vec4(adjustedForward, 0.0),
		glm::vec4(0.0, 0.0, 0.0, 1.0)
	);

	M = M * rotationMatrix;
	M = glm::translate(M, glm::vec3(-CarBase.center.x, -(CarBase.center.y + CarWheel.center.y * 2), -CarBase.center.z));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	//glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	//glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glUniform4f(sp->u("lp"), 0, 0, -6, 1); // light

	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("normal"));
	glEnableVertexAttribArray(sp->a("texCoord0"));

	glm::vec3 tint(16.0 / 255.0, 50 / 255.0, 130.0 / 255.0);
	glUniform3fv(sp->u("tintColor"), 1, glm::value_ptr(tint));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glUniform1i(sp->u("textureMap0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(sp->u("textureMap1"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glUniform1i(sp->u("textureMap2"), 2);

	if (disco) t += glfwGetTime() * 1;
	if (t > 1) t = 0;
	glUniform1f(sp->u("uTime"), t);
	glm::vec3 lightDirection = glm::normalize(glm::vec3((float)(200 * 5) * 8, 100.0f, -(float)(200 * 5)*4));
	glUniform3fv(sp->u("lightDir"), 1, glm::value_ptr(lightDirection));

	glm::vec3 cameraPos = glm::vec3(pos_x,pos_y,pos_z);
	glUniform3fv(sp->u("viewPos"), 1, glm::value_ptr(cameraPos));
	for (int i = 0; i < CarBase.verts.size(); ++i) {
		//glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices); //Wspó³rzêdne wierzcho³ków bierz z tablicy myCubeVertices
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, CarBase.verts[i].data()); //Wspó³rzêdne wierzcho³ków bierz z tablicy myCubeVertices

		//glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, myCubeNormals);
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, CarBase.norms[i].data());

		//glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, myCubeTexCoords); //Wspó³rzêdne teksturowania bierz z tablicy myCubeTexCoords
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, CarBase.tcoords[i].data()); //Wspó³rzêdne teksturowania bierz z tablicy myCubeTexCoords

		//glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);
		glDrawElements(GL_TRIANGLES, CarBase.indices[i].size(), GL_UNSIGNED_INT, CarBase.indices[i].data());
	}

	// --- x --- //
	float wheelPosFront = 1.1235f;
	float wheelPosBack = -1.523f;
	// --- y --- //
	float wheelPosHeight = -0.78624f;
	// --- z --- //
	float wheelPosLeft = -2.1451f;
	float wheelPosRight = 0.02817f;
	// front left
	drawWheel(sp, M, glm::vec3(wheelPosFront, wheelPosHeight, wheelPosLeft), car_speed, 0 + front_rotation);
	// front right
	drawWheel(sp, M, glm::vec3(wheelPosFront, wheelPosHeight, wheelPosRight), car_speed, PI + front_rotation);
	// back left
	drawWheel(sp, M, glm::vec3(wheelPosBack, wheelPosHeight, wheelPosLeft), car_speed, 0);
	// back right
	drawWheel(sp, M, glm::vec3(wheelPosBack, wheelPosHeight, wheelPosRight), car_speed, PI);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void Car::keyCallback(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		front_rotation = glm::mix(front_rotation, -PI / 4.0f, 0.1f);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		front_rotation = glm::mix(front_rotation, PI / 4.0f, 0.1f);
	}
	if (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS)
	{
		front_rotation = glm::mix(front_rotation, 0.0f, 0.1f);
	}

}

glm::vec3 Car::getCameraPos() { return glm::vec3(M[3]); }
