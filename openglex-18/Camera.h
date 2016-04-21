#pragma once

#include "stdafx.h"

class Camera // Class Designed to hold my camera positions and navigate it on user input
{
private:
	sf::Vector3f CameraPos; // Position of the Camera
	sf::Vector3f CameraView; // View of the Camera
public:
	Camera();
	sf::Vector3f GetCameraPos(); // Returns Camera Pos
	sf::Vector3f GetCameraView(); // Returns Camera View

	void ProcessUserInput(float yAngle, float zAngle); // Proccesses user input in order to move camera
};