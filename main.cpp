#include "tardis.h"
#include "vortex.h"
#include "MVPprovider.h"

#include "helpers.h"

#include <Windows.h>
#include <GL\glfw.h>

const int FPS = 30;

int initializeWindow()
{
	if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return -1;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        return -1;
    }

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "Time and Relative Dimension In Space" );

    glfwEnable( GLFW_STICKY_KEYS );

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

int main(int agrc, char** argv)
{
	initializeWindow();

	MVPprovider* mvp = MVPprovider::Instance();
	mvp->setScreenSize(1024, 768);

	//Tardis* m_tardis;
	//m_tardis = new Tardis();

	Vortex* m_vortex;
	m_vortex = new Vortex();

	m_vortex->initialize(NULL, "clouds.dds");
	//m_tardis->initialize();

	//double thisRedraw, lastRedraw = glfwGetTime();

	do
	{
		//mvp->calculateMatrices();
		computeMatricesFromInputs();

		m_vortex->process();
		//m_tardis->process();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_vortex->render();
		//m_tardis->render();

		glfwSwapBuffers();

		//thisRedraw = glfwGetTime() - lastRedraw;
		//printf("%d", 1000/FPS - thisRedraw*1000);
		//if (thisRedraw > 0) Sleep(1000/FPS - thisRedraw*1000);	
	} while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ) );

	return 0;
}