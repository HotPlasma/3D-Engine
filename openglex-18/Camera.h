#pragma once

#include "stdafx.h"

class Camera
{
private:
	sf::Vector3f CameraPos;
	sf::Vector3f CameraView;
public:
	Camera();
	sf::Vector3f GetCameraPos();
	sf::Vector3f GetCameraView();

	void ProcessUserInput(float yAngle, float zAngle);
};