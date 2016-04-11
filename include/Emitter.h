#ifndef EMITTER_H__
#define EMITTER_H__
#include <vector>
#include <ngl/Camera.h>
#include <ngl/Vec3.h>

#include "Particle.h"


class Emitter
{
public :

	Emitter( ngl::Vec3 _pos, int _numParticles, ngl::Vec3 *_wind );

	void update();
    void addParticle();
    void removeParticle();
    inline unsigned int getNumParticles()const {return m_numParticles;}
    void clearParticles();

	void draw();
  inline void setCam(ngl::Camera *_cam){m_cam=_cam;}
  inline ngl::Camera * getCam()const {return m_cam;}
  inline void setShaderName(const std::string &_n){m_shaderName=_n;}
  inline const std::string getShaderName()const {return m_shaderName;}
private :
	ngl::Vec3 m_pos;
	int m_numParticles;
	std::vector <Particle> m_particles;
	ngl::Vec3 *m_wind;
  std::string m_shaderName;
  ngl::Camera *m_cam;

};


#endif

