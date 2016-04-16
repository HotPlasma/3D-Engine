#include "stdafx.h"
#include "Model.h"

Model::Model(string FileLocation, sf::Vector3f Position, string TextureLocation)
{
	sFileName = FileLocation;
	ModelPosition = Position;
	sTexture = TextureLocation;
}

string Model::GetFileLocation()
{
	return sFileName;
}

sf::Vector3f Model::GetPosition()
{
	return ModelPosition;
}
string Model::GetTextureLocation()
{
	return sTexture;
}

void Model::SetFileLocation(string Location)
{
	sFileName = Location;
}
void  Model::SetPosition(sf::Vector3f Position)
{
	ModelPosition = Position;
}
void  Model::SetTextureLocation(string Location)
{
	sTexture = Location;
}

void Model::LoadModel(GLuint newTexture, string Model)
{
	m_textureID = newTexture;
	m_modelReader = new ModelReader(Model);
}

void Model::DrawModel(bool drawWithNormals, bool drawWithTexture)
{
	// activate and specify pointer to vertex array

	glEnableClientState(GL_VERTEX_ARRAY);
	vector<float>& vertices = m_modelReader->GetVertices();
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);



	// pointer to normal array
	if (drawWithNormals)
	{
		glEnableClientState(GL_NORMAL_ARRAY);

		vector<float>&normals = m_modelReader->GetNormals();

		glNormalPointer(GL_FLOAT, 0, &normals[0]);
	}

	if (drawWithTexture)
	{
		glEnable(GL_TEXTURE_2D);
		vector<float>& textureCoordinates = m_modelReader->GetTextureCoordinates();
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glTexCoordPointer(2, GL_FLOAT, 0, &textureCoordinates[0]);
	}


	// draw the shape...
	// TODO
	glDrawArrays(GL_TRIANGLES, 0, (unsigned int)vertices.size() / 3);
	// deactivate vertex arrays after drawing


	if (drawWithNormals)
	{
		// deactivate
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (drawWithTexture)
	{
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}
}