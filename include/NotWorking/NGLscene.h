#ifndef NGLSCENE_H__
#define NGLSCENE_H__
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include <QOpenGLWindow>

class NGLscene : public QOpenGLWindow
{
  public:

    NGLscene();

    ~NGLscene();

    void initializeGL();

    void paintGL();

private:

    void createTriangle();

    void resizeGL(QResizeEvent *_event);

    void resizeGL(int _w, int _h);


    void keyPressEvent(QKeyEvent *_event);

    void mouseMoveEvent (QMouseEvent * _event );

    void mousePressEvent ( QMouseEvent *_event);

    void mouseReleaseEvent ( QMouseEvent *_event );

    void wheelEvent( QWheelEvent *_event);

    int m_width;

    int m_height;

    GLuint m_vao;
    GLuint m_size;
};

#endif
