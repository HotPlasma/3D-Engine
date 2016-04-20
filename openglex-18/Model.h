#pragma once

#include "stdafx.h"
#include <string>
#include "ModelReader.h"

using namespace std;

class Model
{
private:
	string sFileName;
	string sTexture;

	sf::Vector3f ModelPosition;
	sf::Vector3f ModelRotation;
	sf::Vector3f ModelScale;


	GLuint m_textureID;
	ModelReader* m_modelReader;
public:
	Model();
	Model(string FileLocation, string TextureLocation, sf::Vector3f Position, sf::Vector3f Rotation, sf::Vector3f Scale);
	string GetFileLocation();
	GLuint GetTextureLocation();
	sf::Vector3f GetPosition();
	sf::Vector3f GetRotation();
	sf::Vector3f GetScale();

	void SetTexture(GLuint TextureID);
	void SetFileLocation(string NewLocation);
	void SetTextureLocation(string NewLocation);
	void SetPosition(sf::Vector3f NewPosition);
	void SetRotation(sf::Vector3f NewRotation);
	void SetScale(sf::Vector3f NewScale);

	void LoadModel(string Model);
	void DrawModel(bool drawWithNormals, bool drawWithTexture);


};

