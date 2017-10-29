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
#define CONSTRAINT_ITERATIONS 10 // how many iterations of constraint satisfaction each frame (more is rigid, less is soft)

//Used for calculating a Delta time (In seconds)
GLfloat g_OldTime;
GLfloat g_NewTime;
GLfloat g_DeltaTime;

Utils g_Util;

// Just below are three global variables holding the actual animated stuff; Cloth and Ball 
Cloth cloth1(14, 10, 21, 15); // one Cloth object of the Cloth class
glm::vec3 ball_pos(7, -5, 0); // the center of our one ball
float ball_radius = 2; // the radius of our one ball

//Vars for wind direction, on/off etc
enum Wind_Direction {Up = 1, Down, Left, Right};
Wind_Direction Wind_Direction_Var = Down;
bool Wind_On = false;

//Vars for hooks
int i_HookCount = 2;

//Below: keystate stuff from past projects

/*#define BUTTON_UP 0
#define BUTTON_DOWN 1
unsigned char keyState[255];
void keyboard(unsigned char key, int x, int y) {
	keyState[key] = BUTTON_DOWN;
}
void keyboard_up(unsigned char key, int x, int y) {
	keyState[key] = BUTTON_UP;
}
if (keyState[(unsigned char)'2'] == BUTTON_DOWN)
{

}//*/

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

	GLfloat lightAmbient1[4] = { 0.0,0.0,0.0,0.0 };
	GLfloat lightPos1[4] = { 1.0,0.0,-0.2,0.0 };
	GLfloat lightDiffuse1[4] = { 0.5,0.5,0.3,0.0 };

	glLightfv(GL_LIGHT1, GL_POSITION, (GLfloat *)&lightPos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, (GLfloat *)&lightAmbient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, (GLfloat *)&lightDiffuse1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

float ball_time = 0; // counter for used to calculate the z position of the ball below

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
		//	x_rand = rand() % 20;
		//	x_rand /= -10;
			z_rand = rand() % 12;
			z_rand /= -10;
		//	std::cout << "x: " << x_rand << std::endl;
			std::cout << "z: " << z_rand << std::endl;
			break;
		case Down:
		//	x_rand = rand() % 20;
		//	x_rand /= 10;
			z_rand = rand() % 12;
			z_rand /= 10;
		//	std::cout << "x: " << x_rand << std::endl;
			std::cout << "z: " << z_rand << std::endl;
			break;
		case Left:
			x_rand = rand() % 20;
			x_rand /= -10;
			//y_rand = rand() % 12;
			//y_rand /= -10;
			std::cout << "x: " << x_rand << std::endl;
			//std::cout << "y: " << y_rand << std::endl;
			break;
		case Right:
			x_rand = rand() % 20;
			x_rand /= 10;
		//	y_rand = rand() % 12;
		//	y_rand /= 10;
			std::cout << "x: " << x_rand << std::endl;
		//	std::cout << "y: " << y_rand << std::endl;
			break;
		default:
			break;
		}
	}
	ball_time++;
	ball_pos.z = cos(ball_time / 50.0) * 7;

	//cloth1.addForce(Vec3(0, -0.2, 0)*TIME_STEPSIZE2); // add gravity each frame, pointing down
	//cloth1.windForce(glm::vec3(0.25, 0, 0.1)*g_DeltaTime); // generate some wind each frame
	cloth1.addForce(glm::vec3(0, -0.5, 0)*g_DeltaTime); // add gravity each frame, pointing down
	cloth1.windForce(glm::vec3(x_rand, y_rand, z_rand)*g_DeltaTime); // generate some wind each frame
	cloth1.timeStep(DAMPING, g_DeltaTime, CONSTRAINT_ITERATIONS); // calculate the particle positions of the next frame
	//cloth1.ballCollision(ball_pos, ball_radius); // resolve collision with the ball

												 // drawing

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_LIGHTING); // drawing some smooth shaded background - because I like it ;)
	glBegin(GL_POLYGON);
	glColor3f(0.8f, 0.8f, 1.0f);
	glVertex3f(-200.0f, -100.0f, -100.0f);
	glVertex3f(200.0f, -100.0f, -100.0f);
	glColor3f(0.4f, 0.4f, 0.8f);
	glVertex3f(200.0f, 100.0f, -100.0f);
	glVertex3f(-200.0f, 100.0f, -100.0f);
	glEnd();
	glEnable(GL_LIGHTING);

	glTranslatef(-6.5, 6, -9.0f); // move camera out and center on the cloth
	//glRotatef(25, 0, 1, 0); // rotate a bit to see the cloth from the side
	cloth1.drawShaded(); // finally draw the cloth with smooth shading

	//glPushMatrix(); // to draw the ball we use glutSolidSphere, and need to draw the sphere at the position of the ball
	//glTranslatef(ball_pos.x, ball_pos.y, ball_pos.z); // hence the translation of the sphere onto the ball position
	//glColor3f(0.4f, 0.8f, 0.5f);
	//glutSolidSphere(ball_radius - 0.1, 50, 50); // draw the ball, but with a slightly lower radius, otherwise we could get ugly visual artifacts of cloth penetrating the ball slightly
	//glPopMatrix();

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
		break;
	case 115: //s
		Wind_Direction_Var = Down;
		break;
	case 97: //a
		Wind_Direction_Var = Left;
		break;
	case 100: //d
		Wind_Direction_Var = Right;
		break;
	case 99: //c
		if (Wind_On)
		{
			Wind_On = false;
		}
		else
		{
			Wind_On = true;
		}
		break;
	//Hooks
	case 120: //x
		i_HookCount++;
		if (i_HookCount > 19)
		{
			i_HookCount = 19;
		}
		//above check is to make sure that at most there are 20 hooks. Since the below check does a - 1, cap is set at 19 because if my math isnt wrong 20 - 1 - 19 = 0
		//the - 1 isnt in atm but im leaving the cap at 19 incase its added later
		//for later ben: the reason nothing appears on the left side is because you never set that side to unmovable
		//find a way to make the code scan along the top row of the particle vector until it finds unmovable nodes
		//then find the mid point between that node and the initial node
		//you also need to have the code recognise that an odd number means that the middle needs an unmovable node
		//cloth1.getParticle(cloth1.getWidth() - (cloth1.getWidth() / (i_HookCount)), 0)->makeUnmovable();
		cloth1.DynamicHooks(i_HookCount, true);
		std::cout << "i_HookCount: " << i_HookCount << std::endl;

		break;
	case 122: //z
		i_HookCount--;
		if (i_HookCount < 2)
		{
			i_HookCount = 2;
		}
		cloth1.DynamicHooks(i_HookCount, false);
		std::cout << "i_HookCount: " << i_HookCount - 1 << std::endl;

		break;
	default:
		break;
	}
}

void arrow_keys(int a_keys, int x, int y)
{
	switch (a_keys) {
	case GLUT_KEY_UP:
		glutFullScreen();
		break;
	case GLUT_KEY_DOWN:
		glutReshapeWindow(1280, 720);
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
	init();
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(arrow_keys);
	glutIdleFunc(update);

	glutMainLoop();
}