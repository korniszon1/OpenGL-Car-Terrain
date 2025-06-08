#include"camera.h"


Camera::Camera(int w, int h,glm::vec3 pos):position(pos), height(h), width(w)
{}

void Camera::update_camera(float fovy, float aspect, ShaderProgram *sp, float near, float far)
{

	V = glm::lookAt(
		position,
		rotation + position,
		up);

	P = glm::perspective(fovy * PI / 180.0f, aspect, near, far);

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

}
void Camera::update_position(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}


Camera::~Camera()
{
}

void Camera::keyCallback(GLFWwindow* window) {

	//Stara kamera
	/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += speed * rotation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += speed * -glm::normalize(glm::cross(rotation,up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += speed * -rotation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += speed * glm::normalize(glm::cross(rotation, up));
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float mRotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float mRotY = sensitivity * (float)(mouseX - (height / 2)) / height;

		
		rotation = glm::rotate(rotation, glm::radians(-mRotX), glm::normalize(glm::cross(rotation, up)));

		rotation = glm::rotate(rotation, glm::radians(-mRotY),up);

		glfwSetCursorPos(window, (width / 2), (height / 2) );
	}*/
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		float offsetX = (float)(mouseX - width / 2) * sensitivity;
		float offsetY = (float)(mouseY - height / 2) * sensitivity;

		yaw -= offsetX;
		pitch += offsetY;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < 0.0f) pitch = 0.0f;

		glfwSetCursorPos(window, width / 2, height / 2);
	}
}

void Camera::updateOrbit() {
	float yawRad = glm::radians(yaw);
	float pitchRad = glm::radians(pitch);

	position.x = target.x + distance * cos(pitchRad) * sin(yawRad);
	position.y = target.y + distance * sin(pitchRad);
	position.z = target.z + distance * cos(pitchRad) * cos(yawRad);

	rotation = glm::normalize(target - position);
}

glm::vec3 Camera::getPos() { return position; }
glm::mat4 Camera::getCameraView() { return V;}
glm::mat4 Camera::getCameraProj() { return P; }