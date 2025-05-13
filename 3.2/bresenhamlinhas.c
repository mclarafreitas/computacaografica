#include <GLUT/glut.h> // Para macOS. No Linux, use <GL/glut.h>
#include <stdlib.h>

// Protótipos
void init(void);
void display(void);
void bresenhamLine(int x0, int y0, int x1, int y1);

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(256, 256);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Linha com Bresenham");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

// Inicialização do OpenGL
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fundo branco
    glOrtho(0, 256, 0, 256, -1, 1);   // Coordenadas da janela
}

// Função de desenho
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0); // Cor verde

    // Desenha usando Bresenham
    bresenhamLine(40, 200, 200, 10);

    glFlush();
}

// Algoritmo de Bresenham para todas direções (adaptado)
void bresenhamLine(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    glBegin(GL_POINTS);
    while (1) {
        glVertex2i(x0, y0);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
    glEnd();
}
