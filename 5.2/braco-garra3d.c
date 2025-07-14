#include <GLUT/glut.h>
#include <stdlib.h>

static int shoulder = 0, elbow = 0;
static int indicator = 0, middle = 0, thumb = 0;
static int base = 0; // New variable for base rotation

void init(void){
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
  glEnable(GL_DEPTH_TEST); // Enable depth testing
  glEnable(GL_CULL_FACE);  // Enable face culling
}

void display(void){
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear depth buffer as well
  glPushMatrix();

  // Rotate the entire arm around the Y-axis for the base rotation
  glRotatef((GLfloat) base, 0.0, 1.0, 0.0);

  /* origin positioned at the shoulder */
  glTranslatef (-1.0, 0.0, 0.0);
  glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);

  /* origin positioned at the center of the upper arm */
  glTranslatef (1.0, 0.0, 0.0);
  glPushMatrix();
  glColor3f(1.0, 0.0, 0.0); // Red color for the shoulder
  glScalef (2.0, 0.4, 1.0);
  glutSolidCube (1.0); // Use glutSolidCube
  glPopMatrix();
   
  /* origin positioned at the elbow */
  glTranslatef (1.0, 0.0, 0.0);
  glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
  glTranslatef (1.0, 0.0, 0.0); // This translation puts the origin at the end of the elbow
  glPushMatrix();
  glColor3f(0.0, 1.0, 0.0); // Green color for the elbow
  glScalef (2.0, 0.4, 1.0);
  glutSolidCube (1.0); // Use glutSolidCube
  glPopMatrix();

  // --- Gripper (Garra) Implementation ---
  // Translate to the end of the last segment (elbow)
  // Now add the fingers from this point

  // Indicator finger (Blue)
  glPushMatrix();
  glColor3f(0.0, 0.0, 1.0); // Blue color for indicator
  glTranslatef(1.0, 0.2, 0.0); // Position the indicator finger relative to the elbow end
  glRotatef((GLfloat) indicator, 0.0, 0.0, 1.0); // Rotate the indicator finger
  glTranslatef(0.2, 0.0, 0.0); // Adjust origin to center of finger for scaling
  glScalef(0.4, 0.2, 0.2); // Scale to look like a finger
  glutSolidCube(1.0); // Use glutSolidCube
  glPopMatrix();

  // Middle finger (Cyan)
  glPushMatrix();
  glColor3f(0.0, 1.0, 1.0); // Cyan color for middle
  glTranslatef(1.0, -0.2, 0.0); // Position the middle finger
  glRotatef((GLfloat) middle, 0.0, 0.0, 1.0); // Rotate the middle finger
  glTranslatef(0.2, 0.0, 0.0); // Adjust origin to center of finger for scaling
  glScalef(0.4, 0.2, 0.2); // Scale to look like a finger
  glutSolidCube(1.0); // Use glutSolidCube
  glPopMatrix();

  // Thumb finger (Yellow)
  glPushMatrix();
  glColor3f(1.0, 1.0, 0.0); // Yellow color for thumb
  glTranslatef(0.8, -0.2, -0.5); // Position the thumb finger
  glRotatef((GLfloat) thumb, 1.0, 0.0, 0.0); // Rotate the thumb finger (around X-axis for thumb-like movement)
  glTranslatef(0.0, -0.2, 0.2); // Adjust origin to center of finger for scaling
  glScalef(0.2, 0.4, 0.2); // Scale to look like a thumb
  glutSolidCube(1.0); // Use glutSolidCube
  glPopMatrix();
  // --- End of Gripper (Garra) Implementation ---

  /* origin returns to the original coordinate system */
  glPopMatrix();
  glutSwapBuffers();
}

void reshape (int w, int h){
  glViewport (0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef (0.0, 0.0, -5.0);
}

void keyboard (unsigned char key, int x, int y){
  switch (key) {
  case 's':
    shoulder = (shoulder + 5) % 360;
    glutPostRedisplay();
    break;
  case 'S':
    shoulder = (shoulder - 5) % 360;
    glutPostRedisplay();
    break;
  case 'e':
    elbow = (elbow + 5) % 360;
    glutPostRedisplay();
    break;
  case 'E':
    elbow = (elbow - 5) % 360;
    glutPostRedisplay();
    break;
  case 'i': // Rotate indicator clockwise
    indicator = (indicator + 5) % 360;
    glutPostRedisplay();
    break;
  case 'I': // Rotate indicator anti-clockwise
    indicator = (indicator - 5) % 360;
    glutPostRedisplay();
    break;
  case 'm': // Rotate middle clockwise
    middle = (middle + 5) % 360;
    glutPostRedisplay();
    break;
  case 'M': // Rotate middle anti-clockwise
    middle = (middle - 5) % 360;
    glutPostRedisplay();
    break;
  case 'p': // Rotate thumb clockwise
    thumb = (thumb + 5) % 360;
    glutPostRedisplay();
    break;
  case 'P': // Rotate thumb anti-clockwise
    thumb = (thumb - 5) % 360;
    glutPostRedisplay();
    break;
  case 'b': // Rotate base clockwise
    base = (base + 5) % 360;
    glutPostRedisplay();
    break;
  case 'B': // Rotate base anti-clockwise
    base = (base - 5) % 360;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Changed display mode
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}