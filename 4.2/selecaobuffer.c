#include <GLUT/glut.h> 
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int x1, y1, x2, y2;
    GLfloat fillColor[3];
    GLfloat borderColor[3];
} Polygon;

Polygon polys[4];
bool changeFill = true;

void randomColor(GLfloat *color) {
    color[0] = (GLfloat)rand() / (RAND_MAX + 1.0);
    color[1] = (GLfloat)rand() / (RAND_MAX + 1.0);
    color[2] = (GLfloat)rand() / (RAND_MAX + 1.0);
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 256, 0, 256);

    int coords[4][4] = {
        {30, 143, 113, 226},
        {143, 143, 226, 226},
        {30, 30, 113, 113},
        {143, 30, 226, 113}
    };

    for (int i = 0; i < 4; i++) {
        polys[i].x1 = coords[i][0];
        polys[i].y1 = coords[i][1];
        polys[i].x2 = coords[i][2];
        polys[i].y2 = coords[i][3];
        polys[i].fillColor[0] = 1.0;
        polys[i].fillColor[1] = 1.0;
        polys[i].fillColor[2] = 0.0;
        polys[i].borderColor[0] = 0.0;
        polys[i].borderColor[1] = 0.0;
        polys[i].borderColor[2] = 0.0;
    }
}

void drawPolygon(Polygon p) {
    glColor3fv(p.fillColor);
    glBegin(GL_POLYGON);
    glVertex2i(p.x1, p.y1);
    glVertex2i(p.x2, p.y1);
    glVertex2i(p.x2, p.y2);
    glVertex2i(p.x1, p.y2);
    glEnd();

    glColor3fv(p.borderColor);
    glBegin(GL_LINE_LOOP);
    glVertex2i(p.x1, p.y1);
    glVertex2i(p.x2, p.y1);
    glVertex2i(p.x2, p.y2);
    glVertex2i(p.x1, p.y2);
    glEnd();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < 4; i++) {
        drawPolygon(polys[i]);
    }
    glFlush(); // Usado em buffer simples
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'f':
        case 'F':
            changeFill = true;
            break;
        case 'b':
        case 'B':
            changeFill = false;
            break;
        case 27:
            exit(0);
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int yOpenGL = 256 - y;
        for (int i = 0; i < 4; i++) {
            if (x >= polys[i].x1 && x <= polys[i].x2 &&
                yOpenGL >= polys[i].y1 && yOpenGL <= polys[i].y2) {
                if (changeFill) {
                    randomColor(polys[i].fillColor);
                } else {
                    randomColor(polys[i].borderColor);
                }
                glutPostRedisplay();
                break;
            }
        }
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // buffer simples
    glutInitWindowSize(256, 256);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Seleção de polígonos - Buffer Simples");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
