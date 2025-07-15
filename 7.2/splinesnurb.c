#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_POINTS 20

GLfloat vertices[MAX_POINTS][3];
int nVertices = 0;
int movingPoint = -1;

GLUnurbsObj *nurbs;

GLint grau = 3; // Grau da curva
GLint tipoNos = 0; // 0: uniforme, 1: uniforme aberto, 2: não uniforme

GLfloat nos_uniforme[MAX_POINTS + 10];
GLfloat nos_uniforme_aberto[MAX_POINTS + 10];
GLfloat nos_nao_uniforme[MAX_POINTS + 10];

void initNos() {
    int nCtrlPts = nVertices;
    int nNos = nCtrlPts + grau + 1;

    // Uniforme
    for (int i = 0; i < nNos; i++)
        nos_uniforme[i] = (GLfloat)i;

    // Uniforme aberto
    for (int i = 0; i <= grau; i++)
        nos_uniforme_aberto[i] = 0.0f;
    for (int i = grau + 1; i < nCtrlPts; i++)
        nos_uniforme_aberto[i] = i - grau;
    for (int i = nCtrlPts; i < nNos; i++)
        nos_uniforme_aberto[i] = nCtrlPts - grau;

    // Não uniforme (exemplo customizado)
    for (int i = 0; i < nNos; i++) {
        if (i < grau + 1)
            nos_nao_uniforme[i] = 0.0f;
        else if (i >= nCtrlPts)
            nos_nao_uniforme[i] = 1.0f;
        else
            nos_nao_uniforme[i] = (GLfloat)(rand() % 100) / 100.0f;
    }
}

void desenhaCurva() {
    if (nVertices < grau + 1) return;

    int nNos = nVertices + grau + 1;
    GLfloat *nó_usado = NULL;

    switch (tipoNos) {
        case 0: nó_usado = nos_uniforme; break;
        case 1: nó_usado = nos_uniforme_aberto; break;
        case 2: nó_usado = nos_nao_uniforme; break;
    }

    gluBeginCurve(nurbs);
    gluNurbsCurve(nurbs, nNos, nó_usado, 3, &vertices[0][0], grau + 1, GL_MAP1_VERTEX_3);
    gluEndCurve(nurbs);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Desenha pontos de controle
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < nVertices; i++)
        glVertex3fv(vertices[i]);
    glEnd();

    // Desenha linhas entre pontos
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < nVertices; i++)
        glVertex3fv(vertices[i]);
    glEnd();

    // Desenha curva
    glColor3f(0.0, 0.0, 1.0);
    desenhaCurva();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'u':
            tipoNos = 0;
            printf("Usando vetor de nós uniforme.\n");
            glutPostRedisplay();
            break;
        case 'o':
            tipoNos = 1;
            printf("Usando vetor de nós uniforme aberto.\n");
            glutPostRedisplay();
            break;
        case 'n':
            tipoNos = 2;
            printf("Usando vetor de nós não uniforme.\n");
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
    }
}

void mouse(int button, int state, int x, int y) {
    float fx = (float)x;
    float fy = 500.0f - (float)y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        for (int i = 0; i < nVertices; i++) {
            if (abs(fx - vertices[i][0]) < 5.0 && abs(fy - vertices[i][1]) < 5.0) {
                movingPoint = i;
                return;
            }
        }
        if (nVertices < MAX_POINTS) {
            vertices[nVertices][0] = fx;
            vertices[nVertices][1] = fy;
            vertices[nVertices][2] = 0.0;
            nVertices++;
            initNos();
            glutPostRedisplay();
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        movingPoint = -1;
    }
}

void motion(int x, int y) {
    if (movingPoint >= 0) {
        vertices[movingPoint][0] = (float)x;
        vertices[movingPoint][1] = 500.0f - (float)y;
        glutPostRedisplay();
    }
}

void init(void) {
    nurbs = gluNewNurbsRenderer();
    gluNurbsProperty(nurbs, GLU_SAMPLING_TOLERANCE, 25.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("splinesnurbs.c");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}
