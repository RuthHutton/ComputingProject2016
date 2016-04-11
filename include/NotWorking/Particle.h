#ifndef PARTICLE_H__
#define PARTICLE_H__
#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <ngl/Camera.h>

class Particle
{
public:

    Particle(ngl::Vec3 _startPos, const ngl::Camera *_cam);
    ~Particle() = default;
    void update();
    void draw() const;

private:

    ngl::Vec3 m_pos;
    ngl::Vec3 m_dir;
    ngl::Vec3 m_startPos;
    ngl::Colour m_colour;
    int m_life=0;
    int m_maxLife;
    const ngl::Camera *m_camera;

};

#endif
