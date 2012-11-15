#include <GLTools.h>
#ifdef __APPLE__
#include <glut/glut.h>          // OS X version of GLUT
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#include <glFrame.h>
#include <glFrustum.h> 
#include <StopWatch.h>
#include <GLUT/GLUT.h>

GLFrame cameraframe;
GLFrustum viewFrustum;

M3DMatrix44f VPMatrix;
M3DMatrix44f mCamera;

CStopWatch timer;

GLuint shader;
static GLint MVPMatrixLocation;    

void SetUpFrame(GLFrame &frame, const M3DVector3f origin, const M3DVector3f forward, const M3DVector3f up) 
{					
	frame.SetOrigin(origin);
	frame.SetForwardVector(forward);
	M3DVector3f side , oUp;
	m3dCrossProduct3(side,forward,up);
	m3dCrossProduct3(oUp,side,forward);
	frame.SetUpVector(oUp);
	frame.Normalize();
}

void LookAt(GLFrame &frame, const M3DVector3f cam, const M3DVector3f at, const M3DVector3f up)
{   
	M3DVector3f forward;   
	m3dSubtractVectors3(forward, at, cam); 
	SetUpFrame(frame, cam, forward, up);
}

void ChangeSize(int w, int h) {
    glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(20, w/h, 1 , 50);
}

void SetupRC() {
    glClearColor(0.8f, 0.8f, 0.8f, 0.8f);

	shader = gltLoadShaderPairWithAttributes("shader.vp", "shader.fp",
            2, GLT_ATTRIBUTE_VERTEX, "vVertex", GLT_ATTRIBUTE_COLOR, "vColor"); 


    fprintf(stdout, "GLT_ATTRIBUTE_VERTEX : %d\nGLT_ATTRIBUTE_COLOR : %d \n",
            GLT_ATTRIBUTE_VERTEX, GLT_ATTRIBUTE_COLOR);

	MVPMatrixLocation=glGetUniformLocation(shader,"MVPMatrix");
		if(MVPMatrixLocation==-1)
		{
			fprintf(stderr,"uniform MVPMatrix could not be found\n");
		}

	//glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,matrix);

	glFrontFace(GL_CW);
	glEnable( GL_DEPTH_TEST );

}

///////////////////////////////////////////////////////////////////////////////
// Called to draw scene

void RenderScene(void) {
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glUseProgram(shader);
	const float* mProjection = viewFrustum.GetProjectionMatrix();
		
	cameraframe.GetCameraMatrix(mCamera);
	m3dMatrixMultiply44(VPMatrix,mProjection,mCamera);
    glUseProgram(shader);
	glUniformMatrix4fv(MVPMatrixLocation,1,GL_FALSE,VPMatrix);

	  glBegin(GL_QUADS); 
		    glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.3, 0.3, 0.3);
			glVertex3f(-0.5f, 0.5f, 0.0f); 
			glVertex3f( 0.5f, 0.5f, 0.0f); 
			glVertex3f( 0.5f,-0.5f, 0.0f); 
			glVertex3f(-0.5f,-0.5f, 0.0f); 
	   glEnd();

       glBegin(GL_TRIANGLES);

			glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0, 1.0, 0.0);
			glVertex3f(-0.5f, -0.5f, 0.0f);          
			glVertex3f(0.5f,-0.5f, 0.0f);   
			glVertex3f(0.0f,0.0f, 1.0f);         

			glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 1.0, 1.0, 0.0);
			glVertex3f(-0.5f, 0.5f, 0.0f);          
			glVertex3f(0.5f,0.5f, 0.0f);   
			glVertex3f(0.0f,0.0f, 1.0f);        

			glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 1.0, 0.0, 0.0);
			glVertex3f(0.5f, -0.5f, 0.0f);          
			glVertex3f(0.5f,0.5f, 0.0f);   
			glVertex3f(0.0f,0.0f, 1.0f); 
	
			glVertexAttrib3f(GLT_ATTRIBUTE_COLOR, 0.0, 0.0, 1.0);
			glVertex3f(-0.5f, -0.5f, 0.0f);          
			glVertex3f(-0.5f,0.5f, 0.0f);   
			glVertex3f(0.0f,0.0f, 1.0f); 
	   glEnd();          

    // Perform the buffer swap to display back buffer
    glutSwapBuffers();
	
	M3DVector3f at={0,0,0};
	M3DVector3f up={0,0,1};
	M3DVector3f cam;
	float angle=timer.GetElapsedSeconds();
	cam[0]=6.0*sin(angle);
	cam[1]=6.0*cos(angle);
	cam[2]=6.0f; 
	LookAt(cameraframe, cam, at, up);

glutPostRedisplay();
}


///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs

int main(int argc, char* argv[]) {
  

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Triangle");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }

    SetupRC();

    glutMainLoop();
    return 0;
}