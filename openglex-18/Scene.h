#pragma once

#include "stdafx.h"
#include "Model.h"

class Scene
{
private:
	vector<Model>ModelList;
public:
	Scene(string filename);
	void ReadSceneFile(string filename);
	void LoadScene();
};