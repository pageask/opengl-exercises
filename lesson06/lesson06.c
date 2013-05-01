#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ESCAPE 27

struct _Image_ {
  unsigned long width;
  unsigned long height;
  char* data;
};
typedef struct _Image_ Image;

int window;
float xrot, yrot, zrot;
int textures[1];

int loadImage(const char* filename, Image* image) {
  FILE* file;
  unsigned long i;
  unsigned long size;
  unsigned short int planes;
  unsigned short int bpp;
  char temp;

  file = fopen(filename, "rb");
  if (file == NULL) {
    printf("File not found : %s.\n", filename);
    return 0;
  }

  fseek(file, 18, SEEK_CUR);

  i = fread(&image->width, 4, 1, file);
  if (i != 1) {
    printf("Error reading width from %s.\n", filename);
    return 0;
  }
  printf("Width of %s : %lu.\n", filename, image->width);

  i = fread(&image->height, 4, 1, file);
  if (i != 1) {
    printf("Error reading height from %s.\n", filename);
    return 0;
  }
  printf("Height of %s : %lu.\n", filename, image->height);

  size = image->width * image->height * 3;

  i = fread(&planes, 2, 1, file);
  if (i != 1) {
    printf("Error reading planes from %s.\n", filename);
    return 0;
  }
  if (planes != 1) {
    printf("Planes from %s is not 1 : %u.\n", filename, planes);
    return 0;
  }

  i = fread(&bpp, 2, 1, file);
  if (i != 1) {
    printf("Error reading bpp from %s.\n", filename);
    return 0;
  }
  if (bpp != 24) {
    printf("Bpp from %s is not 24 : %u.\n", filename, bpp);
    return 0;
  }

  fseek(file, 24, SEEK_CUR);

  image->data = (char*)malloc(size);
  if (image->data == NULL) {
    printf("Error allocating memorry for color-corrected image data");
    return 0;
  }

  i = fread(image->data, size, 1, file);
  if (i != 1) {
    printf("Error reading image data from %s.\n", filename);
    return 0;
  }

  for (i = 0; i < size; i += 3) {
    temp = image->data[i];
    image->data[i] = image->data[i + 2];
    image->data[i + 2] = temp;
  }

  return 1;
}

void loadGLTextures(const char* filename) {
  Image* image;

  image = (Image*)malloc(sizeof(Image));
  if (image == NULL) {
    printf("Error allocating space for image");
    exit(0);
  }

  if (!loadImage(filename, image)) {
    exit(1);
  }

  glGenTextures(1, &textures[0]);
  glBindTexture(GL_TEXTURE_2D, textures[0]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, 3, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
}

void initGL(/*int width, int height, */const char* filename) {
  loadGLTextures(filename);
  glEnable(GL_TEXTURE_2D);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glClearDepth(1.0);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  glShadeModel(GL_SMOOTH);

  /*
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
  */
}

void reshapeGLScene(int width, int height) {
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
  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(xrot, 1.0f, 0.0f, 0.0f);
  glRotatef(yrot, 0.0f, 1.0f, 0.0f);
  glRotatef(zrot, 0.0f, 0.0f, 1.0f);

  glBindTexture(GL_TEXTURE_2D, textures[0]);

  glBegin(GL_QUADS);

  // Front face 
  glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
  glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, -1.0f, 1.0f);
  glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);

  // Back face
  glTexCoord2f(1.0f, 0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 1.0f);glVertex3f(1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 0.0f);glVertex3f(1.0f, -1.0f, -1.0f);
  /*
  glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, 1.0f, -1.0f);
  glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, -1.0f, -1.0f);
  */

  // Top face
  glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, 1.0f, -1.0f);

  // Bottom face
  glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, -1.0f, -1.0f);
  glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, -1.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, -1.0f, 1.0f);

  // Right face
  glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0f, -1.0f, -1.0f);
  glTexCoord2f(1.0f, 1.0f);glVertex3f(1.0f, 1.0f, -1.0f);
  glTexCoord2f(0.0f, 1.0f);glVertex3f(1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);glVertex3f(1.0f, -1.0f, 1.0f);

  // Left face
  glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f, -1.0f);
  glTexCoord2f(1.0f, 0.0f);glVertex3f(-1.0f, -1.0f, 1.0f);
  glTexCoord2f(1.0f, 1.0f);glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f);glVertex3f(-1.0f, 1.0f, -1.0f);

  glEnd();

  xrot += 1.0f;
  yrot += 1.0f;
  zrot += 1.0f;

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
  char* filename;
  if (argc != 2) {
    filename = "nehe.bmp";
  } else {
    filename = argv[1];
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);
  glutInitWindowPosition(0, 0);

  window = glutCreateWindow("Pageask's OpenGL lesson06");

  glutDisplayFunc(&drawGLScene);
  //glutFullScreen();
  glutIdleFunc(&drawGLScene);
  glutReshapeFunc(&reshapeGLScene);
  glutKeyboardFunc(&keyPressed);

  initGL(/*640, 480, */filename);

  glutMainLoop();

  return 1;
}
