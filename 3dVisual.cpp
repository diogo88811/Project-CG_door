
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/Glut.h>

//--------------------------------- Definir cores
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define ORANGE   1.0, 0.5, 0.1, 1.0
#define CYAN     0.0, 1.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.3, 0.3, 0.3, 1.0
#define PI		 3.14159

//================================================================================
//===========================================================Variaveis e constantes
GLint     msec = 10;
char      texto[30];						//.. texto a ser escrito no ecran


//-------------------- Porta
//

//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint		wScreen = 600, hScreen = 500;		//.. janela
GLfloat		yC = 100.0;	//.. Mundo

//------------------------------------------------------------ Observador 
GLfloat  rVisao = 8, aVisao = PI, incVisao = 0.05;
GLfloat  obsP[] = { rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao) };
GLfloat  obsT[] = { obsP[0] - rVisao * cos(aVisao), obsP[1], obsP[2] - rVisao * sin(aVisao) };

float anguloZ = 95;
int   tipoProjeccao = 0;

//--------------------------------------------- Escada
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

static GLfloat cor[] = { BLUE, BLUE, BLUE, BLUE, RED, RED, RED, RED, YELLOW, YELLOW, YELLOW ,YELLOW,RED,BLUE,RED,BLUE,BLUE,BLUE };
//gloabl variables
float centrox = 0;
float rotacao = 0;
float rotacaPortaPequena = 0;
GLboolean   frenteVisivel = 0;
GLint sempreRodar = 0;
GLint sempreRodar1 = 0;
GLfloat altura = 0;
GLfloat incALT = 0.01;

static GLuint     cima[] = { 8, 11, 10,  9 };   // regra da mao direita
static GLuint     baixo[] = { 13,14,15,12 };
static GLuint     atras[] = { 23,20,21,22 };
static GLuint     frente[] = { 17,16,19,18 };
static GLuint     direita[] = { 7, 6, 5, 4 };
static GLuint     esquerda[] = { 0,1,2,3 };

//================================================================================
//=========================================================================== INIT
void initialize(void)
{
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	glCullFace(GL_BACK);		//………………………………………………………………………………Mostrar so as da frente

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cor);
	glEnableClientState(GL_COLOR_ARRAY);

	srand(1);
}

float aleatorio(GLint minimo, GLint maximo) {
	GLfloat y;
	y = rand() % 1000;
	return (minimo + 0.001 * y * (maximo - minimo));
}

void desenhaTexto(char* string, GLfloat x, GLfloat y)
{
	glPushMatrix();
	glTranslatef(x, 0, y);
	glRasterPos2f(0, 0);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
	glPopMatrix();
}


void drawEixos()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo
	glColor4f(ORANGE);
	glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(10, 0, 0);
	glEnd();
	glColor4f(GREEN);
	glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 10, 0);
	glEnd();
	glColor4f(CYAN);
	glBegin(GL_LINES);
		glVertex3i(0, 0, 0);
		glVertex3i(0, 0, 10);
	glEnd();

}

void drawObservador_Local()
{
	glColor4f(GREEN);
	glPushMatrix();
		glTranslatef(obsP[0], obsP[1], obsP[2]);
		glutSolidCube(1);
	glPopMatrix();
}
void drawObservador_Target()
{
	glColor4f(YELLOW);
	glPushMatrix();
		glTranslatef(obsT[0], obsT[1], obsT[2]);
		glutSolidCube(0.8);
	glPopMatrix();
}

void draw_Informacao()
{
	if (tipoProjeccao)
		sprintf_s(texto, 30, "Projecao = Ortogonal");
	else
		sprintf_s(texto, 30, "Projecao = Perspectiva");
	desenhaTexto(texto, -15, -15);


}

void draw_cube() {
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, atras);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
}

//======================================
void drawScene() {
	//=================================================== Qual o lado visivel ???
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normais);
	*/
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
		if (rotacao > 0 && rotacao <= 90)
			rotacao = rotacao - 1;
	}
	if (sempreRodar1) {
		if (rotacao >= 0 && rotacao < 90)
			rotacao = rotacao + 1;
	}

	//glTranslatef(centrox, altura, 0.0);
	//glTranslatef(0.3/2, 6/2, 3/2);
	glRotatef(rotacao, 0.0, 1.0, 0.0);

	glPushMatrix();
	glTranslatef( 0.3*tam, 6*tam + 2, 3*tam);
	glScalef(0.3, 6, 3);
	draw_cube();
	glPopMatrix();

	
	//the small door to open 
	glPushMatrix();
	//glTranslatef(0.3 * tam, 2 * tam, 3 * tam);


	glTranslatef(0, 4 * tam, 0);
	glRotatef(rotacaPortaPequena, 0, 0, 1);
	glTranslatef(0.3 * tam, -2 * tam, 3 * tam);
	glScalef(0.3, 2, 1);
	draw_cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3 * tam, 2 * tam, 5 * tam);
	glScalef(0.3, 2, 1);
	draw_cube();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3 * tam, 2 * tam, tam);
	glScalef(0.3, 2, 1);
	draw_cube();
	glPopMatrix();

	

}




//====================================================== 
//====================================================== 
void display(void) {

	//================================================================= APaga 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//================================================================= Viewport 1 MAPA
	// projecao ortogonal
	glViewport(0, 0, 0.25 * wScreen, 0.25 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);
	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawObservador_Local();
	drawObservador_Target();
	drawScene();
	draw_Informacao();

	//================================================================= Viewport 2
	glViewport(0.25 * wScreen, 0.25 * hScreen, 0.75 * wScreen, 0.75 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (tipoProjeccao == 0)
		gluPerspective(anguloZ, (float)wScreen / hScreen, 0.1, 9999);
	if (tipoProjeccao == 1)
		glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], obsT[0], obsT[1], obsT[2], 0, 1, 0);

	//…………………………………………………………………………………………………………………………………………………………Objectos
	drawEixos();
	drawScene();


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();
}


void Timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(msec, Timer, 1);
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {


	switch (key) {
	
   //------------------------------ zoom
	case 'z': 	case 'Z':
		break;
	case 'x':	case 'X':
		break;

		//------------------------------ projeccao
	case 'p': 	case 'P':
		tipoProjeccao = (tipoProjeccao + 1) % 2;
		glutPostRedisplay();
		break;
	
		//--------------------------- Escape
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
			if (rotacaPortaPequena ==  0) {
				rotacaPortaPequena = 70;
			}
			else
				rotacaPortaPequena = 0;
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
			sempreRodar1 = 0;
			glutPostRedisplay();
			break;
		case 't':
		case 'T':
			sempreRodar1 = 1;
			sempreRodar = 0;
			glutPostRedisplay();
			break;

			//--------------------------- Escape
		case 27:
			exit(0);
			break;

	}

}



void teclasNotAscii(int key, int x, int y) {


	//------------------------------- olhar para a origem
		obsT[0] = 0; obsT[1] = 0; obsT[2] = 0;
		
		if (key == GLUT_KEY_UP)    obsP[1] = obsP[1] + 0.5;
		if (key == GLUT_KEY_DOWN)  obsP[1] = obsP[1] - 0.5;
		if (key == GLUT_KEY_LEFT)  aVisao = aVisao + 0.1;
		if (key == GLUT_KEY_RIGHT) aVisao = aVisao - 0.1;

		if (obsP[1] > yC)   obsP[1] = yC;
		if (obsP[1] < -yC)  obsP[1] = -yC;
		obsP[0] = rVisao * cos(aVisao);
		obsP[2] = rVisao * sin(aVisao);
		   
	    glutPostRedisplay();

}


//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("|Observador:'SETAS'|  |Projecao -'p'| |Open the Door- 'r''t'| |Small Door- 'e'| ");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}



