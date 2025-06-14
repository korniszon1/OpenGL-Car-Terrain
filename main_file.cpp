/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "shaderprogram.h"
#include <chrono>
#include <cstdio>
#include "utils.h"

//#include "model.h"
//#include "cube.h"
#include <random>
#include "camera.h"
#include "terrain.h"
#include "car.h"
#include "model_loader.h"
#include "skybox.h"
#include "water.h"
#include "grass.h"


float aspectRatio = 1;

glm::vec3 cam_pos = glm::vec3(0.0f, 1.0f, -6.0f); // startowa pozycja kamery
glm::vec3 cam_rot = glm::vec3(0.0f, 0.0f, 0.0f);

float cam_max_speed = 0.05f;
float cam_pos_speed = 0.0f;
float cam_rot_speed = 0.0f;

Camera* camera = new Camera(500, 500,cam_pos);
Terrain teren;
Car samochod;
Skybox skybox;
Water ocean;

// trawa ustawienia -----------------------------------------
const int grassCount = 5;
const float maxRenderDistance = 100.0f;
const int chunkSize = 16;
Grass plants(800, grassCount);
// trawa ustawienia -----------------------------------------

// --- resources ---
// shaders
ShaderProgram *mainSp;
ShaderProgram *wireSp;
ShaderProgram *discoCarSp;

// textures
GLuint tex0;
GLuint tex1;
GLuint tex2;
GLuint tex3;
GLuint tex4;
GLuint tex5;
GLuint tex6;
GLuint tex7;

GLuint skyboxTexture;

ShaderProgram* sp;
ShaderProgram* skyboxShader;
ShaderProgram* shadowSp;
ShaderProgram* waterSp;


GLuint loadCubemap(std::vector<std::string> faces) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++) {
		std::vector<unsigned char> image;
		unsigned width, height;
		lodepng::decode(image, width, height, faces[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}


void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_A) samochod.setRotationSpeed( PI / 2);
        if (key == GLFW_KEY_D) samochod.setRotationSpeed(-PI / 2);
		if (key == GLFW_KEY_W) samochod.setAcceleration( 0.5);
		if (key == GLFW_KEY_S) samochod.setAcceleration(-0.5);

		if (key == GLFW_KEY_1) sp = mainSp;
		if (key == GLFW_KEY_2) sp = wireSp;
		if (key == GLFW_KEY_0) Car::disco = !Car::disco;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A && samochod.getRotationSpeed() > 0) samochod.setRotationSpeed(0);
		if (key == GLFW_KEY_D && samochod.getRotationSpeed() < 0) samochod.setRotationSpeed(0);
		if (key == GLFW_KEY_W || key == GLFW_KEY_S) samochod.setAcceleration(0);
    }
}


void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	tex0 = readTexture("textures/ground.png");
	tex1 = readTexture("textures/ground_normal.png");
	tex2 = readTexture("textures/water.png");
	tex3 = readTexture("textures/noise.png");
	tex4 = readTexture("textures/plantMini.png");
	tex5 = readTexture("textures/plantMini_normal.png");
	tex6 = readTexture("textures/bush.png");
	tex7 = readTexture("textures/bush_normal.png");

	mainSp = new ShaderProgram("shaders/v_terrain.glsl", NULL, "shaders/f_terrain.glsl");
	wireSp = new ShaderProgram("shaders/v_simplest.glsl", "shaders/g_simplest.glsl", "shaders/f_simplest.glsl");
	discoCarSp = new ShaderProgram("shaders/v_simplest.glsl", NULL, "shaders/f_car_disco.glsl");
	skyboxShader = new ShaderProgram("shaders/v_skybox.glsl", NULL, "shaders/f_skybox.glsl");
	waterSp = new ShaderProgram("shaders/v_water.glsl", "shaders/g_water.glsl", "shaders/f_water.glsl");

	sp = mainSp;

	Car::CarBase = loadModel("models/car_base.obj");
	Car::CarWheel = loadModel("models/car_wheel.obj");
	Car::baseTexture = readTexture("models/car_texture.png");
	Car::emissionTexture = readTexture("models/car_emission.png");
	Car::tintAreaTexture = readTexture("models/color_tint_area.png");

	std::vector<std::string> faces = {
		"textures/skybox/clouds1_east.png",
		"textures/skybox/clouds1_west.png",
		"textures/skybox/clouds1_up.png",
		"textures/skybox/clouds1_down.png",
		"textures/skybox/clouds1_north.png",
		"textures/skybox/clouds1_south.png"
	};

	skyboxTexture = loadCubemap(faces);
	skybox.initSkybox();

	plants.init(teren,tex4,tex5,tex6,tex7,chunkSize);

}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &tex0);
	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);
	glDeleteTextures(1, &tex3);
	glDeleteTextures(1, &tex4);
	glDeleteTextures(1, &tex5);
	glDeleteTextures(1, &tex6);
	glDeleteTextures(1, &tex7);

	glDeleteTextures(1, &Car::baseTexture);
	glDeleteTextures(1, &Car::emissionTexture);
	glDeleteTextures(1, &Car::tintAreaTexture);

    delete mainSp;
	delete wireSp;
	delete discoCarSp;
	delete skyboxShader;
	delete waterSp;
}


//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, Camera *camera) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = camera->getCameraProj();
	glm::mat4 view = camera->getCameraView();
	glm::vec3 cameraPos = camera->getPos();

	glm::vec3 carPos = samochod.getPos();
	float carAngle = samochod.getAngle();

	//Skybox musi renderowac sie pierwszy
	skybox.drawSkybox(skyboxShader, skyboxTexture, view, projection);
	teren.drawTerrain(sp, tex0, tex1, carPos.x, carPos.z, carAngle, view, projection, camera->getPos());
	ocean.drawWater(waterSp, tex2, tex3, skyboxTexture, carPos.x, carPos.z, carAngle, view, projection, camera->getPos());

	samochod.drawCar(discoCarSp, view, projection);

	camera->target = carPos;
	camera->updateOrbit();
	
	plants.drawGrass(sp, *camera, maxRenderDistance, chunkSize);
	
    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(512, 512, "Car", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	samochod.setPos(glm::vec3(500, 500, 500));

	glfwSetTime(0); //Zeruj timer
	glfwSwapInterval(0); //wylaczony vsync

	using clock = std::chrono::high_resolution_clock;

	auto lastTime = clock::now();
	int frameCount = 0;
	double fpsTimer = 0.0;
	char buffer[128];

	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		auto currentTime = clock::now();
		std::chrono::duration<double> elapsed = currentTime - lastTime;
		lastTime = currentTime;
		double deltaTime = elapsed.count();

		fpsTimer += deltaTime;
		frameCount++;

		if (fpsTimer >= 1.0) {
			std::snprintf(buffer, sizeof(buffer), "FPS: %.2f", frameCount / fpsTimer);
			glfwSetWindowTitle(window, buffer);
			frameCount = 0;
			fpsTimer = 0.0;
		}

		camera->update_camera(50.0f, aspectRatio, sp, 0.01f, 2000.0f);

		samochod.updateCar(glfwGetTime(), teren);

        glfwSetTime(0); //Zeruj timer
		drawScene(window, camera); //Wykonaj procedurę rysującą

		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
		camera->keyCallback(window);
		samochod.keyCallback(window);
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
