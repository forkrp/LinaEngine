/*
Author: Inan Evin
www.inanevin.com

MIT License

Lina Engine, Copyright (c) 2018 Inan Evin

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

4.0.30319.42000
10/15/2018 10:33:27 AM

*/

#include "pch.h"
#include "Rendering/Lina_Mesh.h"  
#include <vector>

/*Lina_Mesh::Lina_Mesh(std::vector<Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
{
	Vertices = _Vertices;
	Indices = _Indices;
}*/

void Lina_Mesh::InitMesh()
{
	//This function assigns unique ID to our Vertex Buffer Object & Index Buffer Object. 
	glCreateBuffers(1, &m_VBO);
	glCreateBuffers(1, &m_IBO);
	size = 0;
}


/*void Lina_Mesh::InitMeshWithExistingVertices()
{
	//This function assigns unique ID to our Vertex Buffer Object & Index Buffer Object. 
	glCreateBuffers(1, &m_VBO);
	glCreateBuffers(1, &m_IBO);

	size = 0;


	// Init Index & Vertex array.
	m_Indices = new unsigned int[Indices.size() * sizeof(int)];
	m_Vertices = new Vertex[Vertices.size() * sizeof(Vertex)];

	// Set index & vertex array elements.
	int counter = 0;

	for (std::vector<Vertex>::iterator it = Vertices.begin(); it != Vertices.end(); it++)
	{
		m_Vertices[counter] = *it;
		counter++;
	}
	counter = 0;
	for (std::vector<unsigned int>::iterator it = Indices.begin(); it != Indices.end(); it++)
	{
		m_Indices[counter] = (*it) - 1;
		counter++;
	}

	// Set sizes.
	iSize = Indices.size();
	vSize = Vertices.size();
}*/

// Used for loaded .obj files. 
void Lina_Mesh::InitBuffers()
{
	size = iSize;

	//This function binds the Vertex Buffer Object..
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	//After binding the buffer we specify the size of the data and the actual data itself.
	glBufferData(GL_ARRAY_BUFFER, vSize * sizeof(m_Vertices[0]), m_Vertices, GL_STATIC_DRAW);

	// Bind & Buffer for index buffer object.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize * sizeof(m_Indices[0]), m_Indices, GL_STATIC_DRAW);
}

void Lina_Mesh::AddVertices(Vertex* vertices, unsigned int vSize, int* indices, unsigned int iSize)
{
	AddVertices(vertices, vSize, indices, iSize, false);
}

// Used for manually created meshes.
void Lina_Mesh::AddVertices(Vertex* vertices, unsigned int vsize, int* indices, unsigned int isize, bool calculateNormals)
{

	if (calculateNormals)
		CalculateNormals(vertices, vsize, indices, isize);

	size = isize;

	//This function binds the Vertex Buffer Object..
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	//After binding the buffer we specify the size of the data and the actual data itself.
	glBufferData(GL_ARRAY_BUFFER, vsize, vertices, GL_STATIC_DRAW);

	// Bind & Buffer for index buffer object.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize , indices, GL_STATIC_DRAW);
}

void Lina_Mesh::Draw()
{

	glEnableVertexAttribArray(0);	// Positions
	glEnableVertexAttribArray(1);	// Texture Coordinates
	glEnableVertexAttribArray(2);	// Normals

	// Bind buffers & draw.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, Lina_Vertex::SIZE * 4, (void*)0);	// Positions.
	glVertexAttribPointer(1, 2, GL_FLOAT, false, Lina_Vertex::SIZE * 4, (void*)12);	// Texture coords. 4 bytes per floating point number. x-y-z-xCor-yCor.
	glVertexAttribPointer(2, 3, GL_FLOAT, false, Lina_Vertex::SIZE * 4, (void*)20);	// Normals

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

void Lina_Mesh::CalculateNormals(Vertex* vertices, int vSize, int* indices, int iSize)
{
	for (int i = 0; i < iSize; i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		// Form 2 lines that will have the up direction as their cross product.
		Vector3 v1 = vertices[i1].GetPosition() - vertices[i0].GetPosition();
		Vector3 v2 = vertices[i2].GetPosition() - vertices[i0].GetPosition();

		// Calculate up.
		Vector3 normal = Vector3::Cross(v1, v2).normalized();

		// Add normals to the vertices.
		vertices[i0].SetNormal(vertices[i0].GetNormal() + normal);
		vertices[i1].SetNormal(vertices[i0].GetNormal() + normal);
		vertices[i2].SetNormal(vertices[i0].GetNormal() + normal);
	}

	// Set the length to one to get the appropriate normalized length & direction for every single vertex.
	for (int i = 0; i < vSize; i++)
		vertices[i].SetNormal(vertices[i].GetNormal().normalized());
}