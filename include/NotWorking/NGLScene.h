#ifndef NGLSCENE_H__
#define NGLSCENE_H__
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include "Particle.h"
#include <QOpenGLWindow>
#include <vector>


class NGLScene : public QOpenGLWindow
{
  public:

    NGLScene();

    ~NGLScene();

    void initializeGL();

    void paintGL();

    std::vector <Particle *>m_particle;
#
    void timerEvent(QTimerEvent *_event);



private:

    void resizeGL(QResizeEvent *_event);
    void resizeGL(int _w, int _h);

    void keyPressEvent(QKeyEvent *_event);

    void mouseMoveEvent (QMouseEvent * _event );
    void mousePressEvent ( QMouseEvent *_event);
    void mouseReleaseEvent ( QMouseEvent *_event );

    void wheelEvent( QWheelEvent *_event);
    int m_width;
    int m_height;

    ngl::Camera m_camera;

};

#endif
