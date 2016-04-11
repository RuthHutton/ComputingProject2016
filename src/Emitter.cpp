#include "Emitter.h"
#include "ParticleFactory.h"


Emitter::Emitter(ngl::Vec3 _pos, int _numParticles, ngl::Vec3 *_wind )
{
	m_wind=_wind;
	for (int i=0; i< _numParticles; ++i)
	{
    m_particles.push_back(Particle(_pos,m_wind,this));
	}
	m_numParticles=_numParticles;
}

void Emitter::addParticle()
{
  ParticleFactory *p = new  ParticleFactory;



  delete p;
  ++m_numParticles;

  if(m_numParticles>1000)
      m_numParticles=1000;
  else
      m_numParticles=m_numParticles;
}

void Emitter::removeParticle()
{
  /// when we remove a particle we need to first check to see if we have any
  if(m_numParticles>0)
  {
    m_particles.pop_back();
    --m_numParticles;
  }
}



/// @brief a method to update each of the particles contained in the system
void Emitter::update()
{
	for(int i=0; i<m_numParticles; ++i)
	{
		m_particles[i].update();
	}
}
/// @brief a method to draw all the particles contained in the system
void Emitter::draw()
{
	for(int i=0; i<m_numParticles; ++i)
	{
		m_particles[i].draw();
	}


}

void Emitter::clearParticles()
{
  m_particles.clear();
  m_numParticles=0;
}
