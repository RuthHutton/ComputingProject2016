#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include "Particle.h"
#include <ngl/Camera.h>
#include <ngl/BBox.h>
#include <ngl/Random.h>
#include <ngl/Light.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>



const static float INCREMENT=0.01;

const static float ZOOM=0.1;

NGLScene::NGLScene(int _numParticles)
{
  m_rotate=false;
  m_spinXFace=0;
  m_spinYFace=0;
  setTitle("Particles");
  m_numParticles =_numParticles;
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(QResizeEvent *_event )
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
  m_cam.setShape(45.0f,(float)width()/height(),0.05f,350.0f);
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_cam.setShape(45.0f,(float)_w/_h,0.05f,350.0f);
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
}


void NGLScene::initializeGL()
{
  ngl::NGLInit::instance();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  ngl::Vec3 from(0,5,18);
  ngl::Vec3 to(0,0,0);
  ngl::Vec3 up(0,1,0);
  m_cam.set(from,to,up);
  m_cam.setShape(40,(float)720.0/576.0,0.5,150);
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->createShaderProgram("Phong");
  shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
  shader->loadShaderSource("PhongVertex","shaders/Phong.vs");
  shader->loadShaderSource("PhongFragment","shaders/Phong.fs");
  shader->compileShader("PhongVertex");
  shader->compileShader("PhongFragment");
  shader->attachShaderToProgram("Phong","PhongVertex");
  shader->attachShaderToProgram("Phong","PhongFragment");
  shader->bindAttribute("Phong",0,"inVert");
  shader->bindAttribute("Phong",1,"inUV");
  shader->bindAttribute("Phong",2,"inNormal");
  shader->linkProgramObject("Phong");
  (*shader)["Phong"]->use();
  shader->setShaderParam1i("Normalize",1);

  ngl::Material m(ngl::STDMAT::SILVER);
  m.loadToShader("material");
  ngl::Light light(ngl::Vec3(1,10,20),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::LightModes::POINTLIGHT);
  ngl::Mat4 iv=m_cam.getViewMatrix();
  iv.transpose();
  light.setTransform(iv);
  light.setAttenuation(1,0,0);
  light.enable();
  light.loadToShader("light");

  m_wind.set(5,5,5);
  m_emitter.reset(new Emitter(ngl::Vec3(0,-5,0),1000,&m_wind));
  m_emitter->setCam(&m_cam);
  m_emitter->setShaderName("Phong");

  m_text.reset( new ngl::Text(QFont("Arial",10)));
  m_text->setScreenSize(width(),height());
  glViewport(0,0,width(),height());

  ngl::VAOPrimitives *prim =  ngl::VAOPrimitives::instance();
  prim->createSphere("sphere",0.05f,40.0f);
  m_particleTimer=startTimer(15);

  m_bbox.reset( new ngl::BBox(ngl::Vec3(300),1000.0f,1000.0f,1000.0f));

  m_bbox->setDrawMode(GL_LINE);

}

void NGLScene::loadMatricesToColourShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  (*shader)["nglColourShader"]->use();
  ngl::Mat4 MV;
  ngl::Mat4 MVP;

  MV= m_mouseGlobalTX*m_cam.getViewMatrix() ;
  MVP=MV*m_cam.getProjectionMatrix();
  shader->setShaderParamFromMat4("MVP",MVP);

}


void NGLScene::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);

  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  rotX.rotateX(m_spinXFace);
  rotY.rotateY(m_spinYFace);
  m_mouseGlobalTX=rotY*rotX;
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;

 ngl::ShaderLib *shader=ngl::ShaderLib::instance();
 (*shader)["nglColourShader"]->use();
 loadMatricesToColourShader();


 shader->use("nglDiffuseShader");

   for(Particle s : m_particleArray)
   {
       s.draw();
   }

  QString text;

  text.sprintf("Number of particles: %d",m_emitter->getNumParticles());

  m_text->setColour(1,1,1);

  m_text->renderText(10,10,text);
  m_text->renderText(10,30,QString("Space = clear all particles"));
  m_text->renderText(10,50,QString("1 = Add Particle"));
  m_text->renderText(10,70,QString("2 = Remove last particle"));
  m_text->renderText(10,90,QString("z,x,c,v change emit angle of sphere"));
  m_text->renderText(10,110,QString("Use arrow keys to change wind vector"));
  m_text->renderText(10,130,QString("Wind Vector  %1 %2 %3").arg(m_wind.m_x).arg(m_wind.m_y).arg(m_wind.m_z));
  m_text->renderText(10,150,QString("Press ESC to exit"));

  m_bbox->draw();
  m_emitter->draw();

 }

void NGLScene::updateScene()
{
    for(Particle &s : m_particleArray)
    {
        s.move();
    }
    checkCollisions();
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::addParticles()
{
    m_emitter->addParticle();
}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{

  if(m_rotate && _event->buttons() == Qt::LeftButton)
  {
    int diffx=_event->x()-m_origX;
    int diffy=_event->y()-m_origY;
    m_spinXFace += (float) 0.5f * diffy;
    m_spinYFace += (float) 0.5f * diffx;
    m_origX = _event->x();
    m_origY = _event->y();
    update();

  }
  else if(m_translate && _event->buttons() == Qt::RightButton)
  {
    int diffX = (int)(_event->x() - m_origXPos);
    int diffY = (int)(_event->y() - m_origYPos);
    m_origXPos=_event->x();
    m_origYPos=_event->y();
    m_modelPos.m_x += INCREMENT * diffX;
    m_modelPos.m_y -= INCREMENT * diffY;
    update();

   }
}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{
  if(_event->button() == Qt::LeftButton)
  {
    m_origX = _event->x();
    m_origY = _event->y();
    m_rotate =true;
  }
  else if(_event->button() == Qt::RightButton)
  {
    m_origXPos = _event->x();
    m_origYPos = _event->y();
    m_translate=true;
  }

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{
  if (_event->button() == Qt::LeftButton)
  {
    m_rotate=false;
  }
  if (_event->button() == Qt::RightButton)
  {
    m_translate=false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

	if(_event->delta() > 0)
	{
		m_modelPos.m_z+=ZOOM;
	}
	else if(_event->delta() <0 )
	{
		m_modelPos.m_z-=ZOOM;
	}
	update();
}


bool NGLScene::particleParticleCollision( ngl::Vec3 _pos1, GLfloat _radius1, ngl::Vec3 _pos2, GLfloat _radius2 )
{
  ngl::Vec3 relPos;
  GLfloat dist;
  GLfloat minDist;
  GLfloat len;
  relPos =_pos1-_pos2;
  len=relPos.length();
  dist=len*len;
  minDist =_radius1+_radius2;
  if(dist <=(minDist * minDist))
  {
    return true;
  }
  else
  {
    return false;
  }
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::BBoxCollision()
{
  float ext[6];
  ext[0]=ext[1]=(m_bbox->height()/2.0f);
  ext[2]=ext[3]=(m_bbox->width()/2.0f);
  ext[4]=ext[5]=(m_bbox->depth()/2.0f);

  ngl::Vec3 p;

  GLfloat D;
  for(Particle &s : m_particleArray)
  {
    p=s.getPos();

    for(int i=0; i<6; ++i)
    {

      D=m_bbox->getNormalArray()[i].dot(p);

      D+=s.getRadius();

      if(D >=ext[i])
      {

        GLfloat x= 2*( s.getDirection().dot((m_bbox->getNormalArray()[i])));
        ngl::Vec3 d =m_bbox->getNormalArray()[i]*x;
        s.setDirection(s.getDirection()-d);
        s.setHit();
      }
     }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void  NGLScene::checkParticleCollisions()
{
  bool collide;

  unsigned int size=m_particleArray.size();

    for(unsigned int ToCheck=0; ToCheck<size; ++ToCheck)
    {
        for(unsigned int Current=0; Current<size; ++Current)
        {
            if(ToCheck == Current)  continue;

      else
      {
        collide =particleParticleCollision(m_particleArray[Current].getPos(),m_particleArray[Current].getRadius(),
                                           m_particleArray[ToCheck].getPos(),m_particleArray[ToCheck].getRadius()
                                          );
        if(collide== true)
        {
          m_particleArray[Current].reverse();
          m_particleArray[Current].setHit();
        }
      }
    }
  }
}


void  NGLScene::checkCollisions()
{

    if(m_checkParticleParticle == true)
    {
        checkParticleCollisions();
    }
    BBoxCollision();

}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  switch (_event->key())
  {
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_F : showFullScreen(); break;

  case Qt::Key_Up : m_wind.m_y+=0.1; break;
  case Qt::Key_Down : m_wind.m_y-=0.1; break;
  case Qt::Key_Left : m_wind.m_x+=0.1; break;
  case Qt::Key_Right : m_wind.m_x-=0.1; break;
  case Qt::Key_I : m_wind.m_z+=0.1; break;
  case Qt::Key_O : m_wind.m_z-=0.1; break;
  case Qt::Key_W : m_wind.set(1,1,1); break;

  case Qt::Key_1 : addParticles(); break;

  case Qt::Key_Space : m_emitter->clearParticles(); break;
  case Qt::Key_2 : m_emitter->removeParticle(); break;

  default : break;
  }
    update();
}

void NGLScene::timerEvent(QTimerEvent *_event )
{
	if(_event->timerId() ==   m_particleTimer)
	{
		m_emitter->update();
		update();
	}

}
