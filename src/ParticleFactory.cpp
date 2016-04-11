#include "ParticleFactory.h"
#include "Particle.h"

Particle * ParticleFactory::CreateParticle(
        ngl::Vec3 _pos,
        ngl::Vec3 *_wind,
        Emitter *_parent)

{
return new Particle(_pos, _wind, _parent);
}
