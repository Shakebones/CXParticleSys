/*
* File name - ParticleSystem.h
* Author - Cui Xue
* Updated - 10/18/14
* Desc - 
* 
*/
#include "ParticleSystem.h"

CForMouseSimpleParticleSys::CForMouseSimpleParticleSys()
{
}

CForMouseSimpleParticleSys::CForMouseSimpleParticleSys(const CForMouseSimpleParticleSys& source)
{
}

CForMouseSimpleParticleSys::~CForMouseSimpleParticleSys()
{

}

bool CForMouseSimpleParticleSys::Initialize(int numParticles,			// Number of particles in one ejector obj
											float r,					// Red component of color for the particle
											float g,					// Green component of color for the particle
											float b,					// Blue component of color for the particle
											float life,					/* It could be seen as the alpha component of color in the particle.
																		   When a particle is going to end, we can use it for reducing the 
																		   particle's color and let it be transparent, then disappear.*/
											float fade_speed,			// How fast disappear of a particle
											float x_direction,			// Particle's motion in x axis
											float y_direction,			// Particle's motion in y axis
											float x_force_component,	// Impact a particle's motion in x axis
											float y_force_component, 	// Impact a particle's motion in y axis)
											float particle_size,		// Particle's size
											float slowDown)				// Slow down the motion velocity
{
	bool result;
	result = this->_initialize(numParticles, r, g, b, life, fade_speed,
							   x_direction, y_direction, 
							   x_force_component, y_force_component, particle_size, slowDown);
	if( !result )
		return false;

	return true;
}

// OpenGL Interfaces in this function
bool CForMouseSimpleParticleSys::LoadTexture(char* pFilename)
{
	bool Status = false;								
	AUX_RGBImageRec *TextureImage[1];					
	memset(TextureImage, 0, sizeof(void*) * 1);			

	if ( TextureImage[0] = this->_load_texture("Data/Particle.bmp") )	
	{
		Status = true;

		// OpenGL interfaces
		glGenTextures(1, &this->m_texture[0]);	
		glBindTexture(GL_TEXTURE_2D, m_texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}

	if (TextureImage[0])							
	{
		if (TextureImage[0]->data)					
		{
			free(TextureImage[0]->data);			
		}
		free(TextureImage[0]);						
	}

	// OpenGL interfaces
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f,0.0f,0.0f,0.0f);					// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);							// Disable Depth Testing
	glEnable(GL_BLEND);									// Enable Blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Type Of Blending To Perform
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				// Really Nice Point Smoothing
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D, this->m_texture[0]);	// Select Our Texture

	return Status;									
}

bool CForMouseSimpleParticleSys::Reset(bool active, float initSpeed,
									   int fadeSpeed,
									   float life, float x, float y,
									   float r, float g, float b,
									   int x_direction, 
									   int y_direction,
									   float x_force_component, 
									   float y_force_component)
{
	bool result;

	result = this->_reset(active, initSpeed, fadeSpeed,
						  life, x, y, 
						  r, g, b, x_direction, y_direction,
						  x_force_component, y_force_component);
	if( !result )
		return false;

	return true;
}

void CForMouseSimpleParticleSys::Ejector(int posX, int posY)
{
	this->_render(posX, posY);
}

bool CForMouseSimpleParticleSys::RenderFinish()
{
	bool result;

	result = this->_isFinished();

	if( !result )
		return false;

	return true;
}

void CForMouseSimpleParticleSys::Shutdown()
{
	this->_shutdown();
}

bool CForMouseSimpleParticleSys::_initialize(int _numParticles,			// Number of particles in one ejector obj);)
											 float _r,					// Red component of color for the particle
											 float _g,					// Green component of color for the particle
											 float _b,					// Blue component of color for the particle
											 float _life,				/* It could be seen as the alpha component of color in the particle.
											 							   When a particle is going to end, we can use it for reducing the 
											 							   particle's color and let it be transparent, then disappear.*/
											 int   _fade_speed,			// How fast disappear of a particle
											 int   _x_direction,		// Particle's motion in x axis
											 int   _y_direction,		// Particle's motion in y axis
											 float _x_force_component,	// Impact a particle's motion in x axis
											 float _y_force_component,	// Impact a particle's motion in y axis
											 float _particle_size,		// Particle's size
											 float _slowDown)
{
	CForMouseSimpleParticleSys::Particle particle_properties;
	for(int i = 0; i != _numParticles; i++){
		this->m_particles_container.push_back(particle_properties);
	}

	for(vector<Particle>::iterator ite = this->m_particles_container.begin();
		ite != this->m_particles_container.end(); ite++){
			ite->active = false;
			ite->initSpeed = float(rand()%50);
			ite->x = 0.0f;
			ite->y = 0.0f;
			ite->r = _r;
			ite->g = _g;
			ite->b = _b;
			ite->size = _particle_size;
			ite->life = _life;
			ite->fade = float(rand()%_fade_speed) / 1000.0f + 0.02f;
			ite->x_direction = float(rand()%_x_direction) - _x_direction / 2;
			ite->y_direction = float(rand()%_y_direction) - _y_direction / 2;
			ite->x_force_component = _x_force_component;
			ite->y_force_component = _y_force_component;
	}

	this->m_slowDown = _slowDown;

	return true;
}

bool CForMouseSimpleParticleSys::_reset(bool _active, float _initSpeed,
										int _fade_speed,
										float _life, float _x, float _y,
										float _r, float _g, float _b,
										int _x_direction, int _y_direction,
										float _x_force_component, float _y_force_component)
{
	if( 0 == this->m_particles_container.size() )
		return false;

	for(vector<Particle>::iterator ite = this->m_particles_container.begin();
		ite != this->m_particles_container.end(); ite++)
	{
		ite->active = _active;
		ite->fade = float(rand()%_fade_speed) / 1000.0f + 0.02f;
		ite->initSpeed = float(rand()%(int)_initSpeed);
		ite->life = _life;
		ite->x = _x;
		ite->y = _y;
		ite->r = _r;
		ite->g = _g;
		ite->b = _b;
		ite->x_direction = float(rand()%_x_direction) - _x_direction / 2;
		ite->y_direction = float(rand()%_y_direction) - _y_direction / 2;
		ite->x_force_component = _x_force_component;
		ite->y_force_component = _y_force_component;
	}

	return true;
}

AUX_RGBImageRec* CForMouseSimpleParticleSys::_load_texture(char* pFilename)
{
	 FILE *File = NULL;								// File Handle
        if (!pFilename)								// Make Sure A Filename Was Given
        {
                return NULL;						// If Not Return NULL
        }

        fopen_s(&File, pFilename, "r");				// Check To See If The File Exists
        if (File)									// Does The File Exist?
        {
			fclose(File);							// Close The Handle
			return auxDIBImageLoad(pFilename);		// Load The Bitmap And Return A Pointer
        }

        return NULL;			
}

/* Function name - CForMouseSimpleParticleSys
 * Desc - 
 * The function describes how particles move in each frame
 * with its own life span
 */
void CForMouseSimpleParticleSys::_render(int posX, int posY)
{
	for(vector<Particle>::iterator ite = this->m_particles_container.begin();
		ite != this->m_particles_container.end(); ite++){
			if( ite->active == true ){
				float x = ite->x + posX;
				float y = ite->y + posY;

				glColor4f(ite->r, ite->g, ite->b, ite->life);					// OpenGL interfaces for setting the ready primitive's color

				glBegin(GL_TRIANGLE_STRIP);										// Draw in triangle strip approach
				glTexCoord2d(1,1);	glVertex2f(x + ite->size, y + ite->size);	// Right-Bottom vertex and texture coordinates
				glTexCoord2d(0,1);	glVertex2f(x - ite->size, y + ite->size);	// Left-Bottom vertex and texture coordinates
				glTexCoord2d(1,0);	glVertex2f(x + ite->size, y - ite->size);	// Right-Top vertex and texture coordinates
				glTexCoord2d(0,0);	glVertex2f(x - ite->size, y - ite->size);	// Left-Top vertex and texture coordinates
				glEnd();
				glFlush();

				ite->x_direction += ite->x_force_component;
				ite->y_direction += ite->y_force_component;
				ite->x += ite->x_direction / (this->m_slowDown * 1000) * ite->initSpeed * 0.25f;
				ite->y += ite->y_direction / (this->m_slowDown * 1000) * ite->initSpeed * 0.25f;
				ite->life -= ite->fade;
			}
	}
}

bool CForMouseSimpleParticleSys::_isFinished()
{
	for(vector<Particle>::iterator ite = this->m_particles_container.begin();
		ite != this->m_particles_container.end(); ite++)
	{
		if( ite->life >= 0.0f )
			return false;
	}

	return true;
}

void CForMouseSimpleParticleSys::_shutdown()
{
	this->m_particles_container.empty();
	this->m_particles_container.clear();
}