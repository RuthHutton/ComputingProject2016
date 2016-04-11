#ifndef PARTICLE_FACTORY_H__
#define PARTICLE_FACTORY_H__

#include "Particle.h"

class Emitter;

class ParticleFactory
{
    public :
        Particle *CreateParticle(
                ngl::Vec3 _pos,
                ngl::Vec3 *_wind,
                Emitter *_parent);

    private :

};

#endif

