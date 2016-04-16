#pragma once

#include "stdafx.h"
#include <string>
#include "ModelReader.h"

using namespace std;

class Model
{
private:
	string sFileName;
	sf::Vector3f ModelPosition;
	string sTexture;

	GLuint m_textureID;
	ModelReader* m_modelReader;
public:
	Model(string FileLocation, sf::Vector3f Position, string TextureLocation);
	string GetFileLocation();
	sf::Vector3f GetPosition();
	string GetTextureLocation();

	void SetFileLocation(string Location);
	void SetPosition(sf::Vector3f Position);
	void SetTextureLocation(string Location);

	void LoadModel(GLuint newTexture, string Model);
	void DrawModel(bool drawWithNormals, bool drawWithTexture);


};

