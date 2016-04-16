#include "stdafx.h"
#include "ModelReader.h"


ModelReader::ModelReader(string filename)
{
	ReadModelObjData(filename);

	//expand the data suitable for lDrawArrays()
	CreateExpandedVertices();
	CreateExpandedNormals();
	CreateExpandedTextureCoordinates();
}

ModelReader::~ModelReader(void)
{

}

void ModelReader::ReadModelObjData(string filename)
{
	std::fstream modelfile(filename, std::ios_base::in);

	if(!modelfile.is_open())
	{
		std::cerr << "File " << filename << " not found.";
		DebugBreak();
		throw std::invalid_argument( "File not found" );
		return;	//ERROR!!!
	}

	string line;
	string token = "";
	while (getline(modelfile, line))
	{
		istringstream iss(line);

		// process the line
		token = "";

		iss >> token; // read to first whitespace
	
		if (token == "#")
		{
			// ignore rest of line
			// iss >> s;
		}
		else if (token == "g")
		{
			// read the model name
			iss >> m_modelName;
			// ignore rest of line
		}
		else if (token == "v")
		{
			ProcessVertexLine(iss);
		}
		else if (token == "vn")
		{
			ProcessVertexNormalLine(iss);
		}
		else if (token == "vt")
		{
			ProcessVertexTextureLine(iss);
		}
		else if (token == "f")
		{
			ProcessFaceLine(iss);
		}
		else
		{
			// ignore any line without these qualifiers
			// ignore rest of line
		}
	}
	modelfile.close();

}

void ModelReader::ProcessVertexLine(istringstream& iss)
{

	// TODO - store 3 floats in m_vertices vector
	float fVertex;

	for (int i = 0; i < 3; i++)
	{
		iss >> fVertex;

		m_vertices.push_back(fVertex);
	}

}

void ModelReader::ProcessVertexNormalLine(istringstream& iss)
{
	// TODO - store 3 floats in m_vertexNormals vector
	float fVertexNormal;

	for (int i = 0; i < 3; i++)
	{
		iss >> fVertexNormal;

		m_vertexNormals.push_back(fVertexNormal);
	}
}

void ModelReader::ProcessVertexTextureLine(istringstream& iss)
{
	// TODO - store 2 floats in m_vertexTextureCoordinates vector
	float fVertexTextureLine;

	for (int i = 0; i < 2; i++)
	{
		iss >> fVertexTextureLine;

		m_vertexTextureCoordinates.push_back(fVertexTextureLine);
	}
}


void ModelReader::ProcessFaceLine(istringstream& iss)
{
	// TODO - process 3 types of f line data
	int iFaces;
	static const int forwardSlash = 0x2F;

	for (int i = 0; i < 3; i++)
	{
		iss >> iFaces;
		m_faceVertexIndices.push_back(iFaces - 1);
		// now look for a slash
		int lookAhead = iss.peek();
		if (lookAhead == forwardSlash)
		{
			// if its a slash we have either 1/2/3 or 1//2
			// get rid of this slash
			int discard = iss.get();
			// now we can look at what is after the slash
			// is it another slash?
			lookAhead = iss.peek();

			if (lookAhead == forwardSlash)
			{
				// if it is then get rid of it
				discard = iss.get();

				// now we read the normal - ie it was 1//2

				int iNormal = 0;
				iss >> iNormal;

				m_faceNormalIndices.push_back(iNormal - 1);
			}
			else
			{
				// here we only had 1 slash so it must be 1/2/3
				// read the teture index
				int iTexture;
				iss >> iTexture;
				m_faceTextureIndices.push_back(iTexture - 1);
				// get the slash
				discard = iss.get();
				// read the normal
				int iNormal;
				iss >> iNormal;
				m_faceNormalIndices.push_back(iNormal - 1);

			}
		}
		else
		{
			// nothing to do here - is f 1 2 3
		}
	}
}


void ModelReader::CreateExpandedVertices()
{
	for (std::vector<unsigned int>::iterator it = m_faceVertexIndices.begin() ; it != m_faceVertexIndices.end(); ++it)
	{
		int vertexNumber = (*it);
		// 3 floats per triangular face
		
		int a;
		a = (*it) * 3;
		// TODO - index and copy 3 floats to the m_vertexTriplets vector
		for (int i = 0; i < 3; i++)
		{
			
			//cout << a << endl;


			float v = m_vertices[a + i];


			m_vertexTriplets.push_back(v);
		}	
	}
}
void ModelReader::CreateExpandedNormals()
{
	// we create a list of normal triplets for each face (with duplicates)
	// this will then be the same size as the vertexTriplets vector

	for (std::vector<unsigned int>::iterator it = m_faceNormalIndices.begin() ; it != m_faceNormalIndices.end(); ++it)
	{
		//here we have a 1 base index
		// get the face number as 0 indexed
		int vertexNormalNumber = (*it);

		int a;
		a = (*it) * 3;

		for (int i = 0; i < 3; i++)
		{
			
			//cout << a << endl;

			if ((a + i) >= (int)m_vertexNormals.size())
			{

			}
			else
			{
				float v = m_vertexNormals[a + i];
			

				m_vertexNormalTriplets.push_back(v);
			}
		}	


		// TODO - index and copy 3 floats to the m_vertexNormalTriplets vector

	}
}
void ModelReader::CreateExpandedTextureCoordinates()
{
	// create actual vertices here (with duplicates)
	// this is in the form that glDrawArrays can work with
	//
	// assume triangles so far
	// assert((faceVertexIndices.size() % 3) == 0);

	for (std::vector<unsigned int>::iterator it = m_faceTextureIndices.begin() ; it != m_faceTextureIndices.end(); ++it)
	{
		int textureNumber = (*it);

		int a = textureNumber * 2;

		for (int i = 0; i < 2; i++)
		{
			m_vertexTexturePairs.push_back(m_vertexTextureCoordinates[a + i]);
		}
	}
	// TODO  - index and copy 2 floats to the m_vertexTexturePairs vector

}


// Get methods gove access to the vector data

vector<float>& ModelReader::GetVertices()
{
	return m_vertexTriplets;
}
vector<float>& ModelReader::GetNormals()
{
	return m_vertexNormalTriplets;
}
vector<float>& ModelReader::GetTextureCoordinates()
{
	return m_vertexTexturePairs;
}