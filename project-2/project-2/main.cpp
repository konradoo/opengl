#include <GLTools.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <StopWatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

CStopWatch timer;

GLint shader;
GLint MVPMatrixLocation;

GLFrame frame;
GLFrustum frustum;

M3DMatrix44f cameraMatrix;
M3DMatrix44f projectionMatrix;
M3DMatrix44f translationMatrix;
M3DMatrix44f MVPMatrix;

void setUpFrame(GLFrame &frame,const M3DVector3f origin, const M3DVector3f forward, const M3DVector3f up)
{
  frame.SetOrigin(origin);
  frame.SetForwardVector(forward);
  M3DVector3f side, oUp;
  m3dCrossProduct3(side, forward, up);
  m3dCrossProduct3(oUp, side, forward);
  frame.SetUpVector(oUp);
  frame.Normalize();
};

void lookAt(GLFrame &frame, const M3DVector3f eye, const M3DVector3f at, const M3DVector3f up)
{
  M3DVector3f forward;
  m3dSubtractVectors3(forward, at, eye);
  setUpFrame(frame, eye, forward, up);
}

void updateCamera()
{
  M3DVector3f at = {0, 0, 0};
  M3DVector3f up = {0, 0, 1};
  M3DVector3f eye;
  float angle = timer.GetElapsedSeconds() * 0.1;
  
  eye[0] = 6.8f * cos(angle);
  eye[1] = 6.0f * sin(angle);
  eye[2] = 5.0f;
  lookAt(frame, eye, at, up);
  frame.GetCameraMatrix(cameraMatrix);
  m3dMatrixMultiply44(projectionMatrix, frustum.GetProjectionMatrix(), cameraMatrix);
  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, projectionMatrix);
}

void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  frustum.SetPerspective(60.0, w/h, 1.0, 1000.0);
}

void initialize()
{
  glClearColor(1.0, 1.0, 1.0, 1.0);

  shader = gltLoadShaderPairWithAttributes("shader.vp", "shader.fp", 2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_COLOR, "vColor");
  
  MVPMatrixLocation = glGetUniformLocation(shader, "MVPMatrix");
  if (MVPMatrixLocation == -1)
  {
	fprintf(stderr, "uniform MVPMatrix could not be found\n");
  }
}

void drawGrid()
{
  glBegin(GL_LINES);
  
  glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.8, 0.8, 0.8);
  
  for (int y = -10; y < 10; y ++)
  {
    glVertex3f( 10.0, y, 0.0);
    glVertex3f(-10.0, y, 0.0);
  }
  
  for (int x = -10; x < 10; x ++)
  {
    glVertex3f(x,  10.0, 0.0);
    glVertex3f(x, -10.0, 0.0);
  }
  
  glEnd();
}

void drawPyramid()
{
  glBegin(GL_QUADS);
  glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.5, 0.5, 0.5);
  glVertex3f(-1.0,  1.0, 0.0);
  glVertex3f( 1.0,  1.0, 0.0);
  glVertex3f( 1.0, -1.0, 0.0);
  glVertex3f(-1.0, -1.0, 0.0);
  glEnd();

  glBegin(GL_TRIANGLES);
  glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0, 0.0, 1.0);
  glVertex3f(-1.0,  1.0, 0.0);
  glVertex3f( 0.0,  0.0, 1.0);
  glVertex3f(-1.0, -1.0, 0.0);
  glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 1.0, 1.0, 0.0);
  glVertex3f(-1.0,  1.0, 0.0);
  glVertex3f( 1.0,  1.0, 0.0);
  glVertex3f( 0.0,  0.0, 1.0);
  glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 1.0, 0.0, 0.0);
  glVertex3f( 1.0,  1.0, 0.0);
  glVertex3f( 1.0, -1.0, 0.0);
  glVertex3f( 0.0,  0.0, 1.0);
  glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0, 1.0, 0.0);
  glVertex3f( 0.0,  0.0, 1.0);
  glVertex3f( 1.0, -1.0, 0.0);
  glVertex3f(-1.0, -1.0, 0.0);
  glEnd();
}

void render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  
  glUseProgram(shader);
  
  updateCamera();
  
  // Drawing multiple pyramids
//  drawGrid();
//
//  m3dTranslationMatrix44(cameraMatrix, 0.0, 0.0, -1.0);
//  m3dMatrixMultiply44(MVPMatrix, projectionMatrix, cameraMatrix);
//  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, MVPMatrix);
//  drawPyramid();
//
//  m3dRotationMatrix44(cameraMatrix, 30, 0.0, 1.0, 1.0);
//  m3dMatrixMultiply44(MVPMatrix, projectionMatrix, cameraMatrix);
//  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, MVPMatrix);
//  drawPyramid();
//
//  m3dScaleMatrix44(cameraMatrix, 1.0, 1.0, 0.5);
//  m3dMatrixMultiply44(MVPMatrix, projectionMatrix, cameraMatrix);
//  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, MVPMatrix);
//  drawPyramid();
  
  
  // Drawing multiple pyramids using GLMatrixStack
//  GLMatrixStack matrixStack;
//  matrixStack.PushMatrix();
//  matrixStack.LoadMatrix(frustum.GetProjectionMatrix());
//  frame.GetCameraMatrix(cameraMatrix);
//  matrixStack.MultMatrix(cameraMatrix);
//  matrixStack.PushMatrix();
//  matrixStack.PushMatrix();
//  
//  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, matrixStack.GetMatrix());
//  drawGrid();
//  
//  matrixStack.Translate(0.0, 1.0, 0.0);
//  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, matrixStack.GetMatrix());
//  drawPyramid();
//  
//  matrixStack.PopMatrix();
//  matrixStack.Rotate(45.0, 0, 0, 1);
//  matrixStack.Translate(0.0, 0.0, 1.0);
//  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, matrixStack.GetMatrix());
//  drawPyramid();
//  
//  matrixStack.PopMatrix();
//  matrixStack.Translate(2.0, -1.0, -1.0);
//  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, matrixStack.GetMatrix());
//  drawPyramid();

  // Drawing multiple pyramids using GLMatrixStack and GLGeometryTransform
  GLMatrixStack modelViewStack;
  GLMatrixStack projectionStack;
  GLGeometryTransform geometryPipeline;
  geometryPipeline.SetMatrixStacks(modelViewStack, projectionStack);
  
  projectionStack.LoadMatrix(frustum.GetProjectionMatrix());
  modelViewStack.PushMatrix();
  frame.GetCameraMatrix(cameraMatrix);
  modelViewStack.LoadMatrix(cameraMatrix);
  modelViewStack.PushMatrix();
  
  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, geometryPipeline.GetModelViewProjectionMatrix());
  drawGrid();
  
  modelViewStack.Translate(2.0, 1.0, 0.0);
  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, geometryPipeline.GetModelViewProjectionMatrix());
  drawPyramid();

  modelViewStack.PopMatrix();
  modelViewStack.Rotate(45.0, 0,0, 1.0);
  modelViewStack.Translate(0.0, 0.0, 1.0);
  glUniformMatrix4fv(MVPMatrixLocation, 1, GL_FALSE, geometryPipeline.GetModelViewProjectionMatrix());
  drawPyramid();
  
  
  glutSwapBuffers();
  glutPostRedisplay();
}

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Project #2: Pyramid");
  glutReshapeFunc(resize);
  glutDisplayFunc(render);
  glEnable(GL_DEPTH_TEST);
//  glEnable(GL_CULL_FACE);

  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
    return 1;
  }

  initialize();

  glutMainLoop();
  return 0;
}