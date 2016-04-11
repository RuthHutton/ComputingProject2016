#include <QMouseEvent>
#include <QGuiApplication>
#include <NGLscene.h>
#include <circle.h>
#include <ngl/NGLInit.h>
#include <ngl/Vec3.h>
#include <array>
#include <vector>
#include <iostream>
#include <ngl/Util.h>

NGLscene::NGLscene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Blank NGL");
}


NGLscene::~NGLscene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLscene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
}

void NGLscene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}

void NGLscene::createTriangle()
{
    std::vector<ngl::Vec3> tri;
    tri.push_back(ngl::Vec3(0.0f,0.0f,0.0f));
    for(int deg=0; deg<=360; deg+=10)
    {
        auto rad=ngl::radians(deg);
        tri.push_back(ngl::Vec3(0.8*cos(rad),0.8*sin(rad),0.0f));
    }
    m_size=tri.size();

    // create a VAO
    glGenVertexArrays(1,&m_vao);
    // tell open gl to make the VAO active
    glBindVertexArray(m_vao);
    // generate a Vertex Buffer
    GLuint buffer;
    glGenBuffers(1,&buffer);
    //bind to edit
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    // copy data to Open GL
    glBufferData(GL_ARRAY_BUFFER,
                 tri.size()*sizeof(ngl::Vec3), //size in bytes
                 &tri[0].m_x,
                 GL_STATIC_DRAW
                 );

    glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);



}

void NGLscene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  createTriangle();

}



void NGLscene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);

  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLE_FAN,0,m_size);

}

//----------------------------------------------------------------------------------------------------------------------
void NGLscene::mouseMoveEvent (QMouseEvent * _event)
{

}


//----------------------------------------------------------------------------------------------------------------------
void NGLscene::mousePressEvent ( QMouseEvent * _event)
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLscene::mouseReleaseEvent ( QMouseEvent * _event )
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLscene::wheelEvent(QWheelEvent *_event)
{

}
//----------------------------------------------------------------------------------------------------------------------

void NGLscene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_W : glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); break;
  case Qt::Key_S : glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}
