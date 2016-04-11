#ifndef NGLSCENE_H__
#define NGLSCENE_H__
#include <ngl/Camera.h>
#include <ngl/BBox.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Text.h>
#include "Emitter.h"
#include <QOpenGLWindow>
#include "Particle.h"
#include <QTime>
#include <memory>


class NGLScene : public QOpenGLWindow
{
  Q_OBJECT
  public:

    NGLScene(int _numParticles);

    ~NGLScene();

    void initializeGL();
    void paintGL();
    void resizeGL(QResizeEvent *_event);
    void resizeGL(int _w, int _h);

private:

    int m_spinXFace;
    int m_spinYFace;
    bool m_rotate;
    bool m_translate;
    int m_origX;
    int m_origY;
    int m_origXPos;
    int m_origYPos;
    ngl::Mat4 m_mouseGlobalTX;
    int m_width;
    int m_height;
    ngl::Camera m_cam;
    std::vector <Particle> m_particleArray;
    std::unique_ptr<ngl::BBox> m_bbox;
    bool m_checkParticleParticle;
    ngl::Vec3 m_modelPos;
    std::unique_ptr<ngl::Text> m_text;
    int m_fpsTimer;
    std::unique_ptr<Emitter> m_emitter;
    int m_particleTimer;
    ngl::Vec3 m_wind;
    int m_numParticles;

    void updateScene();
    void addParticles();
    void clearParticles();
    void loadMatricesToShader();
    void loadMatricesToColourShader();
    void keyPressEvent(QKeyEvent *_event);
    void mouseMoveEvent (QMouseEvent * _event );
    void mousePressEvent ( QMouseEvent *_event);
    void mouseReleaseEvent ( QMouseEvent *_event );
    void wheelEvent( QWheelEvent *_event);
    void checkCollisions();
    void resetParticles();
    bool particleParticleCollision( ngl::Vec3 _pos1, GLfloat _radius1, ngl::Vec3 _pos2, GLfloat _radius2 );
    void BBoxCollision();
    void checkParticleCollisions();
    void timerEvent(QTimerEvent *);

};

#endif
