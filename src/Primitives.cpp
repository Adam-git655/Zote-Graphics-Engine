#include "Primitives.h"

static float cubeVertices[] = {
	// positions only
	-0.5f, -0.5f, -0.5f,  // 0
	 0.5f, -0.5f, -0.5f,  // 1
	 0.5f,  0.5f, -0.5f,  // 2
	-0.5f,  0.5f, -0.5f,  // 3
	-0.5f, -0.5f,  0.5f,  // 4
	 0.5f, -0.5f,  0.5f,  // 5
	 0.5f,  0.5f,  0.5f,  // 6
	-0.5f,  0.5f,  0.5f,  // 7
};

static unsigned int cubeIndices[] = {
	0, 1, 2, 2, 3, 0, // back
	4, 5, 6, 6, 7, 4, // front
	0, 4, 7, 7, 3, 0, // left
	1, 5, 6, 6, 2, 1, // right
	3, 7, 6, 6, 2, 3, // top
	0, 4, 5, 5, 1, 0  // bottom
};

Mesh Primitives::createCube()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//fill vertices
	int additive = 0;
	for (int i = 0; i < 8; ++i)
	{
		Vertex v{};

		for (int j = 0; j < 3; ++j)
		{
			float vert = cubeVertices[j + additive];
			if (j == 0)
			{
				v.Position.x = vert;
			}
			if (j == 1)
			{
				v.Position.y = vert;
			}
			if (j == 2)
			{
				v.Position.z = vert;
			}
		}

		v.Normal.x = 0;
		v.Normal.y = 1;
		v.Normal.z = 0;
		v.TexCoords.s = 0;
		v.TexCoords.t = 0;

		additive += 3;
		vertices.emplace_back(v);
	}

	//fill indices
	for (int i = 0; i < sizeof(cubeIndices) / sizeof(unsigned int); ++i)
	{
		indices.emplace_back(cubeIndices[i]);
	}

	return Mesh(vertices, indices, {});
}