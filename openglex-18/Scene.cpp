#include "stdafx.h"
#include "Scene.h"

Scene::Scene(string filename)
{
	ReadSceneFile(filename);
}

void Scene::ReadSceneFile(string filename)
{
	std::fstream sceneFile(filename, std::ios_base::in);

	if (!sceneFile.is_open())
	{
		std::cerr << "File " << filename << " not found.";
		DebugBreak();
		throw std::invalid_argument("File not found");
		return;	//ERROR!!!
	}

	string line;
	string data;

	while (getline(sceneFile, line))
	{

	}
}