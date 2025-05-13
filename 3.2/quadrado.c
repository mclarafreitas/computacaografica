#include <GLUT/glut.h> 
#include <stdlib.h>

float red = 1.0, green = 0.0, blue = 0.0; // Cor inicial: vermelho

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(256, 256); 
  glutInitWindowPosition(100, 100); 
  glutCreateWindow("Quadrado com Mudança de Cor");
  init();
  glutDisplayFunc(display); 
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}

void init(void){
  glClearColor(1.0, 1.0, 1.0, 1.0); // fundo branco
  glOrtho(0, 256, 0, 256, -1, 1);   // define sistema de coordenadas
}

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  
  glColor3f(red, green, blue); // usa a cor atual

  glBegin(GL_POLYGON);
    glVertex2i(30, 226);  // canto superior esquerdo
    glVertex2i(226, 226); // canto superior direito
    glVertex2i(226, 30);  // canto inferior direito
    glVertex2i(30, 30);   // canto inferior esquerdo
  glEnd();

  glFlush();
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
    case 'a': // muda para azul
      red = 0.0; green = 0.0; blue = 1.0;
      glutPostRedisplay();
      break;

    case 'v': // volta para vermelho
      red = 1.0; green = 0.0; blue = 0.0;
      glutPostRedisplay();
      break;

    case 27: // tecla ESC
      exit(0);
      break;
  }
}
