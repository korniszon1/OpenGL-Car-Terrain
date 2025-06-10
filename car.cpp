#include <car.h>

MeshData Car::CarBase;
MeshData Car::CarWheel;

GLuint Car::baseTexture;
GLuint Car::tintAreaTexture;
GLuint Car::emissionTexture;

bool Car::disco = false;
float Car::disco_t = 0.0f;

Car::Car() {}

void Car::drawWheel(ShaderProgram* sp, glm::mat4 M, glm::vec3 translate, float car_speed, float angle) {
	glm::mat4 WMM = M;

	WMM = glm::translate(WMM, translate);
	WMM = glm::rotate(WMM, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	WMM = glm::rotate(WMM, wheels_rot, glm::vec3(0.0f, 0.0f, 1.0f));
	WMM = glm::translate(WMM, -CarWheel.center);

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(WMM));

	for (int i = 0; i < CarWheel.verts.size(); ++i) {
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, CarWheel.verts[i].data());
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, CarWheel.norms[i].data());
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, CarWheel.tcoords[i].data());
		glDrawElements(GL_TRIANGLES, CarWheel.indices[i].size(), GL_UNSIGNED_INT, CarWheel.indices[i].data());
	}
}

void Car::drawCar(ShaderProgram* sp, glm::mat4 V, glm::mat4 P) {
	sp->use();

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));

	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("normal"));
	glEnableVertexAttribArray(sp->a("texCoord0"));

	glm::vec3 tint(16.0 / 255.0, 50 / 255.0, 130.0 / 255.0);
	glUniform3fv(sp->u("tintColor"), 1, glm::value_ptr(tint));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Car::baseTexture);
	glUniform1i(sp->u("textureMap0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Car::tintAreaTexture);
	glUniform1i(sp->u("textureMap1"), 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Car::emissionTexture);
	glUniform1i(sp->u("textureMap2"), 2);

	glUniform1f(sp->u("uTime"), disco_t);
	glUniform4f(sp->u("lp"), 0, 0, -6, 1); // spot light

	glm::vec3 lightDirection = glm::normalize(glm::vec3((float)(200 * 5) * 8, 100.0f, -(float)(200 * 5)*4));
	glUniform3fv(sp->u("lightDir"), 1, glm::value_ptr(lightDirection));

	glUniform3fv(sp->u("viewPos"), 1, glm::value_ptr(pos));

	// --- draw car base models --- //
	for (int i = 0; i < CarBase.verts.size(); ++i) {
		glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, CarBase.verts[i].data());
		glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, CarBase.norms[i].data());
		glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, CarBase.tcoords[i].data());
		glDrawElements(GL_TRIANGLES, CarBase.indices[i].size(), GL_UNSIGNED_INT, CarBase.indices[i].data());
	}

	// --- wheels --- //
	wheels_rot += speed * glfwGetTime() * 2;
	if (wheels_rot > 2 * PI) wheels_rot = 0;

	// x
	float wheelPosFront = 1.1235f;
	float wheelPosBack = -1.523f;
	// y
	float wheelPosHeight = -0.78624f;
	// z
	float wheelPosLeft = -2.1451f;
	float wheelPosRight = 0.02817f;

	// front left
	drawWheel(sp, M, glm::vec3(wheelPosFront, wheelPosHeight, wheelPosLeft), speed, 0 + front_angle);
	// front right
	drawWheel(sp, M, glm::vec3(wheelPosFront, wheelPosHeight, wheelPosRight), speed, PI + front_angle);
	// back left
	drawWheel(sp, M, glm::vec3(wheelPosBack, wheelPosHeight, wheelPosLeft), speed, 0);
	// back right
	drawWheel(sp, M, glm::vec3(wheelPosBack, wheelPosHeight, wheelPosRight), speed, PI);

	glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

void Car::updateCar(double t, Terrain& teren) {
	// slow down
	if (acce == 0.0f) {
		speed *= drag;
		if (speed < 0.01f) speed = 0.0f;
	}

	speed += acce;
	speed = glm::clamp(speed, -5.0f, 30.0f);
	float speed_perc = glm::min(abs(speed > 0.0f ? speed / 15.0f : speed / -5.0f), 1.0f);

	angle += rotation_speed * speed_perc * t;

	prev_pos = pos;

	pos.x -= cos(angle) * speed * t;
	pos.z += sin(angle) * speed * t;
	pos.y = teren.getHeight(pos.x, pos.z);

	M = glm::mat4(1.0f);
	M = glm::translate(M, glm::mix(prev_pos, pos, 0.5));

	glm::vec3 terrain_normal = teren.getTerrainNormal(pos.x, pos.z);
	glm::vec3 forward = glm::normalize(glm::vec3(sin(angle), 0.0f, cos(angle)));
	glm::vec3 right = glm::normalize(glm::cross(forward, terrain_normal));
	glm::vec3 adjustedForward = glm::normalize(glm::cross(terrain_normal, right));
	glm::mat4 rotationMatrix = glm::mat4(glm::vec4(right, 0.0), glm::vec4(terrain_normal, 0.0), glm::vec4(adjustedForward, 0.0), glm::vec4(0.0, 0.0, 0.0, 1.0));
	M = M * rotationMatrix;

	M = glm::translate(M, glm::vec3(-CarBase.center.x, -(CarBase.center.y + CarWheel.center.y * 2), -CarBase.center.z));

	if (disco) disco_t += t / 5;
}

void Car::keyCallback(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { front_angle = glm::mix(front_angle, -PI / 4.0f, 0.1f); }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { front_angle = glm::mix(front_angle, PI / 4.0f, 0.1f); }
	
	if (glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS) { front_angle = glm::mix(front_angle, 0.0f, 0.1f); }
}

//glm::vec3 Car::getPos() { return glm::vec3(M[3]); }

void Car::setPos(glm::vec3 _pos) { pos = _pos; }
glm::vec3 Car::getPos() { return pos; }
float Car::getAngle() { return angle; }
float Car::getRotationSpeed() { return rotation_speed; }

void Car::setAcceleration(float v) { acce = v; }
void Car::setRotationSpeed(float v) { rotation_speed = v; }
