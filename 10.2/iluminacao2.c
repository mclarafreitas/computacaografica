#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>

#define PI 3.1415

GLint WIDTH = 320;
GLint HEIGHT = 240;

GLint fatias = 30;
GLint pilhas = 30;
GLint raioEsfera = 1.5;

GLfloat obs[3] = {0.0, 7.0, 0.0};
GLfloat olho[3] = {0.0, 3.0, 0.0};

GLfloat plano_difusa[] = {0.5, 0.5, 0.0, 1.0};
GLfloat plano_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat plano_brilho[] = {50.0};

GLfloat mat_a_difusa[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_a_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_a_brilho[] = {50.0};

GLfloat mat_b_difusa[] = {0.7, 0.7, 0.7, 0.5};
GLfloat mat_b_especular[] = {1.0, 1.0, 1.0, 0.5};
GLfloat mat_b_brilho[] = {50.0};

GLfloat posicao_luz0[] = {0.0, 10.0, 0.0, 1.0};
GLfloat cor_luz0[] = {1.0, 1.0, 1.0, 1.0};
GLfloat cor_luz0_amb[] = {0.3, 0.3, 0.3, 1.0};

GLfloat posicao_luz1[] = {0.0, 10.0, 5.0, 1.0};
GLfloat cor_luz1[] = {0.0, 0.0, 1.0, 1.0};
GLfloat direcao_luz1[] = {0.0, -10.0, -5.0, 1.0};
GLint spot_luz1 = 30;

GLfloat sem_cor[] = {0.0, 0.0, 0.0, 1.0};

GLint gouraud = 0;

GLfloat tetaxz = 0;
GLfloat raioxz = 6;

// Mouse state
int lastX = -1, lastY = -1;
int shiftPressed = 0;

void clamp(GLfloat *value) {
  if (*value < 0.0f) *value = 0.0f;
  if (*value > 1.0f) *value = 1.0f;
}

void motion(int x, int y) {
  if (lastX < 0 || lastY < 0) {
    lastX = x;
    lastY = y;
    return;
  }

  int dx = x - lastX;
  int dy = y - lastY;

  if (shiftPressed) {
    // Modificar iluminação ambiente
    for (int i = 0; i < 3; i++) {
      cor_luz0_amb[i] += (dx - dy) * 0.005f;
      clamp(&cor_luz0_amb[i]);
    }
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz0_amb);
  } else {
    // Modificar transparência (alpha) do objeto B
    mat_b_difusa[3] += (dx - dy) * 0.005f;
    mat_b_especular[3] = mat_b_difusa[3];
    clamp(&mat_b_difusa[3]);
    clamp(&mat_b_especular[3]);
  }

  lastX = x;
  lastY = y;
  glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
  if (state == GLUT_DOWN) {
    lastX = x;
    lastY = y;
    shiftPressed = (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
  } else if (state == GLUT_UP) {
    lastX = -1;
    lastY = -1;
  }
}

void reshape(int width, int height) {
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, width / (float)height, 0.1, 30.0);
  glMatrixMode(GL_MODELVIEW);
}

void display(void) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glDepthMask(GL_TRUE);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glShadeModel(gouraud ? GL_SMOOTH : GL_FLAT);

  glPushMatrix();

  obs[0] = raioxz * cos(2 * PI * tetaxz / 360);
  obs[2] = raioxz * sin(2 * PI * tetaxz / 360);
  gluLookAt(obs[0], obs[1], obs[2], olho[0], olho[1], olho[2], 0.0, 1.0, 0.0);

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plano_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plano_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, plano_brilho);

  glNormal3f(0, 1, 0);
  glBegin(GL_QUADS);
  glVertex3f(-10, 0, 10);
  glVertex3f(10, 0, 10);
  glVertex3f(10, 0, -10);
  glVertex3f(-10, 0, -10);
  glEnd();

  glPushMatrix();
  glTranslatef(posicao_luz0[0], posicao_luz0[1], posicao_luz0[2]);
  glColor3f(1, 0, 0);
  glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz0);
  glutSolidSphere(0.3, 5, 5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(posicao_luz1[0], posicao_luz1[1], posicao_luz1[2]);
  glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz1);
  glutSolidSphere(0.3, 5, 5);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_EMISSION, sem_cor);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_a_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_a_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_a_brilho);

  glPushMatrix();
  glTranslatef(0.0, 3.0, -3.0);
  glutSolidSphere(raioEsfera, fatias, pilhas);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_b_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_b_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_b_brilho);

  glTranslatef(0.0, 3.0, 3.0);
  glutSolidSphere(raioEsfera, fatias, pilhas);

  glPopMatrix();
  glutSwapBuffers();
}

void special(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP: obs[1] += 1; break;
    case GLUT_KEY_DOWN: obs[1] -= 1; break;
    case GLUT_KEY_LEFT: tetaxz += 2; break;
    case GLUT_KEY_RIGHT: tetaxz -= 2; break;
  }
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27: exit(0); break;
    case 'g': gouraud = !gouraud; break;
    case 'r': raioxz += 1; break;
    case 'R': if (raioxz > 1) raioxz -= 1; break;
  }
  glutPostRedisplay();
}

void init() {
  gouraud = 1;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz0_amb);
  glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz1);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_luz1);
  glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcao_luz1);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
}

void menu(int value) {
  switch (value) {
    case 0: mat_a_especular[0] = mat_a_especular[1] = mat_a_especular[2] = 0.0; break;
    case 1: mat_a_especular[0] = mat_a_especular[1] = mat_a_especular[2] = 0.5; break;
    case 2: mat_a_especular[0] = mat_a_especular[1] = mat_a_especular[2] = 1.0; break;
    case 3: mat_a_difusa[0] = mat_a_difusa[1] = mat_a_difusa[2] = 0.0; break;
    case 4: mat_a_difusa[0] = mat_a_difusa[1] = mat_a_difusa[2] = 0.5; break;
    case 5: mat_a_difusa[0] = mat_a_difusa[1] = mat_a_difusa[2] = 1.0; break;
  }
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  if (!glutCreateWindow("Iluminacao com Transparência e Mouse")) {
    fprintf(stderr, "Erro ao criar janela.\n");
    exit(-1);
  }

  init();

  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);

  glutCreateMenu(menu);
  glutAddMenuEntry("-sem spec", 0);
  glutAddMenuEntry("-spec media", 1);
  glutAddMenuEntry("-spec alta", 2);
  glutAddMenuEntry("-sem difusa", 3);
  glutAddMenuEntry("-difusa media", 4);
  glutAddMenuEntry("-difusa alta", 5);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return 0;
}
