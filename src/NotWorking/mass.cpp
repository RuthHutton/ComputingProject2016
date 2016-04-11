/*#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstdio>

#define screen_x 1024
#define screen_y 768

struct Particle {
    float x;
    float y;
    float r;
    float vx;
    float vy;
    float m;
    float color[3];
};

struct Line {
    float x1;
    float y1;
    float x2;
    float y2;
} line;

void timer(int = 0);
void display();
void mouse(int, int, int, int);
void mouseMotion(int, int);
void add_particle(float, float = 0, float = 0);
void remove_particles();
void keyboard(unsigned char, int, int);
void load();
void save();

int	mouse_x, mouse_y, WIN,
    mode=2,
    crit_mass=1,
    MAX_MASS=50000;
float cnst_elastic=0.75;
double z=0.0;
bool PRESSED_LEFT = false, PRESSED_RIGHT = false,
    PRESSED_MIDDLE = false, SPEED_PARTICLES = false;

std::vector<Particle> particles;

void main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(screen_x, screen_y);
    glutInitWindowPosition(50, 50);
    WIN = glutCreateWindow("Ultimate Gravity Simulator from the darkest depths of ûdun");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-screen_x/2, screen_x/2, screen_y/2, -screen_y/2,0, 1);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    timer();

    glutMainLoop();
}

void timer(int){
    display();
    if(PRESSED_LEFT && !SPEED_PARTICLES)
    {
        add_particle(5000);
        PRESSED_LEFT = false;
    }

    if(PRESSED_RIGHT)
    {
        int x=mouse_x;
        int y=mouse_y;
        for(int i=0;i<250;i++){
            mouse_x=rand()%250-125 + x;
            mouse_y=rand()%250-125 + y;
            add_particle(rand()%500+250);
        }
        PRESSED_RIGHT = false;
    }

    if(PRESSED_MIDDLE)
        remove_particles();

    for(int i = 0; i < particles.size(); i++){
        Particle &p = particles[i];
        bool not_fall = true;
        float tm,ax,ay;
        int id;
        for(int j = 0; j < particles.size(); j++)
        {
            if(j==i)
                continue;

            const Particle &p1 = particles[j];

            float d = sqrt((p1.x - p.x)*(p1.x - p.x) + (p1.y - p.y)*(p1.y - p.y));

            if(d > (p1.r+p.r))
            {
                p.vx += 0.01 * p1.m / (d*d) * (p1.x - p.x)/d;
                p.vy += 0.01 * p1.m / (d*d) * (p1.y - p.y)/d;
            }
            else{
                not_fall = false;
                tm=p1.m+p.m;
                id=j;
                ax=p1.vx;
                ay=p1.vy;
            }
        }

        if(mode==1){
            if(p.x<-screen_x/2){
                p.x=screen_x/2;
            }else if(p.x>screen_x/2){
                p.x=-screen_x/2;
            }

            if(p.y<-screen_y/2){
                p.y=screen_y/2;
            }else if(p.x>screen_y/2){
                p.y=-screen_y/2;
            }
        }else if(mode==2){
            if(p.x+p.vx<-screen_x/2 || p.x+p.vx>screen_x/2){
                p.vx*=(cnst_elastic)*-1;
                p.vy*=(cnst_elastic);
            }

            if(p.y+p.vy<-screen_y/2 || p.y+p.vy>screen_y/2){
                p.vx*=(cnst_elastic);
                p.vy*=(cnst_elastic)*-1;
            }
        }

        if(not_fall){
            p.x += p.vx;
            p.y += p.vy;
        }else{
            float x,y,r;
                mouse_x=p.x;
                mouse_y=p.y;
                ax+=p.vx;
                ay+=p.vy;
            if(i>id){
                particles.erase(particles.begin()+id);
                particles.erase(particles.begin()+i-1);
            }else
            {
                particles.erase(particles.begin()+id);
                particles.erase(particles.begin()+i);
            }
            add_particle(tm,ax,ay);
        }

        if(crit_mass==1){
            for(int i=0;i<particles.size();i++){
                if(particles[i].m>MAX_MASS){
                     int n=25,rmin=15,rmax=20,vel=125;
                     int p=particles[i].m;
                     p/=(n*10);
                     int x=particles[i].x;
                     int y=particles[i].y;
                     particles.erase(particles.begin()+i);
                     for(int i=0;i<n;i++){
                         mouse_x=x+((rand()/RAND_MAX)*(rmax-rmin)+rmin)*cos(i*M_PI/(n/2));
                         mouse_y=y+((rand()/RAND_MAX)*(rmax-rmin)+rmin)*sin(i*M_PI/(n/2));
                         add_particle(p,vel*cos(i*M_PI/(n/2)),vel*sin(i*M_PI/(n/2)));
                         add_particle(p,vel*cos(i*M_PI/(n/2)),vel*sin(i*M_PI/(n/2)));
                    }
                }
            }
        }
    }
    glutTimerFunc(1, timer, 0);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    //~ glColor3f(0, 0, 1);
    //~ glBegin(GL_LINES);
        //~ glVertex2f(line.x1, line.y1);
        //~ glVertex2f(line.x2, line.y2);
    //~ glEnd();

    for(int i = 0; i < particles.size(); i++){
        Particle &p = particles[i];
        glColor3f(p.color[0], p.color[1], p.color[2]);
        glBegin(GL_POLYGON);
        for(float a = 0; a < 2*M_PI; a+=0.25)
            glVertex2f(p.r*cos(a) + p.x, p.r*sin(a) + p.y);
        glEnd();
    }

    glFlush();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y){
    mouse_x = x - screen_x/2;
    mouse_y = y - screen_y/2;

    if(SPEED_PARTICLES){
        if(line.x2 != 0 && line.y2 != 0 && state == GLUT_UP && PRESSED_LEFT)
            add_particle(100,line.x1 - line.x2, line.y1 - line.y2);
        else{
            line.x1 = line.x2 = mouse_x;
            line.y1 = line.y2 = mouse_y;
        }
    }

    if(button == GLUT_LEFT_BUTTON)
        PRESSED_LEFT = state == GLUT_DOWN;
    else if(button == GLUT_RIGHT_BUTTON)
        PRESSED_RIGHT = state == GLUT_DOWN;
    else if(button == GLUT_MIDDLE_BUTTON)
        PRESSED_MIDDLE = state == GLUT_DOWN;
}

void mouseMotion(int x, int y){
    mouse_x = x - screen_x/2;
    mouse_y = y - screen_y/2;

    if(SPEED_PARTICLES && PRESSED_LEFT){
        line.x2 = mouse_x;
        line.y2 = mouse_y;
    }
}

void add_particle(float m,float vx, float vy){
    Particle p;
    p.x = mouse_x;
    p.y = mouse_y;
    p.vx = vx / 30;
    p.vy = vy / 30;
    p.m = m;
    p.r = pow((p.m/M_PI),0.175);

    p.color[0] = 1.0;
    p.color[1] = 1.0-p.m*1/MAX_MASS;
    p.color[2] = 0.0;

    particles.push_back(p);

    if(line.x1 != 0)
        line.x1 = line.x2 = line.y1 = line.y2 = 0;
}

void remove_particles(){
    for(int j=0;j<5;j++){
        for(int i = 0; i < particles.size(); i++)
            particles.pop_back();
    }
}

void save(){
    FILE *file=fopen("save.dat","wb");
    Particle p;
    for(int i=0;i<particles.size();i++){
        p=particles[i];
        fwrite(&p,sizeof(p),1,file);
    }
    fclose(file);
}

void load(){
    FILE *file=fopen("save.dat","rb");
    Particle p;
    remove_particles();
    while(!feof(file)){
        fread(&p,sizeof(p),1,file);
        if(!feof(file)){
            particles.push_back(p);
        }
    }
    fclose(file);
}

void keyboard(unsigned char key, int x, int y){
    int num=	25;
    int num2=	25;
    switch(key){
        case 'g':
            save();
            break;
        case 'h':
            load();
            break;
        case 's':
            SPEED_PARTICLES = !SPEED_PARTICLES;
            break;
        case 'c':
            remove_particles();
            remove_particles();
            remove_particles();
            remove_particles();
            remove_particles();
            remove_particles();
            break;
        case 'r':
            for(int i=0;i<250;i++){
                 mouse_x=rand()%screen_x-screen_x/2;
                 mouse_y=rand()%screen_y-screen_y/2;
                 add_particle(rand()%50+1);
            }
            break;
        case 'd':
            for(int i=0;i<particles.size();i++){
                 particles[i].vx=0;
                 particles[i].vy=0;
            }
            break;
        case '9':
            for(y=-50;y<50;y+=5){
                mouse_x=-100;
                mouse_y=y;
                add_particle(1);

                mouse_x=0;
                add_particle(1);

                mouse_y/=2;
                mouse_y+=50;
                add_particle(1);

                mouse_x=100;
                mouse_y-=10;
                add_particle(1);
            }

            for(x=0;x<20;x++){
                mouse_x=90+x;
                mouse_y=20;
                add_particle(1);
            }

            for(x=-100;x<-30;x+=5){
                mouse_y=50;
                mouse_x=x;
                add_particle(1);
            }

            for(int i=0;i<180;i+=5){
                mouse_x=25*sin(i*M_PI/180)+0;
                mouse_y=25*cos(i*M_PI/180)-25;
                add_particle(1);

                mouse_x=50*sin(-i*M_PI/180)+100;
                mouse_y=50*cos(i*M_PI/180);
                add_particle(1);
            }


            break;
        case '8':
            {
                for(int y=-50;y<50;y+=5){
                    mouse_x=-100;
                    mouse_y=y;
                    add_particle(1);

                    mouse_x=-150;
                    add_particle(1);

                    mouse_x=-80;
                    add_particle(1);

                    mouse_x=-10;
                    add_particle(1);

                    mouse_x=40;
                    add_particle(1);

                    mouse_x=60;
                    add_particle(1);

                    mouse_x=110;
                    add_particle(1);
                }

                for(int i=0;i<180;i+=10){
                    mouse_x=25*cos(i*M_PI/180)-125;
                    mouse_y=25*sin(i*M_PI/180)+50;
                    add_particle(1);

                    mouse_x+=140;
                    add_particle(1);

                    mouse_x=50*sin(i*M_PI/180)-80;
                    mouse_y=50*cos(i*M_PI/180);
                    add_particle(1);
                }

                int num_of_particles=25;
                int	xmin=60,
                    xmax=110;
                int	ymin=-50,
                    ymax=50;
                int dx=(xmax-xmin)/num_of_particles;
                int dy=(ymax-ymin)/num_of_particles;

                for(int i=0;i<num_of_particles;i++){
                    mouse_x=xmin+(dx*i);
                    mouse_y=ymin+(dy*i);
                    add_particle(1);
                }
            }
            break;
        case '7':
            {
                int radius=35;
                mouse_x=0;
                mouse_y=0;
                add_particle(5000);

                mouse_x=-radius;
                add_particle(25,0,mouse_x);

                mouse_x=radius;
                add_particle(25,0,mouse_x);

                mouse_x=0;
                mouse_y=-radius;
                add_particle(25,-mouse_y,0);

                mouse_y=radius;
                add_particle(25,-mouse_y,0);
            }
            break;// the rules
        case '6':
            {
                float r=250;
                for(int i=0;i<1000;i++,r-=0.25){
                    mouse_x=r*sin((i*360/125)*M_PI/180);
                    mouse_y=r*cos((i*360/125)*M_PI/180);
                    add_particle(5);
                }
            }
            break;
        case '5':
            for(int i=0,r=100;i<250;i++){
                mouse_x=r*sin((i*360/250)*M_PI/180);
                mouse_y=r*cos((i*360/250)*M_PI/180);
                add_particle(1);
            }

            for(int i=0,r=75;i<125;i++){
                mouse_x=r*cos((i*360/250)*M_PI/180);
                mouse_y=r*sin((i*360/250)*M_PI/180);
                add_particle(1);
            }

            for(int i=0,r=25;i<25;i++){
                mouse_x=r*sin((i*360/25)*M_PI/180);
                mouse_y=r*cos((i*360/25)*M_PI/180);
                mouse_x+=25;
                mouse_y-=25;
                add_particle(1);
            }

            for(int i=0,r=25;i<25;i++){
                mouse_x=r*sin((i*360/25)*M_PI/180);
                mouse_y=r*cos((i*360/25)*M_PI/180);
                mouse_x-=25;
                mouse_y-=25;
                add_particle(1);
            }

            break;
        case '4':
            for(int r=10;r<100;r+=10){
                for(int i=0;i<25;i++){
                    mouse_x=r*sin((i*360/25)*M_PI/180);
                    mouse_y=r*cos((i*360/25)*M_PI/180);
                    add_particle(5);
                }
            }
            break;//a leg
        case '3':
            for(int i=0,r=100;i<100;i++){
                mouse_x=r*sin((i*360/100)*M_PI/180);
                mouse_y=r*cos((i*360/100)*M_PI/180);
                add_particle(1);
            }
            break;//a leg
        case '2':
            for(int ix=0;ix<num;ix++){
                 mouse_x=ix*(screen_x/num)-screen_x/2;
                 for(int iy=0;iy<num2;iy++){
                     mouse_y=iy*(screen_y/num2)-screen_y/2;
                     add_particle(25);//,mouse_y*0.05,mouse_x*0.05);
                 }
            }
            break;
        case '1':
            for(int ix=0;ix<num*2;ix++){
                 mouse_x=ix*(screen_x/(num*2))-screen_x/2;
                 mouse_y=0;
                 add_particle(25,0,mouse_x*0.025);
            }
            break;
        case ' ':
            z+=0.0025;
            mouse_x=-screen_x/2;
            mouse_y=0;
            add_particle(500,250,sin(z*180/M_PI)*250);
            mouse_x=screen_x/2;
            add_particle(500,-250,sin(z*180/M_PI)*-250);
            break;
        case 27:
        case 'q':
            remove_particles();
            glutDestroyWindow(WIN);
            exit(0);
            break;
    }
}
*/
