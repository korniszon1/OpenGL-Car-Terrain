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
#include "lodepng.h"
#include "shaderprogram.h"
//#include "myCube.h"
//#include "model.h"
//#include "cube.h"

#include "camera.h"
#include "terrain.h"
#include "car.h"
#include "model_loader.h"
#include "skybox.h"
#include "water.h"

float speed_x=0;
float car_speed =0;
float car_acce = 0;
float acce_speed = 0.5f;
float aspectRatio=1;
float gravity = 0.5;
float drag = 0.96f;


glm::vec3 cam_pos = glm::vec3(0.0f, 1.0f, -6.0f); // startowa pozycja kamery
glm::vec3 cam_rot = glm::vec3(0.0f, 0.0f, 0.0f);
float cam_max_speed = 0.05f;
float cam_pos_speed = 0.0f;
float cam_rot_speed = 0.0f;
ShaderProgram *sp;
Camera* camera = new Camera(500, 500,cam_pos);
Terrain teren;
Car samochod;
Skybox skybox;
Water ocean;

ShaderProgram *mainSp;
ShaderProgram *wireSp;
ShaderProgram *discoCarSp;

//Odkomentuj, żeby rysować kostkę
//float* vertices = myCubeVertices;
//float* normals = myCubeNormals;
//float* texCoords = myCubeTexCoords;
//float* colors = myCubeColors;
//int vertexCount = myCubeVertexCount;


GLuint tex0;
GLuint tex1;
GLuint tex2;
GLuint tex3;
GLuint tex4;

GLuint carTexture;
GLuint carTintAreaTexture;
GLuint skyboxTexture;
//GLuint skyboxVAO, skyboxVBO, cubemapTexture;
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



GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	float maxAnisotropy = 1.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		GL_REPEAT);
	return tex;
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

int speed = 1;

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS || action == GLFW_REPEAT) {
        if (key==GLFW_KEY_A) speed_x=PI/2 * speed;
        if (key==GLFW_KEY_D) speed_x=-PI/2 * speed;
        if (key==GLFW_KEY_W) car_acce = acce_speed;
        if (key==GLFW_KEY_S) car_acce = -acce_speed;
		if (key == GLFW_KEY_1)sp = mainSp;
		if (key == GLFW_KEY_2)sp = wireSp;
		//Kamera
		/*if (key == GLFW_KEY_W) cam_pos_speed = cam_max_speed;
		if (key == GLFW_KEY_A) cam_rot_speed = cam_max_speed;
		if (key == GLFW_KEY_S) cam_pos_speed = -cam_max_speed;
		if (key == GLFW_KEY_D) cam_rot_speed = -cam_max_speed;
		printf("%f",cam_rot.r);*/
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_A) speed_x=0;
        if (key==GLFW_KEY_D) speed_x=0;
		if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
			car_acce = 0.0f;
		}

		/*if (key == GLFW_KEY_W) cam_pos_speed = 0;
		if (key == GLFW_KEY_A) cam_rot_speed = 0;
		if (key == GLFW_KEY_S) cam_pos_speed = 0;
		if (key == GLFW_KEY_D) cam_rot_speed = 0;*/
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
	printf("wysokosc: %i %i", width, height);
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);
	tex0 = readTexture("textures/ground.png");
	tex1 = readTexture("textures/ground_normal.png");
	tex2 = readTexture("textures/water.png");
	tex3 = readTexture("textures/noise.png");
	//sp=new ShaderProgram("v_simplest.glsl",NULL, "f_simplest.glsl");

	mainSp = new ShaderProgram("shaders/v_terrain.glsl", NULL, "shaders/f_terrain.glsl");
	wireSp = new ShaderProgram("shaders/v_simplest.glsl", "shaders/g_simplest.glsl", "shaders/f_simplest.glsl");
	discoCarSp = new ShaderProgram("shaders/v_simplest.glsl", NULL, "shaders/f_car_disco.glsl");
	skyboxShader = new ShaderProgram("shaders/v_skybox.glsl", NULL, "shaders/f_skybox.glsl");
	waterSp = new ShaderProgram("shaders/v_water.glsl", "shaders/g_water.glsl", "shaders/f_water.glsl");
	//shadowSp = new ShaderProgram("shaders/v_shadow.glsl", NULL, "shaders/f_shadow.glsl");
	sp = mainSp;

	carTexture = readTexture("models/car_texture.png");
	carTintAreaTexture = readTexture("models/color_tint_area.png");

	Car::CarBase = loadModel("models/car_base.obj");
	Car::CarWheel = loadModel("models/car_wheel.obj");

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
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	glDeleteTextures(1, &tex0);
	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);

	glDeleteTextures(1, &carTexture);
	glDeleteTextures(1, &carTintAreaTexture);

    delete mainSp;
	delete wireSp;
	delete discoCarSp;
}




//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle, float pos_x, float pos_z, float car_speed, Camera *camera) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = camera->getCameraProj();
	glm::mat4 view = camera->getCameraView();

	//Skybox musi renderowac sie pierwszy
	skybox.drawSkybox(skyboxShader, skyboxTexture, view, projection);
	camera->target = glm::vec3(pos_x, samochod.getCameraPos().y, pos_z);
	//camera->updateOrbit();
	teren.drawTerrain(sp, tex0, tex1, pos_x, pos_z, angle, view, projection, camera->getPos());
	camera->updateOrbit();
	ocean.drawWater(waterSp, tex2, tex3, skyboxTexture, pos_x, pos_z, angle, view, projection, camera->getPos());
	samochod.drawCar(discoCarSp, view, projection, carTexture, carTintAreaTexture, angle, pos_x, teren.getHeight(pos_x, pos_z), pos_z, car_speed, teren.getTerrainNormal(pos_x, pos_z));
	
	
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
	float angle=0;

	float pos_x = 300;
	float pos_z = 300;
	glfwSetTime(0); //Zeruj timer

	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		if (car_acce == 0.0f) {
			car_speed *= drag; 
			if (car_speed < 0.01f) car_speed = 0.0f;
		}
		car_speed += car_acce;
		car_speed = glm::clamp(car_speed, -2.0f, 30.0f);
		
        angle+=speed_x*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        //pos_x+=speed_y*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
		pos_x -= cos(angle) * car_speed * glfwGetTime();
		pos_z += sin(angle) * car_speed * glfwGetTime();
		camera->keyCallback(window);
		samochod.keyCallback(window);
		camera->update_camera(50.0f, aspectRatio, sp, 0.01f, 2000.0f);
		//cam_pos += cam_pos_speed * cam_pos;
		//cam_rot += glm::vec3(cam_rot_speed,0,0);
        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle, pos_x, pos_z, car_speed, camera); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.

	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
