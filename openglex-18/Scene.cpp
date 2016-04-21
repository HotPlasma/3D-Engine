#include "stdafx.h"
#include "Scene.h"

Scene::Scene(string filename)
{
	ReadSceneFile(filename); // Send file to be read
	tl = new TextureLoader(); // Create a new textureloader
}

void Scene::ReadSceneFile(string filename)
{

	Model Temp; // Temperary model to be pushed into Scene Vector


	std::fstream sceneFile(filename, std::ios_base::in);

	if (!sceneFile.is_open()) // If file not open
	{
		std::cerr << "File " << filename << " not found."; // Return an error
		DebugBreak();
		throw std::invalid_argument("File not found");
		return;	//ERROR!!!
	}

	string line; // Line being read
	string data; // Current data being read

	while (getline(sceneFile, line))
	{
		istringstream iss(line);
		data = "";
		while(data != "E") // While its not the end of the line
		{
			iss >> data; // Read next bit of data

			if (data == "O") // Have reached .obj file location
			{
				iss >> data;
				Temp.SetFileLocation(data); // Set temp to .obj location
			}
			else if(data == "T") // Have reached texture location
			{
				//Load texture into texture vector
				iss >> data; 
				m_textureID.resize(m_textureID.size() + 1); // Make the textureID vector 1 bigger
				// Set texture to end of vector
				tl->LoadBMP(data, m_textureID[m_textureID.size() - 1], true); 
				Temp.SetTexture(m_textureID[m_textureID.size() - 1]); 
			}

			else if(data == "Tr") // Model needs to be translated
			{
				// Create the translator vector3f
				iss >> data;
				Temp.SetPosition(sf::Vector3f(stof(data), Temp.GetPosition().y, Temp.GetPosition().z));

				iss >> data;
				Temp.SetPosition(sf::Vector3f(Temp.GetPosition().x, stof(data), Temp.GetPosition().z));

				iss >> data;
				Temp.SetPosition(sf::Vector3f(Temp.GetPosition().x, Temp.GetPosition().y, stof(data)));
			}

			else if(data == "R") // Model needs to be rotated
			{
				// Create rotation vector3f
				iss >> data;
				Temp.SetRotation(sf::Vector3f(stof(data), Temp.GetRotation().y, Temp.GetRotation().z));

				iss >> data;
				Temp.SetRotation(sf::Vector3f(Temp.GetRotation().x, stof(data), Temp.GetRotation().z));

				iss >> data;
				Temp.SetRotation(sf::Vector3f(Temp.GetRotation().x, Temp.GetRotation().y, stof(data)));
			}

			else if(data == "S") // Model needs to be scaled
			{
				// Create scale vector3f
				iss >> data;
				Temp.SetScale(sf::Vector3f(stof(data), Temp.GetScale().y, Temp.GetScale().z));

				iss >> data;
				Temp.SetScale(sf::Vector3f(Temp.GetScale().x, stof(data), Temp.GetScale().z));

				iss >> data;
				Temp.SetScale(sf::Vector3f(Temp.GetScale().x, Temp.GetScale().y, stof(data)));
			}

			else if (data == "M") // Assign material to model
			{
				iss >> data;
				Temp.SetMaterial(stof(data));
			}
		}
		ModelList.push_back(Temp); // Push back temp into list of models to be drawn
	}
	sceneFile.close(); // Close file when finished

	for (int i = 0; i < ModelList.size(); i++)
	{
		ModelList[i].LoadModel(ModelList[i].GetFileLocation()); // Load in all models to be ready for drawing
	}
}
