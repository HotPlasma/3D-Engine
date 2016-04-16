#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	CameraPos = sf::Vector3f(1.0f, 1.0f, 1.0);
	CameraView = sf::Vector3f(0.0f, 1.0f, 0.0f);
}

sf::Vector3f Camera::GetCameraPos()
{
	return CameraPos;
}

sf::Vector3f Camera::GetCameraView()
{
	return CameraView;
}

void Camera::ProcessUserInput(float yAngle, float zAngle)
{
		//sf::Vector2f fMousePos = (sf::Vector2f)MousePos / 1000.0f;
		sf::Vector3f ViewVec = CameraView - CameraPos;
	
		CameraView.x = CameraPos.x + (float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z;
		//cout <<  ViewVec.x << " " << ViewVec.z << endl;
		CameraView.y += zAngle * 2.0f;
		CameraView.z = CameraPos.z + (float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z;
	
		float fraction = 0.1f;
	
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			CameraPos.x += ((float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z) * fraction;
			CameraPos.z += ((float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z) * fraction;
			CameraView.x += ((float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z) * fraction;
			CameraView.z += ((float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z) * fraction;
		}
	
		 if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			 CameraPos.x -= ((float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z) * fraction;
			 CameraPos.z -= ((float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z) * fraction;
			 CameraView.x -= ((float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z)* fraction;
			 CameraView.z -= ((float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z) * fraction;
		}
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			CameraPos.z += ((float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z) * fraction;
			CameraPos.x -= ((float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z) * fraction;
			CameraView.z += ((float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z)* fraction;
			CameraView.x -= ((float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z) * fraction;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			CameraPos.z -= ((float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z) * fraction;
			CameraPos.x += ((float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z) * fraction;
			CameraView.z -= ((float)cosf(-yAngle) * ViewVec.x - sinf(-yAngle) * ViewVec.z)* fraction;
			CameraView.x += ((float)sinf(-yAngle) * ViewVec.x + cosf(-yAngle) * ViewVec.z) * fraction;
		}
}