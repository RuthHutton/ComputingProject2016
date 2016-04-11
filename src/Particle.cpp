#include <QMouseEvent>

#include "Particle.h"
#include <ngl/Camera.h>
#include "NGLScene.h"
#include <ngl/Random.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include "Emitter.h"

/// @brief ctor
/// @param _pos the start position of the particle
Particle::Particle(ngl::Vec3 _pos, ngl::Vec3 *_wind,  Emitter *_parent   )
{
	m_pos=_pos;
	m_origin=_pos;
    m_wind=_wind;
    ngl::Random *rand=ngl::Random::instance();
	m_dir.m_x=rand->randomNumber(5)+0.5;
	m_dir.m_y=rand->randomPositiveNumber(10)+0.5;
	m_dir.m_z=rand->randomNumber(5)+0.5;
	m_currentLife=0.0;
    m_emitter=_parent;
    m_gravity = -9.81;
}


void Particle::update()

{

    m_currentLife+=0.01;

    m_pos.m_x=m_origin.m_x+(m_wind->m_x*m_dir.m_x*m_currentLife);
    m_grav = (m_gravity*(m_currentLife*m_currentLife));
    m_pos.m_y= m_origin.m_y+(m_wind->m_y*m_dir.m_y*m_currentLife)+m_grav;
    m_pos.m_z=m_origin.m_z+(m_wind->m_z*m_dir.m_z*m_currentLife);

    if(m_pos.m_y <= m_origin.m_y-0.01)
    {
        m_pos=m_origin;
        m_currentLife=0.0;
        ngl::Random *rand=ngl::Random::instance();
        m_dir.m_x=rand->randomNumber(5)+1.0;
        m_dir.m_y=rand->randomPositiveNumber(10)+1.0;
        m_dir.m_z=rand->randomNumber(5)+1.0;
    }

}

void Particle::keyPressEvent(QKeyEvent *_event)
{
  switch (_event->key())
  {
  case Qt::Key_G : m_grav = 0; break;

  default : break;
  }
    update();
}

void Particle::draw()
{
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::Transformation transform;
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->use(m_emitter->getShaderName());
  transform.setPosition(m_pos);
  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  M=transform.getMatrix();
  MV=transform.getMatrix()*m_emitter->getCam()->getViewMatrix();
  MVP=MV*m_emitter->getCam()->getProjectionMatrix() ;
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
  prim->draw("sphere");
}



void Particle::move()
{
  m_lastPos=m_pos;
  m_pos+=m_dir;
  m_nextPos=m_pos+m_dir;
  m_hit=false;
}



