//#include <QMouseEvent>

//#include "Particle.h"
//#include <ngl/Camera.h>
//#include "gravity.h"
//#include <ngl/Random.h>
//#include <ngl/Transformation.h>
//#include <ngl/ShaderLib.h>
//#include <ngl/VAOPrimitives.h>
//#include "Emitter.h"



//void Gravity::update()
//{
//    m_gravity=-9.81;

//    m_currentLife+=0.01;

//    m_pos.m_x=m_origin.m_x+(m_wind->m_x*m_dir.m_x*m_currentLife);
//    m_grav = (m_gravity*(m_currentLife*m_currentLife));
//    m_pos.m_y= m_origin.m_y+(m_wind->m_y*m_dir.m_y*m_currentLife)+m_grav;
//    m_pos.m_z=m_origin.m_z+(m_wind->m_z*m_dir.m_z*m_currentLife);

//    if(m_pos.m_y <= m_origin.m_y-0.01)
//    {
//        m_pos=m_origin;
//        m_currentLife=0.0;
//        ngl::Random *rand=ngl::Random::instance();
//        m_dir.m_x=rand->randomNumber(5)+1.0;
//        m_dir.m_y=rand->randomPositiveNumber(10)+1.0;
//        m_dir.m_z=rand->randomNumber(5)+1.0;
//    }
//}
