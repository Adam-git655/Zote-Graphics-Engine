#include "Primitives.h"

static float unlitCubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  
	 0.5f, -0.5f, -0.5f,  
	 0.5f,  0.5f, -0.5f,  
	-0.5f,  0.5f, -0.5f, 
	-0.5f, -0.5f,  0.5f,  
	 0.5f, -0.5f,  0.5f,  
	 0.5f,  0.5f,  0.5f,  
	-0.5f,  0.5f,  0.5f,  
};

static unsigned int unlitCubeIndices[] = {
	0, 1, 2, 2, 3, 0, // back
	4, 5, 6, 6, 7, 4, // front
	0, 4, 7, 7, 3, 0, // left
	1, 5, 6, 6, 2, 1, // right
	3, 7, 6, 6, 2, 3, // top
	0, 4, 5, 5, 1, 0  // bottom
};

static float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,   0,0,-1,   0,0,  
	 0.5f, -0.5f, -0.5f,   0,0,-1,   1,0,  
	 0.5f,  0.5f, -0.5f,   0,0,-1,   1,1,  
	-0.5f,  0.5f, -0.5f,   0,0,-1,   0,1,  

	-0.5f, -0.5f,  0.5f,   0,0,1,    0,0,  
	 0.5f, -0.5f,  0.5f,   0,0,1,    1,0,  
	 0.5f,  0.5f,  0.5f,   0,0,1,    1,1,  
	-0.5f,  0.5f,  0.5f,   0,0,1,    0,1,  
	
	-0.5f, -0.5f, -0.5f,   -1,0,0,   0,0,  
	-0.5f,  0.5f, -0.5f,   -1,0,0,   1,0,  
	-0.5f,  0.5f,  0.5f,   -1,0,0,   1,1,  
	-0.5f, -0.5f,  0.5f,   -1,0,0,   0,1,  
	
	 0.5f, -0.5f, -0.5f,   1,0,0,    0,0,  
	 0.5f,  0.5f, -0.5f,   1,0,0,    1,0,  
	 0.5f,  0.5f,  0.5f,   1,0,0,    1,1,  
	 0.5f, -0.5f,  0.5f,   1,0,0,    0,1,  

	 -0.5f, -0.5f, -0.5f,   0,-1,0,   0,0, 
	  0.5f, -0.5f, -0.5f,   0,-1,0,   1,0, 
	  0.5f, -0.5f,  0.5f,   0,-1,0,   1,1, 
	 -0.5f, -0.5f,  0.5f,   0,-1,0,   0,1, 

	 -0.5f,  0.5f, -0.5f,   0,1,0,    0,0, 
	  0.5f,  0.5f, -0.5f,   0,1,0,    1,0, 
	  0.5f,  0.5f,  0.5f,   0,1,0,    1,1, 
	 -0.5f,  0.5f,  0.5f,   0,1,0,    0,1, 
};

static unsigned int cubeIndices[] = {
	0,2,1,   0,3,2,   // back
	4,5,6,   4,6,7,   // front
	8,10,9,  8,11,10, // left
	12,13,14, 12,14,15, // right
	16,17,18, 16,18,19, // bottom
	20,22,21, 20,23,22  // top
};

static float quadVertices[] = {
	-0.5f, 0.0f, -0.5f,   0,1,0,   0,0,  
	 0.5f, 0.0f, -0.5f,   0,1,0,   1,0,  
	 0.5f, 0.0f,  0.5f,   0,1,0,   1,1,  
	-0.5f, 0.0f,  0.5f,   0,1,0,   0,1,  
};

static unsigned int quadIndices[] = {
	0,2,1,
	0,3,2
};

float screenQuadVertices[] = {
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,  
	 1.0f, -1.0f,  1.0f, 0.0f, 
	 1.0f,  1.0f,  1.0f, 1.0f  
};

unsigned int screenQuadIndices[] = {
	0, 1, 2,
	0, 2, 3
};

Mesh Primitives::createCubeUnlit()
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	//fill vertices
	int additive = 0;
	for (int i = 0; i < 8; ++i)
	{
		Vertex v{};

		v.Position = { unlitCubeVertices[additive], unlitCubeVertices[additive + 1], unlitCubeVertices[additive + 2] };
		v.Normal = { 0, 1, 0 };
		v.TexCoords = { 0, 0 };

		additive += 3;
		vertices.emplace_back(v);
	}

	//fill indices
	for (int i = 0; i < sizeof(unlitCubeIndices) / sizeof(unsigned int); ++i)
	{
		indices.emplace_back(unlitCubeIndices[i]);
	}

	return Mesh(vertices, indices, {});
}

Mesh Primitives::createCube(std::string texturePath)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//fill vertices
	int additive = 0;
	for (int i = 0; i < 24; ++i)
	{
		Vertex v{};

		v.Position = { cubeVertices[additive], cubeVertices[additive + 1], cubeVertices[additive + 2] };
		v.Normal = { cubeVertices[additive + 3], cubeVertices[additive + 4], cubeVertices[additive + 5] };
		v.TexCoords = { cubeVertices[additive + 6], cubeVertices[additive + 7] };

		additive += 8;
		vertices.emplace_back(v);
	}

	//fill indices
	for (int i = 0; i < sizeof(cubeIndices) / sizeof(unsigned int); ++i)
	{
		indices.emplace_back(cubeIndices[i]);
	}

	//add texture (if applicable)
	if (texturePath != "")
	{
		Texture tex;
		std::string directory = texturePath.substr(0, texturePath.find_last_of("/"));
		std::string fileName = texturePath.substr(texturePath.find_last_of("/") + 1);
		tex.id = TextureLoader::TextureFromFile(fileName.c_str(), directory);
		tex.type = "texture_diffuse";
		tex.path = texturePath;
		textures.emplace_back(tex);
	}

	return Mesh(vertices, indices, textures);
}

Mesh Primitives::createQuad(std::string texturePath)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//fill vertices
	int additive = 0;
	for (int i = 0; i < 4; ++i)
	{
		Vertex v{};

		v.Position = { quadVertices[additive], quadVertices[additive + 1], quadVertices[additive + 2] };
		v.Normal = { quadVertices[additive + 3], quadVertices[additive + 4], quadVertices[additive + 5] };
		v.TexCoords = { quadVertices[additive + 6], quadVertices[additive + 7] };

		additive += 8;
		vertices.emplace_back(v);
	}

	//fill indices
	for (int i = 0; i < sizeof(quadIndices) / sizeof(unsigned int); ++i)
	{
		indices.emplace_back(quadIndices[i]);
	}

	//add texture (if applicable)
	if (texturePath != "")
	{
		Texture tex;
		std::string directory = texturePath.substr(0, texturePath.find_last_of("/"));
		std::string fileName = texturePath.substr(texturePath.find_last_of("/") + 1);
		tex.id = TextureLoader::TextureFromFile(fileName.c_str(), directory);
		tex.type = "texture_diffuse";
		tex.path = texturePath;
		textures.emplace_back(tex);
	}
	
	return Mesh(vertices, indices, textures);
}

unsigned int Primitives::createScreenQuadVAO()
{
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), screenQuadVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(screenQuadIndices), screenQuadIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	return VAO;
}