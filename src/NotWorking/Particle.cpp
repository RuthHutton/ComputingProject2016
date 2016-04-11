#include "Particle.h"
#include <ngl/Random.h>
#include <ngl/NGLStream.h>
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>

Particle::Particle(ngl::Vec3 _startPos, const ngl::Camera *_cam)
{
m_pos = _startPos;
m_startPos = _startPos;
ngl::Random *rng = ngl::Random::instance();
m_maxLife = rng->randomPositiveNumber(30)+2;
m_colour = rng->getRandomColour();
m_dir = rng->getRandomNormalizedVec3();
m_camera=_cam;
}

void Particle::update()
{
    if (++m_life >= m_maxLife)
    {
        m_pos = m_startPos;
        m_life=0;

    }
    else
    {
        m_pos+=m_dir;
    }
}

void Particle::draw() const
{
//std::cout<<m_pos<<"\n";
ngl::Transformation tx;
tx.setPosition(m_pos);
ngl::ShaderLib *shader = ngl::ShaderLib::instance();
shader->use("nglColourShader");
shader->setUniform("Colour",m_colour);
shader->setUniform("MVP",tx.getMatrix()*m_camera->getVPMatrix());
ngl::VAOPrimitives::instance()->draw("sphere");
}
