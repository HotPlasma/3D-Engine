#pragma once

#include "stdafx.h"
#include <string>
#include "ModelReader.h"

using namespace std;

class Model
{
private:
	string sFileName; // File location of the obj
	string sTexture; // File location of the texture
	int ModelMaterial; // Which material the model with have. Different integers are different materials.

	sf::Vector3f ModelPosition; // Position of Model
	sf::Vector3f ModelRotation; // Rotation of Model
	sf::Vector3f ModelScale; // Scale of Model

	GLuint m_textureID; // ID of the texture
	ModelReader* m_modelReader; // Model Reader for the render the model
public:
	Model(); // Constructor
	Model(string FileLocation, string TextureLocation, sf::Vector3f Position, sf::Vector3f Rotation, sf::Vector3f Scale, int MaterialID); // Full constructor
	string GetFileLocation(); // Returns location of obj
	GLuint GetTextureLocation(); // Returns location of texture
	sf::Vector3f GetPosition(); // Returns rosition of model
	sf::Vector3f GetRotation(); // Returns rotation of model
	sf::Vector3f GetScale(); // Returns scale of model
	int GetMaterial(); // Returns materialID

	void SetTexture(GLuint TextureID); // Set texture ID
	void SetFileLocation(string NewLocation); // Set obj location
	void SetTextureLocation(string NewLocation); // Set texture location
	void SetPosition(sf::Vector3f NewPosition); // Set model position
	void SetRotation(sf::Vector3f NewRotation); // Set model rotation
	void SetScale(sf::Vector3f NewScale); // Set model scale
	void SetMaterial(int MaterialID); // Set model MaterialID

	void LoadModel(string Model); // Loads in the model to be rendered
	void DrawModel(bool drawWithNormals, bool drawWithTexture); // Draws model


};

