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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"

float speed = 3.14f;


//Procedura obsługi błędów
void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

//Procedura inicjująca
void initOpenGLProgram()
{
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

}

void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) printf("A \n");
		if (key == GLFW_KEY_D) printf("D \n");
		if (key == GLFW_KEY_W && (mods & GLFW_MOD_ALT) != 0) printf("ALT+W\n");
		if (key == GLFW_KEY_LEFT) speed = -1;
		if (key == GLFW_KEY_RIGHT) speed = 1;
		if (key == GLFW_KEY_UP) speed += 2;
		if (key == GLFW_KEY_DOWN) speed -= 2;
	}

	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_W) printf("puszczone W\n");
		//speed = 0;
	}
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle)
{
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClearColor(1, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(0, 0, 1);
	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 V = lookAt(
		glm::vec3(0.0f, 0.0f, -5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(50.0f*PI / 180.0f, 1.0f, 1.0f, 50.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(value_ptr(P));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(value_ptr(V*M));

	glm::mat4 M1 = translate(M, glm::vec3(1.0f, 0.0f, 0.0f));
	M1 = rotate(M1, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	glLoadMatrixf(value_ptr(V*M1));
	Models::torus.drawSolid();
	for (int i = 0; i < 12; i++) {
		glm::mat4 M1C = rotate(M1, 30.0f *i *PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		M1C = translate(M1C, glm::vec3(1.0f, 0.0f, 0.0f));
		M1C = scale(M1C, glm::vec3(0.1f, 0.1f, 0.1f));
		glLoadMatrixf(value_ptr(V*M1C));
		Models::cube.drawSolid();
	}

	glm::mat4 M2 = translate(M, glm::vec3(-1.0f, 0.0f, 0.0f));
	M2 = rotate(M2, angle, glm::vec3(0.0f, 0.0f, -1.0f));
	glLoadMatrixf(value_ptr(V*M2));
	Models::torus.drawSolid();
	for (int i = 0; i < 12; i++) {
		glm::mat4 M2C = rotate(M2, 30.0f *i *PI / 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		M2C = translate(M2C, glm::vec3(1.0f, 0.0f, 0.0f));
		M2C = scale(M2C, glm::vec3(0.1f, 0.1f, 0.1f));
		glLoadMatrixf(value_ptr(V*M2C));
		Models::cube.drawSolid();
	}

	glfwSwapBuffers(window);
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit())   //Zainicjuj bibliotekę GLFW
	{
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	GLenum err;
	if ((err = glewInit()) != GLEW_OK)   //Zainicjuj bibliotekę GLEW
	{
		fprintf(stderr, "Nie można zainicjować GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(); //Operacje inicjujące

						 //Główna pętla
	float angle = 0;
	glfwSetTime(0);
	glfwSetKeyCallback(window, key_callback);
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle += speed*glfwGetTime();
		glfwSetTime(0);
		drawScene(window, angle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
