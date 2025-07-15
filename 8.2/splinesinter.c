#include <stdlib.h>
#include <math.h>
#include <GLUT/glut.h>

// --- Variáveis Globais ---
GLint eixox = 0, eixoy = 0, eixoz = 0;
GLint nVertices = 4;
GLfloat vertices[4][4][3];
GLint largura, altura;

enum { BEZIER, NURBS };
GLint spline;

GLUnurbsObj *nc;
GLfloat nos[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLint nNos = 8;

// Novas variáveis para interatividade
int selected_i = -1, selected_j = -1; // Índices do ponto de controle selecionado

// --- Funções ---

void gera_superficie(void) {
    int i, j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            vertices[i][j][0] = 2.0 * ((GLfloat)i - 1.5);
            vertices[i][j][1] = 2.0 * ((GLfloat)j - 1.5);
            if ((i == 1 || i == 2) && (j == 1 || j == 2))
                vertices[i][j][2] = 7.0;
            else
                vertices[i][j][2] = -3.0;
        }
    }
}

void display(void) {
    int i, j;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(eixox, 1, 0, 0);
    glRotatef(eixoy, 0, 1, 0);
    glRotatef(eixoz, 0, 0, 1);
    glScalef(0.25, 0.25, 0.25);

    // Desenha eixos coordenados (opcional)
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(-5, -5, -5);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glEnd();
    glRasterPos3f(1.5, 0, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'x');
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0); glVertex3f(0, 1, 0);
    glEnd();
    glRasterPos3f(0, 1.5, 0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'y');
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
    glEnd();
    glRasterPos3f(0, 0, 1.5);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'z');
    glPopMatrix();
    glEnable(GL_LIGHTING);


    // Desenha a superfície
    switch (spline) {
    case BEZIER:
        glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0, 1.0, 3 * nVertices, 4, &vertices[0][0][0]);
        glEnable(GL_AUTO_NORMAL);
        glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
        glEvalMesh2(GL_FILL, 0, 20, 0, 20);
        break;
    case NURBS:
        gluBeginSurface(nc);
        gluNurbsSurface(nc, nNos, nos, nNos, nos, 4 * 3, 3, &vertices[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
        gluEndSurface(nc);
        break;
    }

    // Desenha os pontos de controle
    glPointSize(7.0);
    glDisable(GL_LIGHTING);
    glBegin(GL_POINTS);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == selected_i && j == selected_j) {
                glColor3f(1.0, 1.0, 0.0); // Amarelo para o ponto selecionado
            } else {
                glColor3f(1.0, 0.0, 0.0); // Vermelho para os outros
            }
            glVertex3fv(&vertices[i][j][0]);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

void init(void) {
    GLfloat mat_diffuse[] = {0.7, 0.7, 0.7, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {100.0};

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    gera_superficie();

    nc = gluNewNurbsRenderer();
    gluNurbsProperty(nc, GLU_SAMPLING_TOLERANCE, 5.0);
    gluNurbsProperty(nc, GLU_DISPLAY_MODE, GLU_FILL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    spline = NURBS;
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MAP2_VERTEX_3);
    display();
}

void reshape(int w, int h) {
    largura = w;
    altura = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 3.0, 8.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'x': eixox = (eixox + 5) % 360; glutPostRedisplay(); break;
    case 'X': eixox = (eixox - 5) % 360; glutPostRedisplay(); break;
    case 'y': eixoy = (eixoy + 5) % 360; glutPostRedisplay(); break;
    case 'Y': eixoy = (eixoy - 5) % 360; glutPostRedisplay(); break;
    case 'z': eixoz = (eixoz + 5) % 360; glutPostRedisplay(); break;
    case 'Z': eixoz = (eixoz - 5) % 360; glutPostRedisplay(); break;
    case 'b': spline = BEZIER; glutPostRedisplay(); break;
    case 'n': spline = NURBS; glutPostRedisplay(); break;
    case 27: exit(0); break;
    }
}

// --- Funções de Seleção e Movimentação ---

// Função para calcular o produto escalar de dois vetores
GLfloat dot_product(GLfloat v1[3], GLfloat v2[3]) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

// Função para subtrair dois vetores (res = v1 - v2)
void subtract_vectors(GLfloat v1[3], GLfloat v2[3], GLfloat res[3]) {
    res[0] = v1[0] - v2[0];
    res[1] = v1[1] - v2[1];
    res[2] = v1[2] - v2[2];
}


// Função para selecionar um ponto de controle
void select_point(int x, int y) {
    GLdouble model[16], proj[16];
    GLint view[4];
    GLdouble near_pos[3], far_pos[3];

    // Obter matrizes e viewport atuais
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);

    // Obter ponto no plano próximo (z=0)
    gluUnProject(x, view[3] - y, 0.0, model, proj, view, &near_pos[0], &near_pos[1], &near_pos[2]);
    // Obter ponto no plano distante (z=1)
    gluUnProject(x, view[3] - y, 1.0, model, proj, view, &far_pos[0], &far_pos[1], &far_pos[2]);

    // Calcular o raio
    GLfloat ray_origin[3] = {(GLfloat)near_pos[0], (GLfloat)near_pos[1], (GLfloat)near_pos[2]};
    GLfloat ray_dir[3];
    subtract_vectors((GLfloat*)far_pos, (GLfloat*)near_pos, ray_dir);

    float min_dist_sq = 1e6; // Um número grande para a distância mínima ao quadrado
    selected_i = -1;
    selected_j = -1;

    // Iterar por todos os pontos de controle para encontrar o mais próximo do raio
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            GLfloat R[3] = {vertices[i][j][0], vertices[i][j][1], vertices[i][j][2]};
            
            // Usando a fórmula: P_o + ((R - P_o) . V) / (V . V) * V
            GLfloat R_minus_Po[3];
            subtract_vectors(R, ray_origin, R_minus_Po);

            float t = dot_product(R_minus_Po, ray_dir) / dot_product(ray_dir, ray_dir);

            // Encontrar o ponto mais próximo na linha
            GLfloat closest_point[3];
            closest_point[0] = ray_origin[0] + t * ray_dir[0];
            closest_point[1] = ray_origin[1] + t * ray_dir[1];
            closest_point[2] = ray_origin[2] + t * ray_dir[2];

            // Calcular a distância (ao quadrado) entre o ponto de controle e o ponto no raio
            GLfloat dist_vec[3];
            subtract_vectors(R, closest_point, dist_vec);
            float dist_sq = dot_product(dist_vec, dist_vec);
            
            if (dist_sq < min_dist_sq) {
                min_dist_sq = dist_sq;
                selected_i = i;
                selected_j = j;
            }
        }
    }
    
    // Define um limiar para a seleção. Se o ponto mais próximo ainda estiver
    // muito longe, considera que nenhum ponto foi clicado.
    // Este valor pode precisar de ajuste.
    if (min_dist_sq > 0.1) {
        selected_i = -1;
        selected_j = -1;
    }
}


// Função de callback do mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            select_point(x, y);
        } else { // state == GLUT_UP
            selected_i = -1; // Desseleciona o ponto
            selected_j = -1;
        }
    }
    glutPostRedisplay();
}

// Função de callback de movimento do mouse (arrastar)
void motion(int x, int y) {
    if (selected_i == -1) return; // Nenhum ponto selecionado

    GLdouble model[16], proj[16];
    GLint view[4];
    GLdouble new_pos[3];
    GLdouble winZ;

    // Obter matrizes e viewport
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);

    // Projeta o ponto selecionado na tela para obter sua profundidade (winZ)
    gluProject(vertices[selected_i][selected_j][0], vertices[selected_i][selected_j][1], vertices[selected_i][selected_j][2],
               model, proj, view, &new_pos[0], &new_pos[1], &winZ);

    // "Desprojeta" a nova posição do mouse (x, y) usando a profundidade original do ponto
    // Isso move o ponto em um plano paralelo à tela
    gluUnProject(x, view[3] - y, winZ, model, proj, view, &new_pos[0], &new_pos[1], &new_pos[2]);
    
    // Atualiza as coordenadas do ponto de controle
    vertices[selected_i][selected_j][0] = (GLfloat)new_pos[0];
    vertices[selected_i][selected_j][1] = (GLfloat)new_pos[1];
    vertices[selected_i][selected_j][2] = (GLfloat)new_pos[2];

    glutPostRedisplay();
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Spline Interativa");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    
    // Registrar as novas callbacks de mouse
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    
    glutMainLoop();
    return 0;
}