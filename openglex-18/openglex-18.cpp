// openglex-18.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#include "TextureLoader.h"
#include "ModelReader.h"
#include "Model.h"
#include "Camera.h"
#include "Scene.h"

#if 0

#define GL_GLEXT_PROTOTYPES 1		// ALLOW USE OF OPEN GL 1.5+ functionality
#define GLEW_STATIC

#include <SFML\glew.h>		// make 1.5 functions into function pointers - bind at runtime.
#include <SFML\OpenGL.hpp>	
#include <SFML\Graphics.hpp>

#include <SFML\wglext.h>
#include <SFML\glext.h>


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



// default light position
GLfloat m_lightPosition[] = {-50.0f, +50.0f, +100.0f, 1.0f};


void SetMaterialDefault() // A default material for most models
{
	GLfloat materialWhiteAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat materialWhiteDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat materialWhiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};		// so keeps light colour
	GLfloat materialWhiteShininess = 700.0f;
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}

void SetMaterialNoDiffuse() // A material for models which should have no shadows e.g. skybox
{
	GLfloat materialWhiteAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat materialWhiteDiffuse[] = {0.0f, 0.0f, 0.0f, 0.0f};
	GLfloat materialWhiteSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};		// so keeps light colour
	GLfloat materialWhiteShininess = 0.0f;
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT,GL_AMBIENT, materialWhiteAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialWhiteDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialWhiteSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, materialWhiteShininess);
}



void SetLightPosition(float x, float y, float z) // Sets light position to given values
{
	m_lightPosition[0] = x;
	m_lightPosition[1] = y;
	m_lightPosition[2] = z;
	glLightfv(GL_LIGHT0, GL_POSITION, m_lightPosition);
}

void ConfigureLightSources() // Sets up openGL light
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
	SetMaterialDefault();
}


int main()
{
	// Create the camera
	Camera FirstPersonView;

	// Used for tilling textures
	const int PLANESIZE = 2048;
	const int TEXSCALE = 512;

	// SFML-2.3.2 depth buffering fails unless a more specfic window is created - as below
	int depthBits = 24;
	int stencilBits = 8;
	int antiAliasingLevel = 2;
	int majorVersion = 3;
	int minorVersion = 3;

	// Create window
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
	
	// enable this to setup the lighting after normals added
	ConfigureLightSources();


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
				// Allows first person view changing with mouse movement
				sf::Vector2i WindowOrigin(SCREEN_WIDTH >> 1, SCREEN_HEIGHT >> 1); // Middle of the screen
				sf::Vector2i mousePos = sf::Mouse::getPosition(window); // Get displacement of mouse
				sf::Mouse::setPosition(WindowOrigin, window); // Reset mouse at origin

				float yAngle = (WindowOrigin - mousePos).x / 1000.0f; 
				float zAngle = (WindowOrigin - mousePos).y / 1000.0f;

				FirstPersonView.ProcessUserInput(yAngle, zAngle); // Send mouse position data to be processed in order to move camera

				// Set up camera as a gluLookAt function
				gluLookAt(FirstPersonView.GetCameraPos().x, FirstPersonView.GetCameraPos().y, FirstPersonView.GetCameraPos().z, FirstPersonView.GetCameraView().x, FirstPersonView.GetCameraView().y, FirstPersonView.GetCameraView().z, 0, 1, 0);
				

				window.setMouseCursorVisible(false); // Mouse is invisible

			}

			SetLightPosition(100,100, -100); // Sets light position every frame from a given angle

			// call the rendering code

			// Tiling of a texture for the ground
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, AirPort.ModelList[0].GetTextureLocation()); // Ground/Plane model should be the first model loaded in
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);			
			glVertex3f(-PLANESIZE / 2.0f, -0.0003f, PLANESIZE / 2.0f);
			glTexCoord2f(TEXSCALE, 0);  
			glVertex3f(PLANESIZE / 2.0f, -0.0003f, PLANESIZE / 2.0f);
			glTexCoord2f(TEXSCALE, TEXSCALE); 
			glVertex3f(PLANESIZE / 2.0f, -0.0003f, -PLANESIZE / 2.0f);
			glTexCoord2f(0, TEXSCALE);  
			glVertex3f(-PLANESIZE / 2.0f, -0.0003f, -PLANESIZE / 2.0f);

			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);

			// Loop through the remainder of the Scene and render all models with given materials
			for (int i = 1; i < AirPort.ModelList.size(); i++)
			{
				glPushMatrix();
				if (AirPort.ModelList[i].GetMaterial() == 1) // Default material
				{
					SetMaterialDefault();
				}
				else if (AirPort.ModelList[i].GetMaterial() == 2) // No diffuse material
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