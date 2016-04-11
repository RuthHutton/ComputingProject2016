#include <iostream>
#include "timer.h"
#include <chrono>
#include <thread>
#include <QtGui/QGuiApplication>
#include <distance.h>
#include <circle.h>
#include <NGLscene.h>


int main(int argc, char **argv)

//CLOCK/TIMER
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

{
//Test if clock is working correctly - pause for x seconds

{

    for (int i = 0; i<10;i++)
    {
        std::cout<<i<<"\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

//DISTANCE
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

{
  Distance p1,p2,p3;
  p1.setCoords(7,8,3); //input coordinates
  p2.setCoords(1,1,1);

  p3=p1.minus(p2);

  std::cout<<"\n Particle 1 Coordinates are: "; p1.disp();
  std::cout<<"\n Particle 2 Coordinates are: "; p2.disp();
  std::cout<<"\n Distance between particle coordinates: "; p3.disp();

}

//CIRCLE
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//  Circle circleOBJ;

//  float rad;

//  std::cout<<"Enter a radius: "<<std::endl;
//  std::cin>>rad;

//  circleOBJ.setradius(rad);
//  circleOBJ.print();

////return 0;



//NGLscene
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

{
  QGuiApplication app(argc, argv);
  // create an OpenGL format specifier
  QSurfaceFormat format;
  // set the number of samples for multisampling
  // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
  format.setSamples(4);
  #if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(1);
  #else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(3);
  #endif
  // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
  format.setProfile(QSurfaceFormat::CoreProfile);
  // now set the depth buffer to 24 bits
  format.setDepthBufferSize(24);
  // now we are going to create our scene window
  NGLscene window;
  // and set the OpenGL format
  window.setFormat(format);
  // we can now query the version to see if it worked
  std::cout<<"Profile is "<<format.majorVersion()<<" "<<format.minorVersion()<<"\n";
  // set the window size
  window.resize(1024, 720);
  // and finally show
  window.show();

  return app.exec();
}
}
