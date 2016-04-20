// openglex-18.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#include "TextureLoader.h"
#include "ModelReader.h"
#include "Model.h"
#include "Camera.h"
#include "Scene.h"

#if 0

#define GL_GLEXT_PROTOTYPES 1		// ALLOW USE OF OPEN GL 1.5+ functionality!!!
#define GLEW_STATIC
// this needs to be the first include
#include <SFML\glew.h>		// make 1.5 functions into function pointers - bind at runtime.
#include <SFML\OpenGL.hpp>	// glew BEFORE opengl headers!!!!
#include <SFML\Graphics.hpp>
// put these inside the SFML\include folder
#include <SFML\wglext.h>
#include <SFML\glext.h>
// note weve also added the lib file glew32s.lib

// shader.h included these
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#endif

using namespace std;

// better to use const int rather than define
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;
const float REFRESH_RATE = 0.03f;

float angle = 0.0f;


// we make the light position a global variable
// (later would belong to a class handling lighting)

// put light behind and above us on left
GLfloat m_lightPosition[] = {-50.0f, +50.0f, +100.0f, 1.0f};

// character buffers for the raw textures
//char textureData1[256*256*3];
//char textureData2[256*256*3];
// array of OpenGL texture objects 
GLuint m_textureID[7];

GLfloat m_xRotationAngle = 45.0f;
GLfloat m_xAngleIncrement = 0.4f;

GLfloat m_yRotationAngle =  0.0f;
GLfloat m_yAngleIncrement = 0.5f;

//vector<ModelReader*> m_modelReader;

void SetMaterialWhite()
{
	GLfloat materialWhiteAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat materialWhiteDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat materialWhiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};		// so keeps light colour
	GLfloat materialWhiteShininess = 700.0f;
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}

void SetMaterialNoDiffuse()
{
	GLfloat materialWhiteAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat materialWhiteDiffuse[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat materialWhiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};		// so keeps light colour
	GLfloat materialWhiteShininess = 0.0f;
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}

void SetMaterialRedShiny()
{
	GLfloat materialRedAmbient[] = {0.2f, 0.0f, 0.0f, 1.0f};
	GLfloat materialRedDiffuse[] = {0.8f, 0.0f, 0.0f, 1.0f};
	GLfloat materialRedSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat materialRedShininess = 700.0f;						// try 500 for a sharper specular highlight
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);
	glMaterialfv(GL_FRONT,GL_AMBIENT, materialRedAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialRedDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialRedSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialRedShininess);
}


void SetLightPosition(float x, float y, float z)
{
	m_lightPosition[0] = x;
	m_lightPosition[1] = y;
	m_lightPosition[2] = z;
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
}

void ConfigureLightSources()
{
	GLfloat lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat noLight[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat lightModelAmbient[] = {0.3f, 0.3f, 0.3f, 1.0};

	// put light behind and above us on left
	SetLightPosition(-50.0f, +50.0f, + 100.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
	glLightfv(GL_LIGHT0, GL_AMBIENT, noLight);			// no ambient light from the source

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);	// use global ambient instead

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// with lighting glColor no longer applies
	// create a default material for the models
	SetMaterialWhite();
}


int main()
{
	// Create the camera
	Camera FirstPersonView;

	// SFML-2.3.2 depth buffering fails unless we create a more specific window - as below
	int depthBits = 24;
	int stencilBits = 8;
	int antiAliasingLevel = 2;
	int majorVersion = 3;
	int minorVersion = 3;

	sf::ContextSettings context(depthBits, stencilBits, antiAliasingLevel, majorVersion, minorVersion)	;
	sf::Window window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "SFML Window", 7U, context);

	// check we got the settings we need
	sf::ContextSettings windowSettings = window.getSettings();


    // Create a clock for measuring time elapsed
    sf::Clock clock;
	
	// always setup OpenGL After the window is configured
    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

	// draw single sided faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    // Setup a perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.f, (float)SCREEN_WIDTH/SCREEN_HEIGHT, 1.f, 500.f);

	Scene AirPort("scenes\\Airport Scene.txt");
	

	// TODO
	// enable this to setup the lighting after normals added
	ConfigureLightSources();

	// load in the model here from the models subdirectory
	//m_modelReader.push_back(new ModelReader(Cube.GetFileLocation()));

    // Start game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event Event;
        while (window.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                window.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                window.close();

            // Resize event : adjust viewport
            if (Event.type == sf::Event::Resized)
                  glViewport(0, 0, Event.size.width, Event.size.height);
        }
        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        window.setActive();

		if(clock.getElapsedTime().asSeconds()>REFRESH_RATE)
		{

			// Clear colour and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Apply some transformations
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			HWND handle = window.getSystemHandle();

			
			if (GetFocus() == handle) // Allows alt-tabing out of screen
			{
				sf::Vector2i WindowOrigin(SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1);
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Mouse::setPosition(WindowOrigin, window);

				float yAngle = (WindowOrigin - mousePos).x / 1000.0f;
				float zAngle = (WindowOrigin - mousePos).y / 1000.0f;

				FirstPersonView.ProcessUserInput(yAngle, zAngle);

				gluLookAt(FirstPersonView.GetCameraPos().x, FirstPersonView.GetCameraPos().y, FirstPersonView.GetCameraPos().z, FirstPersonView.GetCameraView().x, FirstPersonView.GetCameraView().y, FirstPersonView.GetCameraView().z, 0, 1, 0);
				

				window.setMouseCursorVisible(false);

			}

			SetLightPosition(100,100, -100);


			// try SetMaterialRedShiny(); here tosee the effect

			// call the rendering code

			for (int i = 0; i < AirPort.ModelList.size(); i++)
			{
				glPushMatrix();
					SetMaterialWhite();
					if( i==6 )
					{
						SetMaterialNoDiffuse();
					}
					AirPort.ModelList[i].DrawModel(true,true);
				glPopMatrix();
			}

			clock.restart();
		    // Finally, display rendered frame on screen
			window.display();
		}
    }
    return EXIT_SUCCESS;
}