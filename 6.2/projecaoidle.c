#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // Necessário para a função usleep()

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void reshape (int w, int h);
void idle(void); // Protótipo da nova função idle

#define AZUL     0.0, 0.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0
#define AMARELO  1.0, 1.0, 0.0
#define VERDE    0.0, 1.0, 0.0
#define CYAN     0.0, 1.0, 1.0
#define LARANJA  0.8, 0.6, 0.1
#define ROSEO    0.7, 0.1, 0.6
#define CINZA    0.6, 0.6, 0.6

static GLfloat vertices[30]={
  0.0,  30.0, 30.0, /* 0 */
  20.0, 30.0, 30.0, /* 1 */
  30.0, 20.0, 30.0, /* 2 */
  30.0,  0.0, 30.0, /* 3 */
  0.0,   0.0, 30.0, /* 4 */
  0.0,  30.0,  0.0, /* 5 */
  30.0, 30.0,  0.0, /* 6 */
  30.0,  0.0,  0.0, /* 7 */
  0.0,   0.0,  0.0, /* 8 */
  30.0, 30.0, 20.0  /* 9 */
};

static GLubyte frenteIndices[]    = {0,4,3,2,1};
static GLubyte trasIndices[]      = {5,6,7,8};
static GLubyte esquerdaIndices[]  = {0,5,8,4};
static GLubyte direitaIndices[]   = {2,3,7,6,9};
static GLubyte topoIndices[]      = {0,1,9,6,5};
static GLubyte fundoIndices[]     = {3,4,8,7};
static GLubyte trianguloIndices[] = {1,2,9};

// Ângulos de rotação, inicializados em 0
static GLfloat eixoy = 0.0, eixox = 0.0;
int largura, altura;

// --- Nova Função Idle ---
// Esta função é chamada pelo GLUT quando não há outros eventos a serem processados.
void idle(void){
    // Incrementa os ângulos de rotação com valores constantes e diferentes
    eixox += 0.4f;
    eixoy += 0.8f;

    // Garante que os ângulos permaneçam dentro do intervalo 0-360
    if(eixox > 360) eixox -= 360;
    if(eixoy > 360) eixoy -= 360;

    // Solicita que a janela seja redesenhada para mostrar o próximo quadro da animação
    glutPostRedisplay();

    // Introduz um pequeno atraso (em microssegundos) para controlar a velocidade.
    // 16000 microssegundos ≈ 60 quadros por segundo (1.000.000 / 60 ≈ 16666)
    usleep(16000);
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("Animacao com glutIdleFunc");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle); // <-- REGISTRA A FUNÇÃO IDLE AQUI
  glutMainLoop();
  return 0;
}

void init(void){
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glOrtho (-50, 50, -50, 50, -50 , 50);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void reshape (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  largura=w;
  altura=h;
}

void display(void){
  glPushMatrix();
  glRotatef (eixoy, 0.0, 1.0, 0.0);
  glRotatef (eixox, 1.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices);

  glColor3f (AZUL); /* frente */
  glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, frenteIndices);
  glColor3f (AMARELO); /* esquerda */
  glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, esquerdaIndices);
  glColor3f (VERMELHO); /* tras */
  glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, trasIndices);
  glColor3f (VERDE); /* direita */
  glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, direitaIndices);
  glColor3f (CYAN); /* topo */
  glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, topoIndices);
  glColor3f (LARANJA); /* fundo */
  glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, fundoIndices);
  glColor3f (CINZA); /* triangulo */
  glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_BYTE, trianguloIndices);

  glDisableClientState (GL_VERTEX_ARRAY);

  glPopMatrix();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
    case 27: // Tecla ESC para sair
      exit(0);
      break;
    // Teclas para alternar a projeção continuam funcionando
    case 'p':
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(65.0, (GLfloat) largura/(GLfloat) altura, 20.0, 120.0);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      // Ajusta a câmera para a projeção de perspectiva
      gluLookAt(0, 0, 90, 0, 0, 0, 0, 1, 0);
      break;
    case 'o':
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho (-50, 50, -50, 50, -50 , 50);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      break;
  }
  // Não é estritamente necessário chamar glutPostRedisplay aqui,
  // pois a função idle() já faz isso continuamente.
}