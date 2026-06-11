#include "GameObject.h"

GameObject::GameObject(const std::string& name)
	:name(name)
{

}

GameObject::GameObject(const std::string& name, Shader* shader, Model* model)
	:name(name), shader(shader), model(model)
{

}

GameObject::GameObject(const std::string& name, Shader* shader, Mesh* primitive)
	:name(name), shader(shader), primitive(primitive)
{

}


void GameObject::Draw() const
{
	if (!shader) return;

	shader->use();
	shader->setMat4("model", transform.getModelMatrix());
	shader->setVec3("lightSourceCubeColor", color);

	if (model)
	{
		shader->setBool("useTexture", true);
		model->Draw(*shader);
	}
	if (primitive)
	{
		if (primitive->textures.empty())
		{
			shader->setBool("useTexture", false);
			shader->setVec3("colorTint", color);
			shader->setFloat("shininess", shininess);
		}
		else
		{
			shader->setBool("useTexture", true);
		}

		primitive->Draw(*shader);
	}
}

glm::mat4 Transform::getModelMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = model * glm::toMat4(glm::quat(glm::radians(rotation)));
	model = glm::scale(model, scale);
	return model;
}