#include"camera.h"


Camera::Camera(int w, int h,glm::vec3 pos):position(pos), height(h), width(w)
{}

void Camera::update_camera(float fovy, float aspect, ShaderProgram *sp, float near, float far)
{
	V = glm::lookAt(
		position,
		rotation + position,
		up); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(fovy * PI / 180.0f, aspect, near, far);

	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	//printf("pos: %f %f %f \n", (position + rotation).r, (position + rotation).g, (position + rotation).b);
}

Camera::~Camera()
{
}

void Camera::keyCallback(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
	}
	
}