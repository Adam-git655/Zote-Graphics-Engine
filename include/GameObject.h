#pragma once
#include "glm/glm.hpp"
#include <string>
#include "model.h"
#include "ShaderClass.h"

struct Transform
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 getModelMatrix() const;
};

//To add rendering of primitives using mesh class
class GameObject
{
public:
	std::string name = "GameObject";
	Transform transform;
	Shader* shader = nullptr;
	Model* model = nullptr;

	GameObject() = default;
	GameObject(const std::string& name, Shader* shader, Model* model);

	void Draw() const;
};