#ifndef ENTITY_H
#define ENTITY_H

#include "../deps/glm/glm/glm.hpp" // glm::mat4
#include <list>
#include <array>
#include <memory> // std::unique_ptr
#include "model.h"

class Transform {
protected:
	// local space information
	glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_eulerRot = {0.0f, 0.0f, 0.0f};
	glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};

	// global space information concatenate in matrix
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	// dirty flag
	bool m_isDirty = true;

	glm::mat4 getLocalModelMatrix() {
		const glm::mat4 transformX = glm::rotate (
			glm::mat4(1.0f), 
			glm::radians(m_eulerRot.x), 
			glm::vec3(1.0f, 0.0f, 0.0f)
		);
		const glm::mat4 transformY = glm::rotate (
			glm::mat4(1.0f), 
			glm::radians(m_eulerRot.y), 
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		const glm::mat4 transformZ = glm::rotate(
			glm::mat4(1.0f), 
			glm::radians(m_eulerRot.z), 
			glm::vec3(0.0f, 0.0f, 1.0f)
		);

		// X*Y*Z
		const glm::mat4 rotationMatrix = transformX * transformY * transformZ;

		// translation * rotation * scale (TRS matrix)
		return glm::translate(glm::mat4(1.0f), m_position) * rotationMatrix * glm::scale(glm::mat4(1.0f), m_scale);
	}
public:
	void computeModelMatrix() {
		m_modelMatrix = getLocalModelMatrix();
		m_isDirty = false;
	}
	void computeModelMatrix(const glm::mat4& parentGlobalModelMatrix) {
		m_modelMatrix = parentGlobalModelMatrix * getLocalModelMatrix();
		m_isDirty = false;
	}

	void setLocalPosition(const glm::vec3& newPosition) {
		m_position = newPosition;
		m_isDirty = true;
	}
	void setLocalRotation(const glm::vec3& newRotation) {
		m_eulerRot = newRotation;
		m_isDirty = true;
	}
	void setLocalScale(const glm::vec3& newScale) {
		m_scale = newScale;
		m_isDirty = true;
	}

	const glm::vec3& getGlobalPosition() const {
		return m_modelMatrix[3];
	}
	const glm::vec3& getLocalPosition() const {
		return m_position;
	}
	const glm::vec3& getLocalRotation() const {
		return m_eulerRot;
	}
	const glm::vec3& getLocalScale() const {
		return m_scale;
	}
	const glm::mat4& getModelMatrix() const {
		return m_modelMatrix;
	}

	glm::vec3 getRight() const {
		return m_modelMatrix[0];
	}
	glm::vec3 getUp() const {
		return m_modelMatrix[1];
	}
	glm::vec3 getBackward() const {
		return m_modelMatrix[2];
	}
	glm::vec3 getForward() const {
		return -m_modelMatrix[2];
	}
	glm::vec3 getGlobalScale() const {
		return {glm::length(getRight()), glm::length(getUp()), glm::length(getBackward())};
	}

	bool isDirty() const {
		return m_isDirty;
	}
};

class Entity : public Model {
public:
	Transform transform;

	// scene graph
	std::list<std::unique_ptr<Entity>> children;

	Entity* parent = nullptr;

	Model* pModel = nullptr;

	// Entity(Model& model) : pModel{ &model }
	// {}

	// constructor, expects a filepath to a 3D model.
	Entity(string const& path, bool gamma = false) : Model(path, gamma)
	{}

	// add child
	// void addChild() {
	// 	children.emplace_back(std::make_unique<Entity>());
	// 	children.back()->parent = this;
	// }

	template<typename... TArgs>
    void addChild(const TArgs&... args)
    {
        children.emplace_back(std::make_unique<Entity>(args...));
        children.back()->parent = this;
    }

	// update transform if it was changed
	void updateSelfAndChild() {
		if (transform.isDirty()) {
			forceUpdateSelfAndChild();
			return;
		}

		for (auto&& child : children) {
			child->updateSelfAndChild();
		}
	}

	// force update of transform even if local space don't change
	void forceUpdateSelfAndChild() {
		if (parent) {
			transform.computeModelMatrix(parent->transform.getModelMatrix());
		} else {
			transform.computeModelMatrix();
		}

		for (auto&& child : children) {
			child->forceUpdateSelfAndChild();
		}
	}

	void draw(Shader shader) {
		//pModel.Draw(shader);
		for (auto& child : children) {
        	child->Draw(shader);
    	}
	}
};
#endif