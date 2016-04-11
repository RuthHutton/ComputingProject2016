TARGET=PhysicsLibrary
QT+=testlib
CONFIG+=c++11
OBJECTS_DIR=obj

# .cpp files to be included
SOURCES+=$$PWD/src/main.cpp \
         $$PWD/src/circle.cpp \
         $$PWD/src/distance.cpp \
         $$PWD/src/timer.cpp \
         $$PWD/src/Emitter.cpp \
         $$PWD/src/NGLScene.cpp \
         $$PWD/src/Particle.cpp \
         $$PWD/src/ParticleFactory.cpp


# .h files to be included
HEADERS+=$$PWD/include/Emitter.h \
         $$PWD/include/circle.h \
         $$PWD/include/distance.h \
         $$PWD/include/timer.h \
         $$PWD/include/gravity.h \
         $$PWD/include/NGLScene.h \
         $$PWD/include/Particle.h \
         $$PWD/include/ParticleFactory.h


INCLUDEPATH+=$$PWD/include


# include UseNGL.pri to use NGL
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){
       message("including $HOME/NGL")
      include($(HOME)/NGL/UseNGL.pri)
}
else{
       message("Using custom NGL location")
      include($(NGLDIR)/UseNGL.pri)
    }
