#ifndef PARTICLE_H__
#define PARTICLE_H__

#include <ngl/Vec3.h>
#include <ngl/Colour.h>
#include <ngl/ShaderLib.h>
#include <QOpenGLWindow>
#include <ngl/Text.h>
#include <memory>


class Emitter;

class Particle
{
public :

	/// @brief ctor
	/// @param _pos the start position of the particle
    Particle(ngl::Vec3 _pos,ngl::Vec3 *_wind, Emitter *_parent	);
    inline ngl::Vec3 getPos() const {return m_pos;}
    inline GLfloat getRadius() const {return m_radius;}
    inline void setDirection(ngl::Vec3 _d){m_dir=_d;}
    inline ngl::Vec3 getDirection() const { return m_dir;}
    inline void reverse(){m_dir=m_dir*-1.0;}
    inline void setHit(){m_hit=true;}
    inline void setNotHit(){m_hit=false;}
	/// @brief a method to update the particle position
    void move();
	void update();
	/// @brief a method to draw the particle
	void draw();

private :
	/// @brief the curent particle position
	ngl::Vec3 m_pos;
    GLfloat m_radius;
    bool m_hit;
	/// @brief the original particle position
	ngl::Vec3 m_origin;
	/// @brief the direction vector of the particle
	ngl::Vec3 m_dir;
	/// @brief the current life value of the particle
	float m_currentLife;
    /// @brief gravity
    GLfloat m_gravity;
    GLfloat m_grav;
	/// @brief the wind vector
    ngl::Vec3 *m_wind;
    ngl::Vec3 m_lastPos;
    ngl::Vec3 m_nextPos;
    /// @brief a pointer to our emitter
    const Emitter *m_emitter;
    void keyPressEvent(QKeyEvent *_event);

};


#endif
