#include <GLUT/glut.h>  // Para macOS
#include <stdlib.h>
#include <stdio.h>

void display(void);

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(256, 256); 
  glutInitWindowPosition(100, 100); 
  glutCreateWindow("Quadrados 4");
  glClearColor(1.0, 1.0, 1.0, 0.0);  // Fundo branco
  glShadeModel(GL_FLAT);
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);  // Coordenadas normalizadas
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);

  // Inferior esquerdo - vermelho
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex2f(0.0, 0.0);
  glVertex2f(0.5, 0.0);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.0, 0.5);
  glEnd();

  // Inferior direito - verde
  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex2f(0.5, 0.0);
  glVertex2f(1.0, 0.0);
  glVertex2f(1.0, 0.5);
  glVertex2f(0.5, 0.5);
  glEnd();

  // Superior esquerdo - azul
  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_POLYGON);
  glVertex2f(0.0, 0.5);
  glVertex2f(0.5, 0.5);
  glVertex2f(0.5, 1.0);
  glVertex2f(0.0, 1.0);
  glEnd();

  // Superior direito - amarelo
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  glVertex2f(0.5, 0.5);
  glVertex2f(1.0, 0.5);
  glVertex2f(1.0, 1.0);
  glVertex2f(0.5, 1.0);
  glEnd();

  glFlush();
}
