#include <GLUT/glut.h> 
#include <stdlib.h>

// Protótipos
void init(void);
void display(void);
void bresenhamCircle(int xc, int yc, int r);
void plotCirclePoints(int xc, int yc, int x, int y);

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(256, 256);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circunferência com Bresenham");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

// Inicialização
void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fundo branco
    glOrtho(0, 256, 0, 256, -1, 1);   // Coordenadas da janela
}

// Função de desenho
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0); // Cor azul

    bresenhamCircle(128, 128, 50); // Circunferência centrada em (128,128) com raio 50

    glFlush();
}

// Algoritmo de Bresenham para circunferência (Midpoint Circle Algorithm)
void bresenhamCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 1 - r;

    glBegin(GL_POINTS);
    plotCirclePoints(xc, yc, x, y);
    while (x < y) {
        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }
        plotCirclePoints(xc, yc, x, y);
    }
    glEnd();
}

// Plota os 8 pontos simétricos da circunferência
void plotCirclePoints(int xc, int yc, int x, int y) {
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
}
