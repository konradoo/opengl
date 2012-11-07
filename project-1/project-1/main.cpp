#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

void display()
{    // Clear the window with current clearing color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
//  glBegin(GL_QUADS);
//  
//  glColor3f(0.3, 0.3, 0.3);
//  glVertex3f(-0.5f, 0.5f, 0.0f);
//  glVertex3f( 0.5f, 0.5f, 0.0f);
//  glVertex3f( 0.5f,-0.5f, 0.0f);
//  glVertex3f(-0.5f,-0.5f, 0.0f);
//  
//  glEnd();
  
  glBegin(GL_TRIANGLES);

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(-0.5f, -0.5f, 0.0f);
  glVertex3f(0.5f,-0.5f, 0.0f);
  glVertex3f(0.0f,0.0f, 1.0f);
  
  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(-0.5f, 0.5f, 0.0f);
  glVertex3f(0.5f,0.5f, 0.0f);
  glVertex3f(0.0f,0.0f, 1.0f);
  
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.5f, -0.5f, 0.0f);
  glVertex3f(0.5f,0.5f, 0.0f);
  glVertex3f(0.0f,0.0f, 1.0f);
  
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(-0.5f, -0.5f, 0.0f);
  glVertex3f(-0.5f,0.5f, 0.0f);
  glVertex3f(0.0f,0.0f, 1.0f);
  
  glEnd();
  
  glutSwapBuffers();
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Pyramid");
  glEnable(GL_DEPTH_TEST);
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}