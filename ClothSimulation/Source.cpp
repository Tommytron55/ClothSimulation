//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// File Name	: Cloth Simulation	
// Description	: A simulation of cloth where you can change multiple variables to see their effects
// Author_1		: Thomas Male
// Mail_1		: thomas.male55@gmail.com
// Author_2		: Ben Hart
// Mail_2		: 
//

#ifdef _WIN32
#include <windows.h> 
#endif

#include <math.h>
#include <vector>
#include <iostream>
#include <stdlib.h>     /* srand, rand */

#include "Cloth.h"

#include <GL/gl.h>
#include <GL/glu.h> 

/* Some physics constants */
#define DAMPING 0.02 // how much to damp the cloth simulation each frame
#define CONSTRAINT_ITERATIONS 3 // how many iterations of constraint satisfaction each frame (more is rigid, less is soft)

//Used for calculating a Delta time (In seconds)
GLfloat g_OldTime;
GLfloat g_NewTime;
GLfloat g_DeltaTime;

Utils g_Util;

bool RenderBall = false;

int g_numOfHooks = 2;
int g_ParticlesWidthNum = 20;
int g_ParticlesHeightNum = 15;
int g_ClothWidth = 14;
int g_ClothHeight = 10;
Cloth* cloth1;


// Just below are three global variables holding the actual animated stuff; Cloth and Ball 
glm::vec3 ball_pos(7, -5, 0); // the center of our one ball
float ball_radius = 2; // the radius of our one ball

//Vars for wind direction, on/off etc
enum Wind_Direction {Up = 1, Down, Left, Right};
Wind_Direction Wind_Direction_Var = Down;
bool Wind_On = true;
float g_WindSpeed = 3.0f;

//Vars for hooks
int i_HookCount = 2;

//Below: keystate stuff from past projects


void init(GLvoid)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.2f, 0.2f, 0.4f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightPos[4] = { -1.0,1.0,0.5,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPos);

	glEnable(GL_LIGHT1);

	cloth1 = new Cloth(g_ClothWidth, g_ClothHeight, g_ParticlesWidthNum, g_ParticlesHeightNum, g_numOfHooks); // one Cloth object of the Cloth class

	GLfloat lightAmbient1[4] = { 0.0,0.0,0.0,0.0 };
	GLfloat lightPos1[4] = { 1.0,0.0,-0.2,0.0 };
	GLfloat lightDiffuse1[4] = { 0.5,0.5,0.3,0.0 };

	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

GLfloat ball_time = 0; // counter for used to calculate the z position of the ball below

					 /* display method called each frame*/
void render(void)
{
	GLfloat x_rand = 0.0f;
	GLfloat y_rand = 0.0f;
	GLfloat z_rand = 0.0f;
	// calculating positions	5:3
	if (Wind_On)
	{
		switch (Wind_Direction_Var)
		{
		case Up:

			x_rand = rand() % 20;
			x_rand /= -10.0f;
			z_rand = rand() % 12;
			z_rand /= -10.0f;
			std::cout << "x: " << x_rand << std::endl;
			std::cout << "z: " << z_rand << std::endl;
			break;
		case Down:
			x_rand = rand() % 20;
			x_rand /= 10.0f;
			z_rand = rand() % 12;
			z_rand /= 10.0f;
			std::cout << "x: " << x_rand << std::endl;
			std::cout << "z: " << z_rand << std::endl;
			break;
		default:
			break;
		}
	}
	

	cloth1->addForce(glm::vec3(0, -9.81f, 0)*g_DeltaTime * 0.75f); // add gravity each frame, pointing down
	cloth1->windForce(glm::vec3(x_rand, y_rand, z_rand)*g_DeltaTime * g_WindSpeed); // generate some wind each frame
	cloth1->timeStep(DAMPING, g_DeltaTime, CONSTRAINT_ITERATIONS); // calculate the particle positions of the next frame

	cloth1->groundCollision(-15.0f);

												 // drawing

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING); // drawing some smooth shaded background - because I like it ;)
	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.8f, 0.2f);
	glVertex3f(-200.0f, -100.0f, -100.0f);
	glVertex3f(200.0f, -100.0f, -100.0f);
	glColor3f(0.6f, 0.3f, 1.25f);
	glVertex3f(200.0f, 100.0f, -100.0f);
	glVertex3f(-200.0f, 100.0f, -100.0f);
	glEnd();
	glEnable(GL_LIGHTING);

	glTranslatef(-6.5, 6, -15.0f); // move camera out and center on the cloth
	//glRotatef(25, 0, 1, 0); // rotate a bit to see the cloth from the side
	cloth1->drawShaded(); // finally draw the cloth with smooth shading

	if (RenderBall)
	{
		ball_time += g_DeltaTime;
		ball_pos.z = cos(ball_time / 1.0f) * 10.0f;
		//ball_pos.z = cos(ball_time * (180.0f / 30.0f)) * 7;

		if (ball_pos.z <= -9.9f)
		{
			ball_time = 0.0f;
			ball_pos.z = 0.0f;
		}

		cloth1->ballCollision(ball_pos, ball_radius); // resolve collision with the ball

		glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
		glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z); // hence the translation of the sphere onto the ball position
		glColor3f(0.8f, 0.4f, 0.2f);
		glutSolidSphere(ball_radius - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
		glPopMatrix();
	
	}

	//Drawing Floor
	glPushMatrix();
	glTranslatef(0.0f, -90.2f, 0.0f); // hence the translation of the sphere onto the ball position
	glColor3f(0.15f, 0.4f, 0.35f);
	glutSolidCube(150.0f);
	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0)
		gluPerspective(80, (float)w, 1.0, 5000.0);
	else
		gluPerspective(80, (float)w / (float)h, 1.0, 5000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void update() {
	//Update Delta Time
	{
		g_NewTime = glutGet(GLUT_ELAPSED_TIME);
		g_DeltaTime = g_NewTime - g_OldTime;
		g_DeltaTime = g_DeltaTime / 1000;
		g_OldTime = g_NewTime;
	}

}


void keyboard(unsigned char key, int x, int y)
{//lower case ascii values
	switch (key) {
	case 27:
		exit(0);
		break;
	//Wind
	case 119: //w
		Wind_Direction_Var = Up;
		std::cout << "W - Pressed" << std::endl;
		break;
	case 115: //s
		Wind_Direction_Var = Down;
		std::cout << "S - Pressed" << std::endl;
		break;
	case 97: //a
		std::cout << "A - Pressed" << std::endl;
		//Increase Wind;
		if (g_WindSpeed < 25)
			g_WindSpeed++;
		break;
	case 100: //d
		std::cout << "D - Pressed" << std::endl;
		if (g_WindSpeed > 3)
			g_WindSpeed--;
		break;
	case 114: //r
		std::cout << "R - Pressed" << std::endl;
		delete cloth1;
		g_WindSpeed = 3.0f;
		g_numOfHooks = 2;
		RenderBall = false;
		cloth1 = new Cloth(g_ClothWidth, g_ClothHeight, g_ParticlesWidthNum, g_ParticlesHeightNum, g_numOfHooks); // one Cloth object of the Cloth class
		break;
	case 99: //c
		Wind_On = !Wind_On;
		break;
	case 61: // =
		std::cout << "= - Pressed" << std::endl;
		if (g_numOfHooks < g_ParticlesWidthNum) {
			g_numOfHooks++;
			delete cloth1;
			cloth1 = new Cloth(g_ClothWidth, g_ClothHeight, g_ParticlesWidthNum, g_ParticlesHeightNum, g_numOfHooks); // one Cloth object of the Cloth class
		}
		break;
	case 45: // -
		std::cout << "- - Pressed" << std::endl;
		if (g_numOfHooks > 2) {
			g_numOfHooks--;
			delete cloth1;
			cloth1 = new Cloth(g_ClothWidth, g_ClothHeight, g_ParticlesWidthNum, g_ParticlesHeightNum, g_numOfHooks); // one Cloth object of the Cloth class
		}
		break;

	case 32: // space
		std::cout << "Space - Pressed" << std::endl;
		cloth1->dropIt();
		break;

	case 44: // ,
		std::cout << ", - Pressed" << std::endl;
		cloth1->IncrementHookWidth(g_DeltaTime, 10.0f);
		break;

	case 46: // .
		std::cout << ". - Pressed" << std::endl;
		cloth1->DecrementHookWidth(g_DeltaTime, 10.0f);
		break;

	case 98: //b
		RenderBall = !RenderBall;
		std::cout << "B - Pressed" << std::endl;
		break;
	default:
		break;
	}
}

void arrow_keys(int a_keys, int x, int y)
{
	switch (a_keys) {
	case GLUT_KEY_DOWN:
		//glutFullScreen();
		if (g_ClothHeight < 15) {
			g_ClothHeight++;
			delete cloth1;
			cloth1 = new Cloth(g_ClothWidth, g_ClothHeight, g_ParticlesWidthNum, g_ParticlesHeightNum, g_numOfHooks); // one Cloth object of the Cloth class
		}
		break;
	case GLUT_KEY_UP:
		//glutReshapeWindow(1280, 720);
		if (g_ClothHeight > 10) {
			g_ClothHeight--;
			delete cloth1;
			cloth1 = new Cloth(g_ClothWidth, g_ClothHeight, g_ParticlesWidthNum, g_ParticlesHeightNum, g_numOfHooks); // one Cloth object of the Cloth class
		}
		break;
	case GLUT_KEY_RIGHT:
		if (g_ClothWidth < 20) {
			g_ClothWidth++;
			delete cloth1;
			cloth1 = new Cloth(g_ClothWidth, g_ClothHeight, g_ParticlesWidthNum, g_ParticlesHeightNum, g_numOfHooks); // one Cloth object of the Cloth class
		}
		break;
	case GLUT_KEY_LEFT:
		if (g_ClothWidth > 10) {
			g_ClothWidth--;
			delete cloth1;
			cloth1 = new Cloth(g_ClothWidth, g_ClothHeight, g_ParticlesWidthNum, g_ParticlesHeightNum, g_numOfHooks); // one Cloth object of the Cloth class
		}
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);

	glutCreateWindow("Cloth Simulation - Thomas Male  - Ben Hart");

	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);

	init();
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);
	glutIdleFunc(update);

	glutMainLoop();

	delete cloth1;
}