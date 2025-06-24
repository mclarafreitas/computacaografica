#include <GLUT/glut.h>  // Para macOS
#include <math.h>
#include <stdio.h>

#define PI 3.14159265

// HSI de cada vértice (apenas H muda com o tempo)
float H[4] = {0.0, 0.25, 0.5, 0.75}; // valores iniciais de H (entre 0 e 1)
float S = 1.0;  // saturação fixa
float I = 1.0;  // intensidade fixa

// Função para converter HSI para RGB
void hsiToRgb(float h, float s, float i, float *r, float *g, float *b) {
    float r_, g_, b_;

    h = fmod(h, 1.0);         // mantém H no intervalo [0, 1]
    h *= 360;                 // converte para graus

    if (h < 120) {
        float rad = h * PI / 180.0;
        b_ = i * (1 - s);
        r_ = i * (1 + (s * cos(rad)) / cos(PI / 3 - rad));
        g_ = 3 * i - (r_ + b_);
    } else if (h < 240) {
        h -= 120;
        float rad = h * PI / 180.0;
        r_ = i * (1 - s);
        g_ = i * (1 + (s * cos(rad)) / cos(PI / 3 - rad));
        b_ = 3 * i - (r_ + g_);
    } else {
        h -= 240;
        float rad = h * PI / 180.0;
        g_ = i * (1 - s);
        b_ = i * (1 + (s * cos(rad)) / cos(PI / 3 - rad));
        r_ = 3 * i - (g_ + b_);
    }

    *r = fmin(fmax(r_, 0), 1);
    *g = fmin(fmax(g_, 0), 1);
    *b = fmin(fmax(b_, 0), 1);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Calcular RGB para cada vértice
    float rgb[4][3];
    for (int i = 0; i < 4; i++) {
        hsiToRgb(H[i], S, I, &rgb[i][0], &rgb[i][1], &rgb[i][2]);
    }

    glBegin(GL_POLYGON);
    glColor3f(rgb[0][0], rgb[0][1], rgb[0][2]); glVertex2f(0.25, 0.25);
    glColor3f(rgb[1][0], rgb[1][1], rgb[1][2]); glVertex2f(0.75, 0.25);
    glColor3f(rgb[2][0], rgb[2][1], rgb[2][2]); glVertex2f(0.75, 0.75);
    glColor3f(rgb[3][0], rgb[3][1], rgb[3][2]); glVertex2f(0.25, 0.75);
    glEnd();

    glFlush();
}

// Timer para animar a rotação do matiz (H) dos vértices
void timer(int value) {
    for (int i = 0; i < 4; i++) {
        H[i] += 0.01;
        if (H[i] > 1.0) H[i] -= 1.0;
    }
    glutPostRedisplay();
    glutTimerFunc(33, timer, 1);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(256, 256);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Quadrado Colorido com HSI");

    glClearColor(1.0, 1.0, 1.0, 0.0);  // fundo branco

    //glShadeModel(GL_FLAT);  // Comentado para ativar interpolação de cores
    glShadeModel(GL_SMOOTH); // interpolação suave (degradê)

    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

    glutDisplayFunc(display);
    glutTimerFunc(33, timer, 1);
    glutMainLoop();
    return 0;
}
