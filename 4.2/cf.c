#include <GLUT/glut.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265359

// Padrão de 32x32 bits com as letras CF em binário
GLubyte cf_pattern[128] = {
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b11111110, 0b11111110, 0b00000000,
  0b00000011, 0b00000000, 0b00000000, 0b00000000,
  0b00000100, 0b00000000, 0b00000000, 0b00000000,
  0b00001000, 0b00000000, 0b00000000, 0b00000000,
  0b00001000, 0b00000000, 0b00000000, 0b00000000,
  0b00001000, 0b00000000, 0b00000000, 0b00000000,
  0b00000100, 0b00000000, 0b00000000, 0b00000000,
  0b00000011, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b11111110, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00011111, 0b11111111, 0b00000000, 0b00000000,
  0b00000100, 0b00000000, 0b00000000, 0b00000000,
  0b00000100, 0b00000000, 0b00000000, 0b00000000,
  0b00000111, 0b11100000, 0b00000000, 0b00000000,
  0b00000100, 0b00000000, 0b00000000, 0b00000000,
  0b00000100, 0b00000000, 0b00000000, 0b00000000,
  0b00000100, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000,
  0b00000000, 0b00000000, 0b00000000, 0b00000000
};

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 300, 0, 300);
}

void drawHexagon(int centerX, int centerY, int radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i) {
        float angle = PI / 3.0 * i;
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(cf_pattern);
    glColor3f(0.0, 0.0, 1.0); // Azul
    drawHexagon(150, 150, 100);
    glDisable(GL_POLYGON_STIPPLE);

    glFlush();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(300, 300);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hexágono com padrão CF");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
