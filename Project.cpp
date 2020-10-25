#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define PI		 3.14159

//================================================================================
//===========================================================Variaveis e constantes

GLfloat tam = 0.5;
static GLfloat vertices[] = {
	//…………………………………………………………………………………………………… x=tam (Esquerda)
		-tam,  -tam,  tam,	// 0 
		-tam,   tam,  tam,	// 1 
		-tam,   tam, -tam,	// 2 
		-tam,  -tam, -tam,	// 3 
	//…………………………………………………… Direita
		 tam,  -tam,  tam,	// 4 
		 tam,   tam,  tam,	// 5 
		 tam,   tam, -tam,	// 6 
		 tam,  -tam, -tam,	// 7 
	//……………………………………………………… (Cima
		-tam,  tam,  tam,	// 8 
		-tam,  tam, -tam,	// 9 
		 tam,  tam, -tam,	// 10 
		 tam,  tam,  tam,	// 11 
	
		// bottom

		-tam,  -tam,  tam, //12
		-tam,  -tam, -tam,	// 13
		tam,  -tam, -tam,	// 14
		tam,  -tam,  tam,	// 15

		//front
		-tam,  -tam,  tam,	// 16
		-tam,  tam,  tam,	// 17 
		tam,   tam,  tam,	// 18 
		tam,  -tam,  tam,	// 19
		//bback
		-tam,   tam, -tam,	// 20
		tam,   tam, -tam,	// 21
		tam,  -tam, -tam,	// 22
		-tam,  -tam, -tam,	// 23

};

static GLfloat normais[] = {
	//…………………………………………………………………………………………………… x=tam (Esquerda)
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  //…………………………………………………………………………………………………… x=tam (Direita)
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		//…………………………………………………………………………………………………… y=tam (Cima)
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,

	   //bottom
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,
	   
	   //front
	   0.0,   0.0,  1.0,
	   0.0,   0.0,  1.0,
	   0.0,   0.0,  1.0,
	   0.0,   0.0,  1.0,
	   //back
	   0.0,   0.0,  -1.0,
	   0.0,   0.0,  -1.0,
	   0.0,   0.0,  -1.0,
	   0.0,   0.0,  -1.0,
};
//------------------------------------------------------------ Cores
static GLfloat cor[] = { BLUE, BLUE, BLUE, BLUE, RED, RED, RED, RED, YELLOW, YELLOW, YELLOW ,YELLOW,RED,BLUE,RED,BLUE,BLUE,BLUE };
//gloabl variables
float centrox = 0;
float rotacao = 0;
GLboolean   frenteVisivel = 0;
GLint sempreRodar = 0;
GLfloat altura = 0;
GLfloat incALT = 0.01;


//=========================================================== FACES DA MESA

static GLuint     cima[] = { 8, 11, 10,  9 };   // regra da mao direita
static GLuint     baixo[] = { 13,14,15,12 };
static GLuint     atras[] = { 23,20,21,22};
static GLuint     frente[] = { 17,16,19,18};
static GLuint     direita[] = { 7, 6, 5, 4 };
static GLuint     esquerda[] = { 0,1,2,3 };


//------------------------------------------------------------ Objectos (sistema coordenadas)
GLint		wScreen = 800, hScreen = 600;			//.. janela (pixeis)
GLfloat		xC = 10.0, yC = 10.0, zC = 10.0;		//.. Mundo  (unidades mundo)

//------------------------------------------------------------ Visualizacao/Observador 
GLfloat  rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  angZoom = 90;
GLfloat  incZoom = 3;



//================================================================================
//=========================================================================== INIT
void inicializa(void)
{
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………VertexArrays: vertices + normais + cores
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cor);
	glEnableClientState(GL_COLOR_ARRAY);
}



void drawEixos()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
	glColor4f(RED);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();

}



void drawScene() {

	//=================================================== Qual o lado visivel ???
	if (frenteVisivel == 0)
	{
		glDisable(GL_CULL_FACE);
	}
	else {
		glEnable(GL_CULL_FACE);
	}

	if (frenteVisivel == 1)
		glCullFace(GL_BACK);

	if (frenteVisivel == 2)
		glCullFace(GL_FRONT);


	//==================================== MESA	
	// ?? escala, rotacao, translacao ??
	if (sempreRodar) {
		rotacao = rotacao + 1;
		altura = altura + incALT;
		if (altura > 1) incALT = -incALT;
		if (altura < -1) incALT = -incALT;
	}

	glTranslatef(centrox, altura, 0.0);
	glRotatef(rotacao, 0.0, 1.0, 0.0);
	glScalef(0.5, 6, 3);
	glPushMatrix();
	glScalef(2, 2, 2);
	// desenhar uma das faces da mesa
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima); 
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, atras);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);

	glPopMatrix();


}

void display(void) {

	//================================================================= APaga ecran e lida com profundidade (3D)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//================================================================= Não modificar !!!!!!!!!!!!
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3 * zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);
	//================================================================= Não modificar !!!!!!!!!!!!


	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawScene();

	//. . . . . . . . . . . . . . . . . . . . .  Actualizacao
	glutSwapBuffers();
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {


	switch (key) {
	case 'f':
	case 'F':
		// ???
		if (frenteVisivel == 2)
			frenteVisivel = 0;
		else
			frenteVisivel++;
		glutPostRedisplay();
		break;

	case 'A':
	case 'a':
		//??
		glutPostRedisplay();
		break;

	case 'S':
	case 's':
		//??
		glutPostRedisplay();
		break;

	case 'e':
	case 'E':
		//??
		glutPostRedisplay();
		break;

	case 'd':
	case 'D':
		//??
		glutPostRedisplay();
		break;

	case 'r':
	case 'R':
		sempreRodar = 1;
		glutPostRedisplay();
		break;

		//--------------------------- Escape
	case 27:
		exit(0);
		break;
	}

}


void teclasNotAscii(int key, int x, int y) {
	//.. observador pode andar à volda da cena  (setas esquerda / direita)
	//.. observador pode andar para cima e para baixo (setas cima / baixo )

	if (key == GLUT_KEY_UP)   obsP[1] = (obsP[1] + 0.1);
	if (key == GLUT_KEY_DOWN) obsP[1] = (obsP[1] - 0.1);
	if (key == GLUT_KEY_LEFT)  aVisao = (aVisao + 0.1);
	if (key == GLUT_KEY_RIGHT) aVisao = (aVisao - 0.1);

	if (obsP[1] > yC)   obsP[1] = yC;   // limita altura
	if (obsP[1] < -yC)  obsP[1] = -yC;

	obsP[0] = rVisao * cos(aVisao);      // actualiza posicao (X,Z)
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();
}


//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("{jh,ep}@dei.uc.pt|    |Mover/rodar Cima:'r'|   |FaceVisivel:'f'|      |Observador:'SETAS'|        |Andar-'a/s'|        |Rodar -'e/d'| ");

	inicializa();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}