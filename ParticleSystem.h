/*
 * File name - ParticleSystem.h
 * Author - Cui Xue
 * Updated - 10/18/14
 * Desc - 
 * 
 */
#include <vector>
#include <queue>
#include <iterator>
#include <gl\glut.h>
#include <gl\GLAux.h>
using namespace std;

#ifndef _H_PARTICLE_SYSTEM_
#define _H_PARTICLE_SYSTEM_

#pragma comment(lib, "glaux.lib")

/*
* Class name - CForMouseSimpleParticleSys
* Author - Cui Xue
* Updated - 10/18/14
* Desc - 
*/
class CForMouseSimpleParticleSys
{
private:
	
	// Particle's properties
	typedef struct _properties_of_particles
	{
		bool	active;					// Active (Yes/No)
		float	initSpeed;				// Original speed
		float	life;					// Particle Life (Alpha value)
		float	fade;					// Fade Speed
		float	size;					// Texture bias from the mouse pos
		float	r;						// Red Value
		float	g;						// Green Value
		float	b;						// Blue Value
		float	x;						// X Position
		float	y;						// Y Position
		float	x_direction;			// X Direction
		float	y_direction;			// Y Direction

		// External Force
		float	x_force_component;		// X gravity component
		float	y_force_component;		// Y gravity component
	}Particle;

public:
	// Obj constructor & destructor
	CForMouseSimpleParticleSys();
	CForMouseSimpleParticleSys(const CForMouseSimpleParticleSys&);
	~CForMouseSimpleParticleSys();

public:

	// Interfaces
	bool Initialize(int numParticles,			// Number of particles in one ejector obj
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
					float y_force_component,	// Impact a particle's motion in y axis
					float particle_size,		// Particle's size
					float slowDown);			// Slow down the motion velocity

	bool LoadTexture(char* pFilename);			// The texture of each particle
	bool Reset(bool active, float initSpeed,
			   int fadeSpeed,
			   float life, float x, float y,
			   float r, float g, float b,
			   int x_direction, 
			   int y_direction,
			   float x_force_component, 
			   float y_force_component);
												// Reset parameters as initial value
	void Ejector(int posX, int posY);			// Particles' ejector
	bool RenderFinish();						// To tell the main program if current ejector finish all particles rendering.
	void Shutdown();							// Release the particle system

private:

	//Kernels
	bool					_initialize(int _numParticles,			// Number of particles in one ejector obj);
										float _r,					// Red component of color for the particle
										float _g,					// Green component of color for the particle
										float _b,					// Blue component of color for the particle
										float _life,				/* It could be seen as the alpha component of color in the particle.
																	   When a particle is going to end, we can use it for reducing the 
																	   particle's color and let it be transparent, then disappear.*/
										int	  _fade_speed,			// How fast disappear of a particle
										int	  _x_direction,			// Particle's motion in x axis
										int	  _y_direction,			// Particle's motion in y axis
										float _x_force_component,	// Impact a particle's motion in x axis
										float _y_force_component,	// Impact a particle's motion in y axis
										float _particle_size,		// Particle's size
										float _slowDown);

	bool 					_reset(bool _active, float _initSpeed,
								   int _fade_speed,
								   float _life, float _x, float _y,
								   float _r, float _g, float _b,
								   int _x_direction, 
								   int _y_direction,
								   float _x_force_component, 
								   float _y_force_component);		// Reset particles' properties


	AUX_RGBImageRec*		_load_texture(char* pFilename);			
	void					_render(int posX, int posY);
	bool					_isFinished();
	void					_shutdown();

private:
	vector<Particle> m_particles_container;
	GLuint	m_texture[1];
	float	m_slowDown;	
	GLuint	m_col;		
	GLuint	m_delay;
};	

#endif