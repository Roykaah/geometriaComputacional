/**
 * @file primitives.cpp
 * Draws different primitives.
 *
 * Draws different primitives of OpenGL according to a pressed key:
 * 1 for points;
 * 2 for lines;
 * 3 for line strip;
 * 4 for line loop;
 * 5 for triangles;
 * 6 for triangle strip;
 * 7 for triangle fan.
 *
 * @author Ricardo Dutra da Silva
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../lib/utils.h"
#include <math.h>
#include <bits/stdc++.h>

/* Globals */
/** Window width. */
int win_width = 800;
/** Window height. */
int win_height = 600;

/** Program variable. */
int program;
/** Vertex array object. */
unsigned int VAO;
/** Vertex buffer object. */
unsigned int VBO;

/** Primitive type. */
int type_primitive = GL_TRIANGLES;
int quantidade_cliques = 0;

float vertices[] = {
    0.0f, 1.0f,0.0f,0.0f,
    1.0f, 0.0f,0.0f,0.0f,
    0.0f, 0.0f,0.0f,0.0f,
    -1.0f, 0.0f,0.0f,0.0f
    };
class Vector3D
{
public:
    double x,y,z;

    Vector3D(double _x=0, double _y=0, double _z=0)
        : x(_x), y(_y), z(_z)
    {
    }

    double dot(Vector3D v2)
    {
        return x*v2.x + y*v2.y + z*v2.z;
    }

    Vector3D operator+(const Vector3D& v2)
    {
       return Vector3D(x+v2.x, y+v2.y, z+v2.z);
    }

    Vector3D operator-(const Vector3D& v2)
    {
       return Vector3D(x-v2.x, y-v2.y, z-v2.z);
    }

    Vector3D operator*(const double& sc)
    {
       return Vector3D(sc*x, sc*y, sc*z);
    }

    double norm()
    {
        return sqrt(dot(*this));
    }

};
/** Vertex shader. */
const char *vertex_code = "\n"
                          "#version 330 core\n"
                          "layout (location = 0) in vec3 position;\n"
                          "\n"
                          "void main()\n"
                          "{\n"
                          "    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                          "}\0";

/** Fragment shader. */
const char *fragment_code = "\n"
                            "#version 330 core\n"
                            "out vec4 FragColor;\n"
                            "\n"
                            "void main()\n"
                            "{\n"
                            "    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
                            "}\0";

/* Functions. */
void display(void);
void reshape(int, int);
void keyboard(unsigned char, int, int);
void glutMouseFunc(void (*func)(int button, int state,
                                int x, int y));
void mouse(int button, int state, int x, int y);
void initData(void);
void initShaders(void);

/**
 * Drawing function.
 *
 * Draws primitive.
 */
void drawCircle(float cx, float cy, float r, int num_segments)
{
    float theta = 3.1415926 * 2 / float(num_segments);
    float tangetial_factor = tanf(theta);//calculate the tangential factor 

    float radial_factor = cosf(theta);//calculate the radial factor 

    float x = r;//we start at angle = 0 

    float y = 0;
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x + cx, y + cy);//output vertex 

        //calculate the tangential vector 
        //remember, the radial vector is (x, y) 
        //to get the tangential vector we flip those coordinates and negate one of them 

        float tx = -y;
        float ty = x;

        //add the tangential vector 

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        //correct using the radial factor 

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}
int inCircle(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4){
    /*float mr = (y2 - y1) / (x2 - x1);
    float mt = (y3 - y2) / (x3 - x2);

    float x = (mr * mt * (y3 - y1) + mr * (x2 + x3) - mt * (x1 + x2)) / (2.0 * (mr - mt));
    float y = (y1 + y2) / 2.0 - (x - (x1 + x2) / 2.0) / mr;

    float radius = sqrt(pow(x2 - x, 2.0) + pow(y2 - y, 2.0));
    float dist = sqrt(pow(x4 - x, 2.0) + pow(y4 - y, 2.0));
    if (dist < radius){
        return true;
    }
    else{
        return false;
    }*/
    float z1 = (x1*x1+y1*y1);
    float z2 = (x2*x2+y2*y2);
    float z3 = (x3*x3+y3*y3);
    float z4 = (x4*x4+y4*y4);
    float xn = (y2-y1)*(z3-z1)- (y3-y1)*(z2-z1);
    float yn = (x3-x1)*(z2-z1)- (x2-x1)*(z3-z1);
    float zn = (x2-x1)*(y3-y1)- (x3-x1)*(y2-y1);
    float area = (x4 -x1)*xn + (y4-y1)*yn + (z4-z1)*zn;
    printf("area %f\n",area);
    if (area > 0){
        return true;
    }
    else{
        return false;
    }
}
void display()
{
    float ax = vertices[0];
    float ay = vertices[1];
    float bx = vertices[4];
    float by = vertices[5];
    float cx = vertices[8];
    float cy = vertices[9];
    float dx = vertices[12];
    float dy = vertices[13];


    glClear(GL_COLOR_BUFFER_BIT);


 glColor3f(1.0, 1.0, 1.0); 

    //TRIANGULO 1
    glColor3f(1.0, 0.0, 1.0); 
    glBegin(GL_LINES);
    glVertex2f(ax,ay);
    glVertex2f(bx,by);
    glVertex2f(bx,by);
    glVertex2f(cx,cy);
    glVertex2f(cx,cy);
    glVertex2f(ax,ay);
    printf("%f %f %f %f %f %f",ax,ay,bx,by,cx,cy);
    glEnd();

    //TRIANGULO 2
    glColor3f(0.0, 1.0, 1.0); 
    glBegin(GL_LINES);
    glVertex2f(cx,cy);
    glVertex2f(bx,by);
    glVertex2f(bx,by);
    glVertex2f(dx,dy);
    glVertex2f(dx,dy);
    glVertex2f(cx,cy);


    float mr = (by - ay) / (bx - ax);
    float mt = (cy - by) / (cx - bx);

    float x = (mr * mt * (cy - ay) + mr * (bx + cx) - mt * (ax + bx)) / (2.0 * (mr - mt));
    float y = (ay + by) / 2.0 - (x - (ax + bx) / 2.0) / mr;

    float radius = sqrt(pow(bx - x, 2.0) + pow(by - y, 2.0));
    
    float p1[3] = {ax,ay,0};
    float p2[3] = {bx,by,0};
    float p3[3] = {cx,cy,0};

    if (left(p1,p2,p3)){
        inCircle(ax,ay,bx,by,cx,cy,dx,dy) ? glColor3f(1.0, 0.0, 0.0) : glColor3f(0.0, 1.0, 0.0);
    } else{
        inCircle(cx,cy,bx,by,ax,ay,dx,dy) ? glColor3f(1.0, 0.0, 0.0) : glColor3f(0.0, 1.0, 0.0);
    }
    drawCircle(x,y,radius,100);

    glEnd();
    glFlush();

}
/**
 * Reshape function.
 *
 * Called when window is resized.
 *
 * @param width New window width.
 * @param height New window height.
 */
void reshape(int width, int height)
{
    win_width = width;
    win_height = height;
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

/**
 * Keyboard function.
 *
 * Called to treat pressed keys.
 *
 * @param button Pressed button.
 * @param state State of the button.
 * @param x Mouse x coordinate when key pressed.
 * @param y Mouse y coordinate when key pressed.
 */
void mouse(int button, int state, int x, int y)
{
    float area;
    char* orientacao;
    if (state == GLUT_DOWN){

    
    switch (button)
    {    

    case GLUT_LEFT_BUTTON:
        printf("posição do ponto nº %d: x = %d, y = %d\n", quantidade_cliques + 1, x, y);
        vertices[(quantidade_cliques * 4)%12] = (float(x)*2/win_width -1);
        vertices[(quantidade_cliques * 4 + 1)%12] = -(float(y)*2/win_height -1);
        printf("%f e %f",vertices[(quantidade_cliques * 4)%12],-vertices[(quantidade_cliques * 4 +1)%12]);
        quantidade_cliques += 1;
        //int type_primitive = GL_TRIANGLES;
        glutPostRedisplay();
        //area = doisAreaT(vertices,&vertices[4],&vertices[8])/2;
        //printf("A área é de %f e a orientação é: ",abs(area));
        //(area > 0) ? printf(" anti-horário\n") : printf(" horário\n");
        break;
    case GLUT_RIGHT_BUTTON:
        printf("posição do ponto nº %d: x = %d, y = %d\n", quantidade_cliques + 1, x, y);
        vertices[12] = (float(x)*2/win_width -1);
        vertices[13] = -(float(y)*2/win_height -1);
        printf("%f e %f",vertices[12],-vertices[13]);
        //int type_primitive = GL_TRIANGLES;
        glutPostRedisplay();
        //area = doisAreaT(vertices,&vertices[4],&vertices[8])/2;
        //printf("A área é de %f e a orientação é: ",abs(area));
        //(area > 0) ? printf(" anti-horário\n") : printf(" horário\n");
        break;

        break;
    case GLUT_MIDDLE_BUTTON:
        printf("middle button \n");

        break;
    }
    }
}




/**
 * Init vertex data.
 *
 * Defines the coordinates for vertices, creates the arrays for OpenGL.
 */

/** Create program (shaders).
 *
 * Compile shaders and create the program.
 */
void initShaders()
{
    // Request a program and shader slots from GPU
    program = createShaderProgram(vertex_code, fragment_code);
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("DCEL Display");

    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    glutMainLoop();
    return 0;
}
