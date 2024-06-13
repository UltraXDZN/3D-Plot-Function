#include "TE/tinyexpr.h"
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float angleX = 0.0f, angleZ = 0.0f;

const char *expr = "3 - (x*x + y*y)";
te_expr *compiled_expr;
double variables[2]; // x and y

void initOpenGL();
void reshape(int width, int height);
void rotate(int key, int x, int y);
void display();
void renderBitmapString(float x, float y, float z, void *font, const char *string);

double evaluateExpression(double x, double y);


int main(int argc, char **argv) {
    if (argc > 1) {
        expr = argv[1];
    }

    // Čitanje funkcije f(x, y) iz parametra pokretanja
    int err;
    te_variable vars[] = {{"x", &variables[0]}, {"y", &variables[1]}};
    compiled_expr = te_compile(expr, vars, 2, &err);
    if (!compiled_expr) {
        printf("Failed to compile expression: %s\n", expr);
        printf("Error at position: %d\n", err);
        return 1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Surface Plot");

    initOpenGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(rotate); 

    glutMainLoop();

    te_free(compiled_expr);
    return 0;
}




void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel(GL_SMOOTH); 
}


void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}


void rotate(int key, int x, int z) {
    if (key == GLUT_KEY_RIGHT) angleZ += 5;
    else if (key == GLUT_KEY_LEFT) angleZ -= 5;
    else if (key == GLUT_KEY_UP) angleX += 5;
    else if (key == GLUT_KEY_DOWN) angleX -= 5;
    glutPostRedisplay();
}


void renderBitmapString(float x, float y, float z, void *font, const char *string) {
    const char *c;
    glRasterPos3f(x, y, z);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}


double evaluateExpression(double x, double y) {
    variables[0] = x;
    variables[1] = y;
    return te_eval(compiled_expr);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Pozicija kamere
    gluLookAt(30.0, 30.0, 30.0,  
              0.0, 0.0, 0.0,     
              0.0, 0.0, 1.0);   

    // Rotacija
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleZ, 0.0, 0.0, 1.0);

    // Crtanje koordinatnog sustava
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);

    glVertex3f(-20.0, 0.0, 0.0);
    glVertex3f(20.0, 0.0, 0.0);

    glVertex3f(0.0, -20.0, 0.0);
    glVertex3f(0.0, 20.0, 0.0);

    glVertex3f(0.0, 0.0, -20.0);
    glVertex3f(0.0, 0.0, 20.0);
    glEnd();


    for (int i = -20; i <= 20; i += 5) {
        glBegin(GL_LINES);

        glVertex3f(i, -0.5, 0.0);
        glVertex3f(i, 0.5, 0.0);

        glVertex3f(-0.5, i, 0.0);
        glVertex3f(0.5, i, 0.0);

        glVertex3f(0.0, -0.5, i);
        glVertex3f(0.0, 0.5, i);

        glEnd();
        if (i != 0) {
            char label[4];
            sprintf(label, "%d", i);
            renderBitmapString(i, -1.0, 0.0, GLUT_BITMAP_HELVETICA_12, label);
            renderBitmapString(0.0, i, 0.0, GLUT_BITMAP_HELVETICA_12, label);
            renderBitmapString(0.0, -1.0, i, GLUT_BITMAP_HELVETICA_12, label);
        }
    }

    renderBitmapString(21.0, 0.0, 0.0, GLUT_BITMAP_HELVETICA_18, "X");
    renderBitmapString(0.0, 21.0, 0.0, GLUT_BITMAP_HELVETICA_18, "Y");
    renderBitmapString(0.0, 0.0, 21.0, GLUT_BITMAP_HELVETICA_18, "Z");


    // Crtanje f(x, y) funkcije
    glColor3f(0.0, 0.0, 0.0); 
    int numGrid = 10;
    float range = 2.0;
    float step = 2 * range / numGrid;
    for (float x = -range; x <= range - step; x += step) {
        for (float y = -range; y <= range - step; y += step) {
            glBegin(GL_POLYGON);        //  SOLID SHAPE
            // glBegin(GL_LINE_STRIP); -    WIREFRAME
            glVertex3f(x, y, evaluateExpression(x, y));
            glVertex3f(x + step, y, evaluateExpression(x + step, y));
            glVertex3f(x + step, y + step, evaluateExpression(x + step, y + step));
            glVertex3f(x, y + step, evaluateExpression(x, y + step));
            glEnd();
        }
    }

    glutSwapBuffers();
}
