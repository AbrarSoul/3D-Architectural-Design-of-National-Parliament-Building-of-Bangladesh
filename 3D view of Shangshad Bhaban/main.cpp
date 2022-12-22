#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

using namespace std;

int slices,stacks,decRadi = 0,incRadi=0;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle,angle1,angle2,angle3,angle4,angle5;
double outer = 60;
double inner = 45;
double radius = 30;

struct point
{
    double x,y,z;
};
struct point2
{
    double x,y;
};

class vector3
{
public:
    double x,y,z;
    vector3(double a, double b, double c)
    {
        x = a;
        y=b;
        z=c;
    }
    vector3()
    {

    }
    int dot_product(vector3 p)
    {
        double dot = x*p.x + y*p.y + z*p.z;
        return (int)dot;
    }
    vector3 cross_product(vector3 p)
    {
        vector3 cross = vector3();
        cross.x = y*p.z - z*p.y;
        cross.y = z*p.x - x*p.z;
        cross.z = x*p.y - y*p.x;
        return cross;
    }
    void scale(double c)
    {
        x = x*c;
        y = y*c;
        z = z*c;
    }
    vector3 operator+(const vector3& p)
    {
        vector3 add;
        add.x = x + p.x;
        add.y = y + p.y;
        add.z = z + p.z;
        return add;
    }
    void translate(vector3 p)
    {
        this->x = x + p.x;
        this->y = y + p.y;
        this->z = z + p.z;
    }
    void normalize()
    {
        double temp = sqrt(x*x+y*y+z*z);
        this->x = this->x / temp;
        this->y = this->y / temp;
        this->z = this->z / temp;
    }
    vector3 perpendicular(vector3 p)
    {
        if(this->dot_product(p)!=0)
            return vector3();
        p.normalize();
        vector3 perp = p.cross_product(*this);
        //perp.normalize();
        return perp;
    }
    vector3 rotated_vector(vector3 p, double angle)
    {
        if(this->dot_product(p)!=0)
            return vector3();
        vector3 perp = this->perpendicular(p);
        angle = angle * pi / 180;
        perp.scale(sin(angle));
        this->scale(cos(angle));
        //cout<<perp.x<<" "<<perp.y<<" "<<perp.z<<endl;
        //cout<<x<<" "<<y<<" "<<z<<endl<<endl;
        perp = *this + perp;
        //cout<<perp.x<<" "<<perp.y<<" "<<perp.z<<endl;
        return perp;
    }

};

point pos = {100, 100, 0};
vector3 u(0, 0, 1);
vector3 r(-1/sqrt(2), 1/sqrt(2), 0);
vector3 l(-1/sqrt(2), -1/sqrt(2), 0);

point2 center={0,radius};

double theta = 100;
double rotate = 50;


void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void drawBalancedCube(double a)
{
    ///--------------------------front box--------------------------------------
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_QUADS);        /// frontside
    {
        glVertex3f( 58,a+58,20);
        glVertex3f( 58,a+58,0);
        glVertex3f(a+58,58,0);
        glVertex3f(a+58,58 ,20);
    }
    glEnd();
    glBegin(GL_QUADS);       /// backside
    {
        glVertex3f( 58,-a+58,20);
        glVertex3f( 58,-a+58,0);
        glVertex3f(-a+58,58,0);
        glVertex3f(-a+58, 58,20);
    }
    glEnd();
   glBegin(GL_QUADS);  /// rightside
    {
        glVertex3f( 58,a+58,20);
        glVertex3f( 58,a+58,0);
        glVertex3f(-a+58,58,0);
        glVertex3f(-a+58, 58,20);
    }
    glEnd();
    glBegin(GL_QUADS);      /// leftside
    {
        glVertex3f( 58,-a+58,20);
        glVertex3f( 58,-a+58,0);
        glVertex3f(a+58,58,0);
        glVertex3f(a+58, 58,20);
    }
    glEnd();
    glBegin(GL_QUADS);    /// upside
    {
        glVertex3f( 58,a+58,20);
        glVertex3f(-a+58, 58,20);
        glVertex3f( 58,-a+58,20);
        glVertex3f( a+58,58,20);
    }
    glEnd();
}

void drawRoofCube(double a)
{
    ///--------------------------vertical log--------------------------------------
    glColor3f(0.4,0.4,0.4);
    glBegin(GL_QUADS);        /// frontside
    {
        glVertex3f( 26,a+26,35);
        glVertex3f( 26,a+26,0);
        glVertex3f(a+26,26,0);
        glVertex3f(a+26, 26,35);
    }
    glEnd();
    glColor3f(0.4,0.4,0.4);
    glBegin(GL_QUADS);      /// leftside
    {
        glVertex3f( -26,-(a+26),35);
        glVertex3f( -26,-(a+26),0);
        glVertex3f(a+26,26,0);
        glVertex3f(a+26, 26,35);
    }
    glEnd();
    glBegin(GL_QUADS);  /// rightside
    {
        glVertex3f( 26,a+26,35);
        glVertex3f(26,a+26,0);
        glVertex3f(-(a+26),-26,0);
        glVertex3f(-(a+26), -26,35);
    }
    glEnd();
    glBegin(GL_QUADS);        /// backside
    {
        glVertex3f( -26,-(a+26),35);
        glVertex3f( -26,-(a+26),0);
        glVertex3f(-(a+26),-26,0);
        glVertex3f(-(a+26), -26,35);
    }
    glEnd();
    /*glBegin(GL_QUADS);    /// upside
    {
        glVertex3f( 26,a+26,35);
        glVertex3f( -(a+26),-26,35);
        glVertex3f(-26,-(a+26),35);
        glVertex3f( a+26,26,35);
    }
    glEnd();*/

    /*glBegin(GL_QUADS);       /// backside
    {
        glVertex3f( 26,-a+26,35);
        glVertex3f( 26,-a+26,0);
        glVertex3f(-a+26,26,0);
        glVertex3f(-a+26, 26,35);
    }
    glEnd();*/
    ///--------------------------horizontal log--------------------------------------

    glColor3f(0.4,0.4,0.4);
    glBegin(GL_QUADS);            /// front
    {
        glVertex3f( -30,a+27,35);
        glVertex3f( -30,a+27,0);
        glVertex3f(a+27,-30,0);
        glVertex3f(a+27, -30,35);
    }
    glEnd();
    glColor3f(0.3,0.3,0.3);
    glBegin(GL_QUADS);            /// back
    {
        glVertex3f( 30,-(a+27),35);
        glVertex3f( 30,-(a+27),0);
        glVertex3f(-(a+27),30,0);
        glVertex3f(-(a+27),30,35);
    }
    glEnd();
    glBegin(GL_QUADS);            /// left
    {
        glVertex3f( 30,-(a+27),35);
        glVertex3f( 30,-(a+27),0);
        glVertex3f(a+27,-30,0);
        glVertex3f(a+27, -30,35);
    }
    glEnd();
    glBegin(GL_QUADS);            /// right
    {
        glVertex3f( -30,a+27,35);
        glVertex3f( -30,a+27,0);
        glVertex3f(-(a+27),30,0);
        glVertex3f(-(a+27),30,35);
    }
    glEnd();



    ///--------------------------------corner-----------------------------------------------
     glColor3f(0.4,0.4,0.4);
     glBegin(GL_QUADS);       /// right front corner er front side
    {
        glVertex3f( a-3,43,35);
        glVertex3f( a-3,43,0);
        glVertex3f(-a+3,43,0);
        glVertex3f(-a+3,43,35);
    }
    glEnd();
    glColor3f(0.3,0.3,0.3);
     glBegin(GL_QUADS);       /// left
    {
        glVertex3f( -a+3,-43,35);
        glVertex3f( -a+3,-43,0);
        glVertex3f(-a+3,43,0);
        glVertex3f(-a+3,43,35);
    }
    glEnd();
     glBegin(GL_QUADS);       /// right
    {
        glVertex3f( a-3,43,35);
        glVertex3f( a-3,43,0);
        glVertex3f(a-3,-43,0);
        glVertex3f(a-3,-43,35);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);       /// left back corner er front side
    {
        glVertex3f( -a+3,-43,35);
        glVertex3f( -a+3,-43,0);
        glVertex3f(a-3,-43,0);
        glVertex3f(a-3,-43,35);
    }
    glEnd();

glColor3f(0.4,0.4,0.4);
     glBegin(GL_QUADS);       /// left front corner er front side
    {
        glVertex3f( 43,a-3,35);
        glVertex3f( 43,a-3,0);
        glVertex3f(43,-a+3,0);
        glVertex3f(43,-a+3,35);
    }
    glEnd();
    glColor3f(0.3,0.3,0.3);
     glBegin(GL_QUADS);       /// left
    {
        glVertex3f( -43,-(a-3),35);
        glVertex3f( -43,-(a-3),0);
        glVertex3f(43,-a+3,0);
        glVertex3f(43,-a+3,35);
    }
    glEnd();
    glBegin(GL_QUADS);       /// right
    {
        glVertex3f( 43,a-3,35);
        glVertex3f( 43,a-3,0);
        glVertex3f(-43,-(-a+3),0);
        glVertex3f(-43,-(-a+3),35);
    }
    glEnd();
glColor3f(0.6,0.6,0.6);
     glBegin(GL_QUADS);       /// right back corner er front side
    {
        glVertex3f( -43,-(a-3),35);
        glVertex3f( -43,-(a-3),0);
        glVertex3f(-43,-(-a+3),0);
        glVertex3f(-43,-(-a+3),35);
    }
    glEnd();
}

void drawCylinderCube()
{
    glColor3f(0.4,0.4,0.4);
    glBegin(GL_QUADS);            /// front
    {
        glVertex3f(49,-40,20);
        glVertex3f(49,-40,0);
        glVertex3f(78,-70,0);
        glVertex3f(78,-70,20);
    }
    glEnd();

    glBegin(GL_QUADS);            /// back
    {
        glVertex3f(40,-49,20);
        glVertex3f(40,-49,0);
        glVertex3f(70,-78,0);
        glVertex3f(70,-78,20);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
   glBegin(GL_QUADS);            /// up
    {
        glVertex3f(49,-40,20);
        glVertex3f(78,-70,20);
        glVertex3f(70,-78,20);
        glVertex3f(40,-49,20);

    }
    glEnd();
}

void drawRightBox()
{
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);            /// front
    {
        glVertex3f(-49,36,20);
        glVertex3f(-49,36,0);
        glVertex3f(-80,70,0);
        glVertex3f(-80,70,20);
    }
    glEnd();
glColor3f(0.5,0.5,0.5);
    glBegin(GL_QUADS);            /// back
    {
        glVertex3f(-36,49,20);
        glVertex3f(-36,49,0);
        glVertex3f(-70,80,0);
        glVertex3f(-70,80,20);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);            /// right
    {
        glVertex3f(-80,70,20);
        glVertex3f(-80,70,0);
        glVertex3f(-70,80,0);
        glVertex3f(-70,80,20);
    }
    glEnd();
    glBegin(GL_QUADS);            /// up
    {
        glVertex3f(-80,70,20);
        glVertex3f(-49,38,20);
        glVertex3f(-38,49,20);
        glVertex3f(-70,80,20);
    }
    glEnd();



}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
   glBegin(GL_QUADS);
    {
        glVertex3f(a,a, 0);
        glVertex3f(-a,a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(a,-a, 0);

    }
    glEnd();
}
void drawnew(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {
        //create shading effect
        if(i<segments/2)    shade=2*(double)i/(double)segments;
        else    shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,height);
            glVertex3f(points[i].x,points[i].y,height);
        }
        glEnd();
    }
}

void drawCube(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);       /// left front
    {
        glVertex3f(50,a+30, a);
        glVertex3f(50,a+30,0);
        glVertex3f(50,-a-30,0);
        glVertex3f(50,-a-30, a);
    }
    glEnd();

     glBegin(GL_QUADS);       /// right front
    {
        glVertex3f( a+30,50,a);
        glVertex3f( a+30,50,0);
        glVertex3f(-a-30,50,0);
        glVertex3f(-a-30,50,a);
    }
    glEnd();
    glBegin(GL_QUADS);          /// left behind
    {
        glVertex3f(a+30,-50,a);
        glVertex3f(a+30,-50,0);
        glVertex3f(-a-30,-50,0);
        glVertex3f(-a-30,-50,a);
    }
    glEnd();

    glBegin(GL_QUADS);          /// right behind
    {
        glVertex3f(-50,a+30,a);
        glVertex3f(-50,a+30,0);
        glVertex3f(-50,-a-30,0);
        glVertex3f(-50,-a-30,a);
    }
    glEnd();

    glBegin(GL_QUADS);        /// ground
    {
        glVertex3f(50,50,0);
        glVertex3f(-50,50,0);
        glVertex3f(-50,-50,0);
        glVertex3f(50,-50,0);

    }
    glEnd();
    glColor3f(0.7,0.7,0.7);
    glBegin(GL_QUADS);          /// up
    {
        glVertex3f(50,50,a);
        glVertex3f(-50,50,a);
        glVertex3f(-50,-50,a);
        glVertex3f(50,-50,a);

    }
    glEnd();
}

void drawBox1(double a)
{

    glColor3f(0.6,0.6,0.6);
     glBegin(GL_QUADS);          /// front er left box1 er front side
    {
        glVertex3f(80,a+8, a);
        glVertex3f(80,a+8,0);
        glVertex3f(80,-a+18,0);
        glVertex3f(80,-a+18, a);
    }
    glEnd();
    glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(81,a,18);
                glVertex3f(81,a-3,10);
                glVertex3f(81,a+3,10);
            }
            glEnd();
            glBegin(GL_QUADS);
            {
                glVertex3f(81,a+3,9);
                glVertex3f(81,a-3,9);
                glVertex3f(81,a-3,1);
                glVertex3f(81,a+3,1);
            }
            glEnd();
    glColor3f(0.5,0.5,0.5);
     glBegin(GL_QUADS);             /// front er left box1 er right side
    {
        glVertex3f(a+60,28, a);
        glVertex3f(a+60,28,0);
        glVertex3f(-a+70,28,0);
        glVertex3f(-a+70,28, a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);             /// front er left box1 er left side
    {
        glVertex3f(a+60,-2, a);
        glVertex3f(a+60,-2,0);
        glVertex3f(-a+70,-2,0);
        glVertex3f(-a+70,-2, a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);             /// front er left box1 roof
    {
        glVertex3f(a+60,28, a);
        glVertex3f(a+60,-2, a);
        glVertex3f(-a+70,-2,0);
        glVertex3f(-a+70,28, a);
    }
    glEnd();
    ///-----------------------------------------------------------------------
     glBegin(GL_QUADS);          /// front er left box2 er front side
    {
        glVertex3f(80,a-25, a);
        glVertex3f(80,a-25,0);
        glVertex3f(80,-a-15,0);
        glVertex3f(80,-a-15, a);
    }
    glEnd();
    glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(81,-a-7,18);
                glVertex3f(81,-a-10,10);
                glVertex3f(81,-a-4,10);
            }
            glEnd();
            glBegin(GL_QUADS);
            {
                glVertex3f(81,-a-4,9);
                glVertex3f(81,-a-10,9);
                glVertex3f(81,-a-10,1);
                glVertex3f(81,-a-4,1);
            }
            glEnd();
    glColor3f(0.3,0.3,0.3);
     glBegin(GL_QUADS);          /// front er left box2 er right side
    {
        glVertex3f(80,a-25, a);
        glVertex3f(80,a-25,0);
        glVertex3f(50,a-25,0);
        glVertex3f(50,a-25, a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
     glBegin(GL_QUADS);          /// front er left box2 er left side
    {
        glVertex3f(80,a-55, a);
        glVertex3f(80,a-55,0);
        glVertex3f(50,a-55,0);
        glVertex3f(50,a-55, a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
     glBegin(GL_QUADS);          /// front er left box2 er roof
    {
        glVertex3f(80,a-25, a);
        glVertex3f(80,a-55, a);
        glVertex3f(50,a-55, a);
        glVertex3f(50,a-25, a);
    }
    glEnd();


    ///----------------------------------------------------------------------
     glBegin(GL_QUADS);           /// front er right box1 er front side
    {
        glVertex3f(a+8,80, a);
        glVertex3f(a+8,80,0);
        glVertex3f(-a+18,80,0);
        glVertex3f(-a+18,80, a);
    }
    glEnd();
    glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(-a-7,81,18);
                glVertex3f(-a-10,81,10);
                glVertex3f(-a-4,81,10);
            }
            glEnd();
            glBegin(GL_QUADS);
            {
                glVertex3f(-a-4,81,9);
                glVertex3f(-a-10,81,9);
                glVertex3f(-a-10,81,1);
                glVertex3f(-a-4,81,1);
            }
            glEnd();
    glColor3f(0.6,0.6,0.6);
    glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(a,81,18);
                glVertex3f(a-3,81,10);
                glVertex3f(a+3,81,10);
            }
            glEnd();
            glBegin(GL_QUADS);
            {
                glVertex3f(a+3,81,9);
                glVertex3f(a-3,81,9);
                glVertex3f(a-3,81,1);
                glVertex3f(a+3,81,1);
            }
            glEnd();
glColor3f(0.5,0.5,0.5);
     glBegin(GL_QUADS);             /// front er right box1 er left side
    {
        glVertex3f(28,a+60, a);
        glVertex3f(28,a+60,0);
        glVertex3f(28,-a+70,0);
        glVertex3f(28,-a+70, a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);             /// front er right box1 er right side
    {
        glVertex3f(-2,a+60, a);
        glVertex3f(-2,a+60,0);
        glVertex3f(-2,-a+70,0);
        glVertex3f(-2,-a+70, a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);             /// front er right box1 er roof
    {
        glVertex3f(28,a+60, a);
        glVertex3f(-2,a+60, a);
        glVertex3f(-2,-a+70, a);
        glVertex3f(28,-a+70, a);
    }
    glEnd();

    ///------------------------------------------------------------
     glBegin(GL_QUADS);          /// front er right box2 er front side
    {
        glVertex3f(a-25,80, a);
        glVertex3f(a-25,80,0);
        glVertex3f(-a-15,80,0);
        glVertex3f(-a-15,80, a);
    }
    glEnd();
     glBegin(GL_QUADS);          /// front er right box2 er right side
    {
        glVertex3f(a-55,80, a);
        glVertex3f(a-55,80,0);
        glVertex3f(a-55,50,0);
        glVertex3f(a-55,50, a);
    }
    glEnd();
    glColor3f(0.3,0.3,0.3);
       glBegin(GL_QUADS);          /// front er right box2 er left side
    {
        glVertex3f(a-25,80, a);
        glVertex3f(a-25,80,0);
        glVertex3f(a-25,50,0);
        glVertex3f(a-25,50, a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
       glBegin(GL_QUADS);          /// front er right box2 er roof
    {
        glVertex3f(a-25,80, a);
        glVertex3f(a-55,80, a);
        glVertex3f(a-55,50, a);
        glVertex3f(a-25,50, a);
    }
    glEnd();

}

void drawBox2(double a)
{
    glColor3f(0.6,0.6,0.6);
     glBegin(GL_QUADS);          /// back er left box1 er front side
    {
        glVertex3f(-80,-(a+8), a);
        glVertex3f(-80,-(a+8),0);
        glVertex3f(-80,-(-a+18),0);
        glVertex3f(-80,-(-a+18), a);
    }
    glEnd();
    glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(-81,-a,18);
                glVertex3f(-81,-(a-3),10);
                glVertex3f(-81,-(a+3),10);
            }
            glEnd();
            glBegin(GL_QUADS);
            {
                glVertex3f(-81,-(a+3),9);
                glVertex3f(-81,-(a-3),9);
                glVertex3f(-81,-(a-3),1);
                glVertex3f(-81,-(a+3),1);
            }
            glEnd();
    glColor3f(0.4,0.4,0.4);
     glBegin(GL_QUADS);             /// back er left box1 er right side
    {
        glVertex3f(-(a+60),-28, a);
        glVertex3f(-(a+60),-28,0);
        glVertex3f(-(-a+70),-28,0);
        glVertex3f(-(-a+70),-28, a);
    }
    glEnd();
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_QUADS);             /// back er left box1 er left side
    {
        glVertex3f(-(a+60),-(-2), a);
        glVertex3f(-(a+60),-(-2),0);
        glVertex3f(-(-a+70),-(-2),0);
        glVertex3f(-(-a+70),-(-2), a);
    }
    glEnd();
    glBegin(GL_QUADS);             /// back er left box1 er roof
    {
        glVertex3f(-(a+60),-(-2), a);
        glVertex3f(-(a+60),-28, a);
        glVertex3f(-(-a+70),-28, a);
        glVertex3f(-(-a+70),-(-2), a);
    }
    glEnd();
    ///--------------------------------------------------------------------------
     glBegin(GL_QUADS);          /// back er left box2 er front side
    {
        glVertex3f(-80,-(a-25), a);
        glVertex3f(-80,-(a-25),0);
        glVertex3f(-80,-(-a-15),0);
        glVertex3f(-80,-(-a-15), a);
    }
    glEnd();
    glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(-81,-(-a-7),18);
                glVertex3f(-81,-(-a-10),10);
                glVertex3f(-81,-(-a-4),10);
            }
            glEnd();
            glBegin(GL_QUADS);
            {
                glVertex3f(-81,-(-a-4),9);
                glVertex3f(-81,-(-a-10),9);
                glVertex3f(-81,-(-a-10),1);
                glVertex3f(-81,-(-a-4),1);
            }
            glEnd();
    glColor3f(0.3,0.3,0.3);
     glBegin(GL_QUADS);          /// back er left box2 er right side
    {
        glVertex3f(-80,-(a-25), a);
        glVertex3f(-80,-(a-25),0);
        glVertex3f(-50,-(a-25),0);
        glVertex3f(-50,-(a-25), a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
     glBegin(GL_QUADS);          /// back er left box2 er left side
    {
        glVertex3f(-80,-(a-55), a);
        glVertex3f(-80,-(a-55),0);
        glVertex3f(-50,-(a-55),0);
        glVertex3f(-50,-(a-55), a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
     glBegin(GL_QUADS);          /// back er left box2 er roof
    {
        glVertex3f(-80,-(a-55), a);
        glVertex3f(-80,-(a-25), a);
        glVertex3f(-50,-(a-25), a);
        glVertex3f(-50,-(a-55), a);
    }
    glEnd();
    ///----------------------------------------------------------------------
     glBegin(GL_QUADS);           /// back er right box1 er front side
    {
        glVertex3f(-(a+8),-80, a);
        glVertex3f(-(a+8),-80,0);
        glVertex3f(-(-a+18),-80,0);
        glVertex3f(-(-a+18),-80, a);
    }
    glEnd();
    glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(-(-a-7),-81,18);
                glVertex3f(-(-a-10),-81,10);
                glVertex3f(-(-a-4),-81,10);
            }
            glEnd();
            glBegin(GL_QUADS);
            {
                glVertex3f(-(-a-4),-81,9);
                glVertex3f(-(-a-10),-81,9);
                glVertex3f(-(-a-10),-81,1);
                glVertex3f(-(-a-4),-81,1);
            }
            glEnd();
    glColor3f(0.6,0.6,0.6);
    glColor3f(0,0,0);
            glBegin(GL_TRIANGLES);
            {
                glVertex3f(-a,-81,18);
                glVertex3f(-(a-3),-81,10);
                glVertex3f(-(a+3),-81,10);
            }
            glEnd();
            glBegin(GL_QUADS);
            {
                glVertex3f(-(a+3),-81,9);
                glVertex3f(-(a-3),-81,9);
                glVertex3f(-(a-3),-81,1);
                glVertex3f(-(a+3),-81,1);
            }
            glEnd();
glColor3f(0.4,0.4,0.4);
     glBegin(GL_QUADS);             /// back er right box1 er left side
    {
        glVertex3f(-28,-(a+60), a);
        glVertex3f(-28,-(a+60),0);
        glVertex3f(-28,-(-a+70),0);
        glVertex3f(-28,-(-a+70), a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);             /// back er right box1 er right side
    {
        glVertex3f(-(-2),-(a+60), a);
        glVertex3f(-(-2),-(a+60),0);
        glVertex3f(-(-2),-(-a+70),0);
        glVertex3f(-(-2),-(-a+70), a);
    }
    glEnd();
    glColor3f(0.6,0.6,0.6);
    glBegin(GL_QUADS);             /// back er right box1 er roof
    {
        glVertex3f(-(-2),-(a+60), a);
        glVertex3f(-28,-(a+60), a);
        glVertex3f(-28,-(-a+70), a);
        glVertex3f(-(-2),-(-a+70), a);
    }
    glEnd();
    ///------------------------------------------------------------
     glBegin(GL_QUADS);          /// back er right box2 er front side
    {
        glVertex3f(-(a-25),-80, a);
        glVertex3f(-(a-25),-80,0);
        glVertex3f(-(-a-15),-80,0);
        glVertex3f(-(-a-15),-80, a);
    }
    glEnd();
     glBegin(GL_QUADS);          /// back er right box2 er right side
    {
        glVertex3f(-(a-55),-80, a);
        glVertex3f(-(a-55),-80,0);
        glVertex3f(-(a-55),-50,0);
        glVertex3f(-(a-55),-50, a);
    }
    glEnd();
    glColor3f(0.3,0.3,0.3);
       glBegin(GL_QUADS);          /// back er right box2 er left side
    {
        glVertex3f(-(a-25),-80, a);
        glVertex3f(-(a-25),-80,0);
        glVertex3f(-(a-25),-50,0);
        glVertex3f(-(a-25),-50, a);
    }
    glEnd();
   glColor3f(0.5,0.5,0.5);
       glBegin(GL_QUADS);          /// back er right box2 er left side
    {
        glVertex3f(-(a-25),-80, a);
        glVertex3f(-(a-55),-80, a);
        glVertex3f(-(a-55),-50, a);
        glVertex3f(-(a-25),-50, a);
    }
    glEnd();
}

void drawGround()
{
    glColor3f(0,1,1);
    glBegin(GL_QUADS);        /// ground
    {
        glVertex3f(100,100,0);
        glVertex3f(-100,100,0);
        glVertex3f(-100,-100,0);
        glVertex3f(100,-100,0);

    }
    glEnd();
}
void drawCircle(double radius,int segments,int height)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,height);
            glVertex3f(points[i+1].x,points[i+1].y,height);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {
        //create shading effect
        if(i<segments/2)    shade=2*(double)i/(double)segments;
        else    shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCylinder(double radius,double height,int segments,int orientation_x,int orientation_y)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {

        //create shading effect
        if(i<segments/2)    shade=4*(double)i/(double)segments;
        else    shade=4*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x+orientation_x,points[i].y+orientation_y,0);
            glVertex3f(points[i+1].x+orientation_x,points[i+1].y+orientation_y,0);
            glVertex3f(points[i+1].x+orientation_x,points[i+1].y+orientation_y,height);
            glVertex3f(points[i].x+orientation_x,points[i].y+orientation_y,height);
        }
        glEnd();
    }
}




void drawSphere()
{
    struct point points[100][100];
    int i,j;
    double h,r;
    if(decRadi==1)  radius -= 5.0;
    if(incRadi==1)  radius += 5.0;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }

}

void drawObject()
{

    //glutWireSphere(5,10,12);
    glColor3f(1,0,0);
    drawSquare(20);
    glPushMatrix();
    {
        glColor3f(0,1,0);
        glRotatef(-angle,0,1,0);
        glTranslatef(100,0,0);
        drawSquare(20);
        glPushMatrix();
        {
            glColor3f(0.6,0.6,1);
            glRotatef(angle,0,1,0);
            glTranslatef(30,0,0);
            drawSquare(10);
        }
        glPopMatrix();
        glPushMatrix();
        {
            glColor3f(0.3,0.3,0.5);
            glRotatef(angle,0,1,0);
            glTranslatef(20,0,0);
            drawSquare(10);
        }
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glColor3f(0,0,1);
        glRotatef(angle,0,1,0);
        glTranslatef(50,0,0);
        drawSquare(20);
        glPushMatrix();
        {
            glColor3f(1,0.5,0.5);
            glRotatef(angle,0,1,0);
            glTranslatef(20,0,0);
            drawSquare(10);
        }
        glPopMatrix();
    }
    glPopMatrix();


}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void drawArm()
{
    glTranslatef(0,0,0);
    glRotatef(angle,1,0,0);
    glPushMatrix();
    {
        glTranslatef(0,0,-60);
        glRotatef(angle1,1,0,0);
        glPushMatrix();
        {

            glPushMatrix();
            {
                glTranslatef(0,0,-35);
                glRotatef(angle2,1,0,0);
                //glTranslatef(0,0,-20);
                glPushMatrix();
                {
                    glTranslatef(-8,-8,-5);
                    glScalef(.3,.3,1);
                    glutWireSphere(5,10,12);
                }
                glPopMatrix();
            }
            glPopMatrix();
            glPushMatrix();
            {
                glTranslatef(0,0,-35);
                glRotatef(angle3,1,0,0);
                glPushMatrix();
                {
                    glTranslatef(8,8,-5);
                    glScalef(.3,.3,1);
                    glutWireSphere(5,10,12);
                }
                glPopMatrix();
            }
            glPopMatrix();
            glPushMatrix();
            {
                glTranslatef(0,0,-35);
                glRotatef(angle4,1,0,0);
                glPushMatrix();
                {
                    glTranslatef(0,0,-5);
                    glScalef(.3,.3,1);
                    glutWireSphere(5,10,12);
                }
                glPopMatrix();
            }
            glPopMatrix();

            glPushMatrix();
            {
                glBegin(GL_TRIANGLES);
                {
                    glVertex3f(0,0,-24);
                    glVertex3f(10,10,-36);
                    glVertex3f(-10,-10,-36);
                }
                glEnd();
            }
            glPopMatrix();

            glPushMatrix();
            {

                glTranslatef(0,0,-12);

                glPushMatrix();
                {
                    glScalef(.5,.5,3);
                    glutWireSphere(4,10,15);
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();

    }
    glPopMatrix();




    glTranslatef(0,0,-30);

    glPushMatrix();
    {
        glScalef(.5,.5,3);
        glutWireSphere(10,15,20);
    }
    glPopMatrix();
}


void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1':
        if(angle<60)
        angle += 2;
        break;
    case '2':
        if(angle>-60)
        angle -= 2;
        break;
    case '3':
        if(angle1<40)
        angle1 += 2;
        break;
    case '4':
        if(angle1>-40)
        angle1 -= 2;
        break;
    case '5':
        if(angle2<90)
        angle2 += 2;
        break;
    case '6':
        if(angle2>0)
        angle2 -= 2;

        break;
    case '7':
        if(angle3<90)
        angle3 += 2;

        break;
    case '8':
        if(angle3>0)
        angle3 -= 2;
        break;
    case '9':
        if(angle4<90)
        angle4 += 2;

        break;
    case '0':
        if(angle4>0)
        angle4 -= 2;
        break;
    case 'q':
        if(angle5<60)
            angle5+=2;
        break;
    case 'w':
        if(angle5>-60)
            angle5 -= 2;
        break;
    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        pos.x -= 2*l.x;
        pos.y -= 2*l.y;
        pos.z -= 2*l.z;
        break;
    case GLUT_KEY_UP:		// up arrow key
        pos.x += 2*l.x;
        pos.y += 2*l.y;
        pos.z += 2*l.z;
        break;

    case GLUT_KEY_RIGHT:
        pos.x -= 2*r.x;
        pos.y -= 2*r.y;
        pos.z -= 2*r.z;
        break;
    case GLUT_KEY_LEFT:
        pos.x += 2*r.x;
        pos.y += 2*r.y;
        pos.z += 2*r.z;
        break;

    case GLUT_KEY_PAGE_UP:
        pos.x += 2*u.x;
        pos.y += 2*u.y;
        pos.z += 2*u.z;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.x -= 2*u.x;
        pos.y -= 2*u.y;
        pos.z -= 2*u.z;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}



void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    //gluLookAt(0,0,200,	0,0,0,	0,1,0);
    gluLookAt(pos.x, pos.y, pos.z,  pos.x + l.x, pos.y + l.y, pos.z + l.z,   u.x, u.y, u.z);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawGrid();

    //glColor3f(1,0,0);
   //drawSquare(20);
    drawCube(20);
    drawRoofCube(12);
    drawBalancedCube(15);
    drawCylinderCube();
    drawRightBox();
    //drawSphere();
    //drawSS();

    drawCylinder(7,20,50,62,50);
    drawCylinder(7,20,50,50,62);
    drawCylinder(7,20,50,72,60);
    drawCylinder(7,20,50,60,72);
    drawCylinder(16,20,60,40,40);
    drawCylinder(22,20,60,60,-60);
    glColor3f(0,0,0);
    drawCylinder(35,40,20,0,0);
    drawBox1(20);
    drawBox2(20);

    //drawGround();
    //drawCone(20,50,24);

    radius = 30;
    stacks = 20;
    slices = 24;
    //drawSp();
    //drawWheel();
    //glRotatef(angle5,0,1,0);
    //drawArm();
//    moveWheel();
    //drawObject();


    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid=1;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;angle1=0;angle2=0;angle3=0;angle4=0;

    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");
    //vector3 p = vector3(0,0,1) + vector3(1,1,0);
    //cout<<p.x<<p.y<<p.z;

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
