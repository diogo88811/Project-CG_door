
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/Glut.h>
#include "RgbImage.h"
#include <GL\freeGlut.h>
void initMaterials(int material);

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

GLfloat whitePlasticAmb1[] = { 0.19125f, 0.0735f, 0.0225f, 1.0f };
GLfloat whitePlasticDif1[] = { 0.7038f, 0.27048f, 0.0828f, 1.0f };
GLfloat whitePlasticSpec1[] = { 0.256777f, 0.137622f, 0.086014f, 1.0f };
GLint whitePlasticCoef1 = 0.1 * 128;



//================================================================================
//===========================================================Variaveis e constantes
GLint     msec = 10;
char      texto[30];						//.. texto a ser escrito no ecran

GLint	  dim = 64;
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

//---------------------------------- Textures
GLuint   texture[6];
RgbImage imag;
GLint malha = 0;

//---------------------------------------------------- AMBIENTE - fixa
GLint   Dia = 1;     //:::   'D'  
GLfloat intensidadeDia = 0.8;
GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia,intensidadeDia, 1.0 }; //
//----------------------- Luz pontual eixo do y
GLint   ligaTeto = 1;		 //:::   'T'  
GLfloat intensidadeT = 0.3;  //:::   'I'  
GLint   luzR = 1;		 	 //:::   'R'  
GLint   luzG = 1;			 //:::   'G'  
GLint   luzB = 1;			 //:::   'B'  
GLfloat localPos[4] = { 1.0, 15, 2.0, 1.0 };
GLfloat localCorAmb[4] = { 0.5, 0.5, 0.5, 0.0 };
GLfloat localCorDif[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localCorEsp[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localAttCon = 1;
GLfloat localAttLin = 1;
GLfloat localAttQua = 1;

GLfloat   quadS = 5.0;
GLint     material = 1;

GLfloat transparente[4] = { 1,1,1, 0.2 };

//----------------- foco
bool 		Focos[] = { 1,1 };		//.. Dois Focos ligados ou desligados
GLfloat		aberturaFoco = 6;		//.. angulo inicial do foco
GLfloat		anguloINC = 3.0;		//.. incremento
GLfloat		anguloMIN = 3.0;		//.. minimo
GLfloat		anguloMAX = 70.0;		//.. maximo
//GLfloat Pos1[] = { 25, 5, 1, 1.0 };   // Foco 1
GLfloat Pos1[] = { 6, 4, 5, 1.0 };   // Foco 1
GLfloat Pos2[] = { 8.5, 5, 5, 1.0 };   // Foco 2 
//GLfloat Pos2[] = { -2.5, 3.5, -1.0, 1.0 };

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

static GLfloat texturas[] = {
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1 };

static GLfloat cor[] = { BLUE, BLUE, BLUE, BLUE, RED, RED, RED, RED, YELLOW, YELLOW, YELLOW ,YELLOW,RED,BLUE,RED,BLUE,BLUE,BLUE };
//gloabl variables
float centrox = 0;
float rotacao = 0;
float rotacaPortaPequena = 0;
float doorLocker = 0;
GLboolean   frenteVisivel = 0;
GLint sempreRodar = 0;
GLint sempreRodar1 = 0;

static GLuint     cima[] = { 8, 11, 10,  9 };   // regra da mao direita
static GLuint     baixo[] = { 13,14,15,12 };
static GLuint     atras[] = { 23,20,21,22 };
static GLuint     frente[] = { 17,16,19,18 };
static GLuint     direita[] = { 7, 6, 5, 4 };
static GLuint     esquerda[] = { 0,1,2,3 };

//================================================================================
//=========================================================================== INIT


//…………………………………………………………………………………………………………………………………………… LUZES
void initLights(void) {

	//…………………………………………………………………………………………………………………………………………… Ambiente
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

	//…………………………………………………………………………………………………………………………………………… Teto
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
}

void defineFoco() {
	GLfloat Foco_direccao[] = { -1, 0, 0, 0 };	//……… -Z
	//GLfloat Foco_direccao[] = { 0, 0, -1, 0 };	//……… -Z
	GLfloat Foco1_cor[] = { 1, 1,  1, 1 };	//……… Cor da luz 1
	GLfloat Foco2_cor[] = { 1, 0,  0, 1 };	//……… Cor da luz 2
	GLfloat Foco_ak = 1.0;
	GLfloat Foco_al = 0.05f;
	GLfloat Foco_aq = 0.0f;
	GLfloat Foco_Expon = 2.0;		// Foco, SPOT_Exponent

	glLightfv(GL_LIGHT2, GL_POSITION, Pos1);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Foco1_cor);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, Foco_Expon);

	glLightfv(GL_LIGHT1, GL_POSITION, Pos2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Foco2_cor);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, Foco_ak);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, Foco_al);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, Foco_aq);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, aberturaFoco);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, Foco_direccao);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, Foco_Expon);
}

void initTexturas() {

	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("plasticDoor.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	imag.LoadBmpFile("madeira.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	imag.LoadBmpFile("doorLocker.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());


	glGenTextures(1, &texture[4]);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	imag.LoadBmpFile("me.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());


	glGenTextures(1, &texture[5]);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	imag.LoadBmpFile("wallTexture.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());
}
void initialize(void)
{
	glClearColor(BLACK);		//………………………………………………………………………………Apagar
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Profundidade
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Interpolacao de cores	

	initTexturas();
	glEnable(GL_CULL_FACE);		//………………………………………………………………………………Faces visiveis
	glCullFace(GL_BACK);		//………………………………………………………………………………Mostrar so as da frente
	
	glEnable(GL_BLEND);


	initLights();
	//defineFoco();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	initMaterials(22);
	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
	glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cor);
	glEnableClientState(GL_COLOR_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texturas);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	srand(1);
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

void desenha()
{
	int				i, j;
	float			med_dim = (float)dim / 2;


	//----------------------------------------------- Dsenha malha poligonos
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glTranslatef(-6, 3, 0.4); // meio do poligono 

	glNormal3f(0, 0, 1); //normal
	if (malha) {
		glBegin(GL_QUADS);

		glTexCoord2f(0, 0);
		glVertex3d(0, 0, 0);
		glTexCoord2f(1, 0);
		glVertex3d(2, 0, 0);
		glTexCoord2f(1, 1);
		glVertex3d(2, 2, 0);
		glTexCoord2f(0, 1);
		glVertex3d(0, 2, 0);

		glEnd();
	}
	else {
		glBegin(GL_QUADS);
	for (i = 0; i < dim; i++)
		for (j = 0; j < dim; j++) {
			glTexCoord2f((float)j / dim, (float)i / dim);
			glVertex3d((float)j / med_dim, (float)i / med_dim, 0);
			glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
			glVertex3d((float)(j + 1) / med_dim, (float)i / med_dim, 0);
			glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
			glVertex3d((float)(j + 1) / med_dim, (float)(i + 1) / med_dim, 0);
			glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
			glVertex3d((float)j / med_dim, (float)(i + 1) / med_dim, 0);
		}
	glEnd();
	}
	

	
	glPopMatrix();
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


	//==================================== Door	
	// 

	/*if (sempreRodar) {
		if (rotacao > 0 && rotacao <= 90)
			rotacao = rotacao - 1;
	}
	if (sempreRodar1) {
		if (rotacao >= 0 && rotacao < 90)
			rotacao = rotacao + 1;
	}*/

	//glTranslatef(centrox, altura, 0.0);
	//glTranslatef(0.3/2, 6/2, 3/2);

	glMaterialfv(GL_FRONT, GL_AMBIENT, whitePlasticAmb1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whitePlasticDif1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whitePlasticSpec1);
	glMaterialf(GL_FRONT, GL_SHININESS, whitePlasticCoef1);

	glPushMatrix();
	glTranslatef(0.3 * tam, 20 * tam, 3 * tam);
	glRotatef(135, 0, 0, 1);
	glScalef(0.3, 2, 15);
	draw_cube();
	glPopMatrix();


	initMaterials(18);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glPushMatrix();
	glTranslatef (0, 0, 0);
	glTranslatef(0.3 * tam, 10 * tam , -6 * tam);
	glScalef(0.3, 10, 6);
	draw_cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0);
	glTranslatef(0.3 * tam, 10 * tam, 12 * tam);
	glScalef(0.3, 10, 6);
	draw_cube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0);
	glTranslatef(0.3 * tam, 14 * tam + 2, 3 * tam);;
	glScalef(0.3, 2, 3);
	draw_cube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	glPushMatrix();
	glTranslatef(1.0, 15, 2.0);
	draw_cube();
	glPopMatrix();

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	desenha();
	glPopMatrix();
	//glDisable(GL_TEXTURE_2D);


	glRotatef(rotacao, 0.0, 1.0, 0.0);

	

	initMaterials(18);
	glBlendFunc(GL_ONE, GL_ZERO);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTranslatef(0.3 * tam, 6 * tam + 2, 3 * tam);
	glScalef(0.3, 6, 3);
	draw_cube();
	
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	
	initMaterials(16);
	//the small door to open 
	glPushMatrix();
	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, transparente);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(0, 4 * tam, 0);
	glRotatef(rotacaPortaPequena, 0, 0, 1);
	glTranslatef(0.3 * tam, -2 * tam, 3 * tam);
	glScalef(0.3, 2, 1);
	draw_cube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	

	initMaterials(material);
	glBlendFunc(GL_ONE, GL_ZERO);
	glPushMatrix(); //door locker
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTranslatef(0, 10 * tam, (tam * 5) + doorLocker);
	glScalef(0.1, 0.1, 0.5);
	draw_cube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);



	initMaterials(18);
	glBlendFunc(GL_ONE, GL_ZERO);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTranslatef(0.3 * tam, 2 * tam, 5 * tam);
	glScalef(0.3, 2, 1);
	draw_cube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	initMaterials(18);
	

	glBlendFunc(GL_ONE, GL_ZERO);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTranslatef(0.3 * tam, 2 * tam, tam);
	glScalef(0.3, 2, 1);
	draw_cube();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	


}

void iluminacao() {
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
	if (ligaTeto)  glEnable(GL_LIGHT0);
	else   		  glDisable(GL_LIGHT0);
}

//====================================================== 
//====================================================== 
void display(void) {

	//================================================================= APaga 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//================================================================= Viewport 1 MAPA
	// projecao ortogonal
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glViewport(0, 0, 0.25 * wScreen, 0.25 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 20, 0, 10, 40, 10, 0, 0, -1);
	//…………………………………………………………………………………………………………………………………………………………Objectosgl
	glEnable(GL_LIGHTING);
	iluminacao();
	//defineFoco();
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
	//…………………………………………………………………………………………………………………………………………………………Objecto
	defineFoco();
	iluminacao();
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

void updateLuz() {
	localCorAmb[0] = luzR * intensidadeT;
	localCorAmb[1] = luzG * intensidadeT;
	localCorAmb[2] = luzB * intensidadeT;
	localCorDif[0] = luzR * intensidadeT;
	localCorDif[1] = luzG * intensidadeT;
	localCorDif[2] = luzB * intensidadeT;
	localCorEsp[0] = luzR * intensidadeT;
	localCorEsp[1] = luzG * intensidadeT;
	localCorEsp[2] = luzB * intensidadeT;;
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
}

//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y) {

	

	switch (key) {
	
	case 'k':
	case 'K':
		whitePlasticCoef1 = whitePlasticCoef1 + 5;
		if (whitePlasticCoef1 > 128)
			whitePlasticCoef1 = 0;
		printf("%d\n", whitePlasticCoef1);
		glutPostRedisplay();
		break;

	case 'n': 
	case 'N':
		if (malha == 0) {
			malha = 1;
			glutPostRedisplay();
		}
		else {
			malha = 0;
			glutPostRedisplay();
		}
		
		break;
	
	case 'h':
	case 'H':
		if (aberturaFoco < 15) {
			aberturaFoco = aberturaFoco + 0.5;
			glutPostRedisplay();
		}
		else {
			aberturaFoco = 5;
			glutPostRedisplay();
		}

		break;
	case 'o':
	case 'O':
		Focos[0] = !Focos[0];
		if (Focos[0] == 0)
			glDisable(GL_LIGHT2);
		else
			glEnable(GL_LIGHT2);
		glutPostRedisplay();
		break;
	case 'L':
	case 'l':
		Focos[1] = !Focos[1];
		if (Focos[1] == 0)
			glDisable(GL_LIGHT1);
		else
			glEnable(GL_LIGHT1);
		glutPostRedisplay();
		break;
	case 'v':case 'V':
		luzR = (luzR + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;
	case 'g':case 'G':
		luzG = (luzG + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;
	case 'b':case 'B':
		luzB = (luzB + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;
	case 'm': case 'M':
		material = (material + 1) % 18;
		initMaterials(material);
		glutPostRedisplay();
		break;

		//------------------------------ zoom
	case 'z': 	case 'Z':
		Dia = !Dia;
		if (Dia) { luzGlobalCorAmb[0] = 0.7;   luzGlobalCorAmb[1] = 0.7;   luzGlobalCorAmb[2] = 0.7; }
		else { luzGlobalCorAmb[0] = 0.2;   luzGlobalCorAmb[1] = 0.2;   luzGlobalCorAmb[2] = 0.2; }
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
		glutPostRedisplay();
		break;
	case 'x':	case 'X':
		ligaTeto = !ligaTeto;
		updateLuz();
		glutPostRedisplay();
		break;
	case 'i':	case 'I':
		intensidadeT = intensidadeT + 0.1;
		if (intensidadeT > 1.1) intensidadeT = 0;
		updateLuz();
		glutPostRedisplay();
		break;
		//------------------------------ projeccao
	case 'p': 	case 'P':
		tipoProjeccao = (tipoProjeccao + 1) % 2;
		glutPostRedisplay();
		break;

		//--------------------------- Escape
	case 'f':
	case 'F':
		if (frenteVisivel == 2)
			frenteVisivel = 0;
		else
			frenteVisivel++;
		glutPostRedisplay();
		break;

	case 'y':
	case 'Y':
		if (doorLocker == 0) {
			doorLocker = 0.5;
		}
		else {
			doorLocker = 0;
		}
		glutPostRedisplay();
		break;


	case 'e':
	case 'E':
		if (rotacaPortaPequena < 91) {
			rotacaPortaPequena += 0.5;
		}
		break;
		;
	case 'u':
	case 'U':
		if (rotacaPortaPequena > 0) {
			rotacaPortaPequena -= 0.5;
		}
		glutPostRedisplay();
		break;
	case 'r':
	case 'R':
		if (rotacao < 91) {
			rotacao += 0.5;
		}
		glutPostRedisplay();
		break;
	case 't':
	case 'T':
		if (rotacao >=0) {
			rotacao -= 0.5;
		}
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
	glutCreateWindow("|Observador:'SETAS'|  |Projecao -'p'| |Open the Door- 'r''t'| |Small Door- 'e'| |open and close the door lock 'y'| ");

	initialize();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(msec, Timer, 1);

	glutMainLoop();

	return 0;
}



