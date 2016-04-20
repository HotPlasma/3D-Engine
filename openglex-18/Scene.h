#pragma once

#include "stdafx.h"
#include "Model.h"
#include <vector>
#include "TextureLoader.h"

class Scene
{
public:
	Scene(string filename);
	void ReadSceneFile(string filename);
	void LoadScene(Scene SceneToLoad);
	vector<Model> ModelList;

private:
	vector<GLuint> m_textureID;
	TextureLoader* tl;
	void Rotation(sf::Vector3f RotationVector);
	void Scale(sf::Vector3f ScaleVector);
	void Transform(sf::Vector3f TransformVector);
};