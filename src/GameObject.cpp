#include "GameObject.h"

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

	if (model)
		model->Draw(*shader);
	if (primitive)
		primitive->Draw(*shader);
}

glm::mat4 Transform::getModelMatrix() const
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation.x), { 1, 0, 0 });
	model = glm::rotate(model, glm::radians(rotation.y), { 0, 1, 0 });
	model = glm::rotate(model, glm::radians(rotation.z), { 0, 0, 1 });
	model = glm::scale(model, scale);
	return model;
}