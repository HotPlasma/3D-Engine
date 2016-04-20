#include "stdafx.h"
#include "Scene.h"

Scene::Scene(string filename)
{
	ReadSceneFile(filename);
	tl = new TextureLoader();
}

void Scene::ReadSceneFile(string filename)
{

	Model Temp;


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
		istringstream iss(line);
		data = "";
		while(data != "E")
		{
			iss >> data;		

			if (data == "O")
			{
				iss >> data;
				Temp.SetFileLocation(data);
			}
			else if(data == "T")
			{
				//Load texture into texture vector
				iss >> data;
				m_textureID.resize(m_textureID.size() + 1);
				tl->LoadBMP(data, m_textureID[m_textureID.size() - 1]);
				Temp.SetTexture(m_textureID[m_textureID.size() - 1]);
			}

			else if(data == "Tr")
			{
				iss >> data;
				Temp.SetPosition(sf::Vector3f(stof(data), Temp.GetPosition().y, Temp.GetPosition().z));

				iss >> data;
				Temp.SetPosition(sf::Vector3f(Temp.GetPosition().x, stof(data), Temp.GetPosition().z));

				iss >> data;
				Temp.SetPosition(sf::Vector3f(Temp.GetPosition().x, Temp.GetPosition().y, stof(data)));
			}

			else if(data == "R")
			{
				iss >> data;
				Temp.SetRotation(sf::Vector3f(stof(data), Temp.GetRotation().y, Temp.GetRotation().z));

				iss >> data;
				Temp.SetRotation(sf::Vector3f(Temp.GetRotation().x, stof(data), Temp.GetRotation().z));

				iss >> data;
				Temp.SetRotation(sf::Vector3f(Temp.GetRotation().x, Temp.GetRotation().y, stof(data)));
			}

			else if(data == "S")
			{
				iss >> data;
				Temp.SetScale(sf::Vector3f(stof(data), Temp.GetScale().y, Temp.GetScale().z));

				iss >> data;
				Temp.SetScale(sf::Vector3f(Temp.GetScale().x, stof(data), Temp.GetScale().z));

				iss >> data;
				Temp.SetScale(sf::Vector3f(Temp.GetScale().x, Temp.GetScale().y, stof(data)));
			}
		}
		ModelList.push_back(Temp);
	}
	sceneFile.close();

	for (int i = 0; i < ModelList.size(); i++)
	{
		ModelList[i].LoadModel(ModelList[i].GetFileLocation());
	}
}

//void Scene::LoadScene(vector<Model> SceneToLoad);
//{
//	
//}