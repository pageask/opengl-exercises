#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <unistd.h>

#define ESCAPE 27

int window;

void initGL(int width, int height) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  glShadeModel(GL_SMOOTH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
}

void resizeGLScene(int width, int height) {
  if (height == 0)
    height = 1;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
}

void drawGLScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  glTranslatef(-1.5f, 0.0f, -6.0f);

  glBegin(GL_POLYGON);
  glVertex3f(0.0f, 1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glEnd();

  glTranslatef(3.0f, 0.0f, 0.0f);
  glBegin(GL_QUADS);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glEnd();

  glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y) {
  usleep(100);

  if (key == ESCAPE) {
    glutDestroyWindow(window);
    exit(0);
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(0, 0);

  window = glutCreateWindow("Pageask's OpenGl lesson02");

  glutDisplayFunc(&drawGLScene);
  //glutFullScreen();
  glutIdleFunc(&drawGLScene);
  glutReshapeFunc(&resizeGLScene);
  glutKeyboardFunc(&keyPressed);

  initGL(640, 480);

  glutMainLoop();

  return 1;
}
