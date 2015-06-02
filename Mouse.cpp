#include <windows.h>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <queue>
#include <stack>
#include <iterator>
#include <math.h>
#include <cmath>
#include <tchar.h>
#include <strsafe.h>
#include <gl/glut.h>
#include "ParticleSystem.h"

using namespace std;

typedef struct _mouse_pos
{
	int x;
	int y;
}MOUSE_POS, *PMOUSE_pos;

static int g_x1,g_y1,g_x2,g_y2;
static int g_left_button_state=0;
static int g_right_button_state=0;

void draw();

double alpha = 1.0;
double slowdown = 1.0f;
GLuint	texture[1];

// Resource pool size
#define MAX_POOL 1000

/*
 * Desc - 
 * Build up a resources pool for check which particle obj is dead,
 * and then active it again.
 */
queue<CForMouseSimpleParticleSys*> g_render_queue;
queue<MOUSE_POS> g_mouse_pos_queue;

/*
 * Desc - 
 * Resources pools element node
 */
typedef struct _mouse_effects
{
	bool isActived;
	CForMouseSimpleParticleSys* st_mouse_effect;
	MOUSE_POS pos;
}MOUSE_EFFECT, *PMOUSE_EFFECT;

// Resources container
vector<MOUSE_EFFECT> g_vec_mouse_effect;

void CheckResourcesPool();

AUX_RGBImageRec *LoadBMP(char *Filename)					// Loads A Bitmap Image
{
        FILE *File=NULL;									// File Handle
        if (!Filename)										// Make Sure A Filename Was Given
        {
                return NULL;								// If Not Return NULL
        }
		fopen_s(&File, Filename, "r");							// Check To See If The File Exists
        if (File)											// Does The File Exist?
        {
			fclose(File);									// Close The Handle
			return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
        }
        return NULL;										// If Load Failed Return NULL
}

int LoadGLTextures()										// Load Bitmap And Convert To A Texture
{
        int Status=FALSE;									// Status Indicator
        AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Textures
        memset(TextureImage,0,sizeof(void *)*1);			// Set The Pointer To NULL

        if (TextureImage[0]=LoadBMP("Data/Particle.bmp"))	// Load Particle Texture
        {
			Status=TRUE;									// Set The Status To TRUE
			glGenTextures(1, &texture[0]);					// Create One Texture

			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 1, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        }

        if (TextureImage[0])							// If Texture Exists
		{
			if (TextureImage[0]->data)					// If Texture Image Exists
			{
				free(TextureImage[0]->data);			// Free The Texture Image Memory
			}
			free(TextureImage[0]);						// Free The Image Structure
		}
        return Status;									// Return The Status
}

void timer(int p)
{
	glutPostRedisplay();
	glutTimerFunc(20, timer, 0);
}

bool init_mouse_vec()
{
	bool result;
	MOUSE_EFFECT mouse_effect;

	for(int i = 0; i != MAX_POOL; i++)
	{
		mouse_effect.st_mouse_effect = new CForMouseSimpleParticleSys;
		if( !mouse_effect.st_mouse_effect )
			return false;

		result = mouse_effect.st_mouse_effect->Initialize(50, 0.0f, 1.0f, 0.0f,
														  2.0f, 100,
														  500, 500,
														  0.0f, 0.0f, 5.0f, 2.0f);
		if( !result )
			return false;

		mouse_effect.pos.x = 0;
		mouse_effect.pos.y = 0;

		mouse_effect.isActived = false;

		g_vec_mouse_effect.push_back(mouse_effect);
	}

	return true;
}

static bool init(void)
{
	bool result;

	// OpenGL interfaces
	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0,1280.0,0.0,720.0);

	::init_mouse_vec();

	for(vector<MOUSE_EFFECT>::iterator ite = ::g_vec_mouse_effect.begin();
		ite != ::g_vec_mouse_effect.end(); ite++)
	{
		result = ite->st_mouse_effect->LoadTexture("Data/Particle.bmp");
		if( !result )
			return false;
	}

	return true;
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(vector<MOUSE_EFFECT>::iterator ite = g_vec_mouse_effect.begin();
		ite != g_vec_mouse_effect.end(); ite++)
	{
		if(ite->isActived == true)
		{
			ite->st_mouse_effect->Ejector(ite->pos.x, 720 - ite->pos.y);
		}

		if(ite->st_mouse_effect->RenderFinish())
			ite->isActived = false;
	}

	glutPostRedisplay();
	glutSwapBuffers();
}

static void keyboardEvent(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_F1:
		break;

	default:
		break;
	}
}

void CheckResourcesPool()
{

	for(vector<MOUSE_EFFECT>::iterator ite = g_vec_mouse_effect.begin();
		ite != g_vec_mouse_effect.end(); ite++)
	{
		if( ite->isActived == false )
		{
			ite->isActived = true;
			ite->st_mouse_effect->Reset(true, 100, 20, 2.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 100, 100, 0, 0);
			ite->pos.x = g_x1;
			ite->pos.y = g_y1;
			break;
		}
	}
}

static void mouseMotionFunc(GLint x, GLint y)
{
	if(0 == g_left_button_state) return;

	g_x2=x;
	g_y2=y;

	CheckResourcesPool();
	draw();

	g_x1=g_x2;
	g_y1=g_y2;
}

static void mouseEvent(int button,int state,int x, int y)
{
	if(GLUT_LEFT_BUTTON==button)
	{
		switch(state)
		{
		case GLUT_DOWN:
			g_x1 = x;
			g_y1 = y;
			g_left_button_state=1;
			CheckResourcesPool();
			break;
		default:
			break;
		}
	}
}

void Resize(int width, int height)
{
	glutReshapeWindow(1280, 720);
}

void Shutdown()
{
	for(vector<MOUSE_EFFECT>::iterator ite = g_vec_mouse_effect.begin();
		ite != g_vec_mouse_effect.end(); ite++)
	{
		delete ite->st_mouse_effect;
		ite->st_mouse_effect = NULL;
	}
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(50,100);
	glutInitWindowSize(1280,720);
	glutCreateWindow("Test");
	glutReshapeFunc(Resize);
	init();

	glutTimerFunc(20, timer, 0);
	glutDisplayFunc(draw);
	glutSpecialFunc(keyboardEvent);
	glutMouseFunc(mouseEvent);
	glutMotionFunc(mouseMotionFunc);
	glutMainLoop();
	Shutdown();

	return 0;
}