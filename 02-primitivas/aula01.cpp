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

double vertices[] = {
    0.0f, 1.0f,0.0f,0.0f,
    1.0f, 0.0f,0.0f,0.0f,
    0.0f, 0.0f,0.0f,0.0f };

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
void display()
{
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(type_primitive, 0, 4);

    glutSwapBuffers();
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
    double area;
    char* orientacao;
    if (state == GLUT_DOWN){

    
    switch (button)
    {    

    case GLUT_LEFT_BUTTON:
        printf("posição do ponto nº %d: x = %d, y = %d\n", quantidade_cliques + 1, x, y);
        vertices[(quantidade_cliques * 4)%12] = (double(x)*2/win_width -1);
        vertices[(quantidade_cliques * 4 + 1)%12] = -(double(y)*2/win_height -1);
        printf("%lf e %lf",vertices[(quantidade_cliques * 4)%12],-vertices[(quantidade_cliques * 4 +1)%12]);
        quantidade_cliques += 1;
        glBufferSubData(GL_ARRAY_BUFFER,0 ,sizeof(vertices), vertices);
        glutPostRedisplay();
        area = doisAreaT(vertices,&vertices[4],&vertices[8])/2;
        printf("A área é de %lf e a orientação é: ",abs(area));
        (area > 0) ? printf(" anti-horário\n") : printf(" horário\n");
        break;
    case GLUT_RIGHT_BUTTON:
        printf("right button \n");

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

void initData()
{

    // Vertex array.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set attributes.
    glVertexAttribPointer(0, 4, GL_double, GL_FALSE, 4 * sizeof(double), (void *)0);
    glEnableVertexAttribArray(0);

    // Unbind Vertex Array Object.
    glBindVertexArray(0);
}

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
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow(argv[0]);
    glewInit();

    // Init vertex data for the triangle.
    initData();

    // Create shaders.
    initShaders();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
}
