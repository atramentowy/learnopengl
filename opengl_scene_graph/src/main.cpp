#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../deps/stb_image/stb_image.h"

#include "shader.h"
#include "camera.h"
#include "entity.h"

#ifndef ENTITY_H
#define ENTITY_H

#include <list> //std::list
#include <memory> //std::unique_ptr

class Entity : public Model
{
public:
	list<unique_ptr<Entity>> children;
	Entity* parent;
};
#endif

#include <iostream>
#include <filesystem>

void processInput(GLFWwindow *window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(char const * path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

filesystem::path getExecutablePath() {
	char path[MAX_PATH];
	GetModuleFileNameA(nullptr, path, MAX_PATH);
	return std::filesystem::path(path).parent_path();
}

int main() {
	auto exePath = getExecutablePath();
	std::filesystem::path projectRoot = exePath.parent_path().parent_path();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); // MSAA

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "opengl test", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	// glfw mouse capture
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// configure opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE); // MSAA
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// build and compile shaders
	std::filesystem::path shaderVertPath = projectRoot/"res"/"shaders"/"shader.vert";
	std::filesystem::path shaderFragPath = projectRoot/"res"/"shaders"/"shader.frag";
	Shader shader(shaderVertPath.string().c_str(), shaderFragPath.string().c_str());

	// load entities
	std::filesystem::path modelPath = projectRoot/"res"/"monkey.obj";

	Entity myEntity(modelPath.string().c_str());
	//Model model = Model(modelPath.string().c_str());
	//Entity myEntity(model);

	const float scale = 0.75;
	myEntity.transform.setLocalScale({scale, scale, scale});
	{
		Entity* lastEntity = &myEntity;
		for(unsigned int i = 0; i < 10; ++i) {
		    lastEntity->addChild(modelPath.string().c_str());
		    lastEntity = lastEntity->children.back().get();

		    // set transform values
		    lastEntity->transform.setLocalPosition({3, 0, 0});
		    lastEntity->transform.setLocalScale({scale, scale, scale});
		}
	}
	myEntity.updateSelfAndChild();

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// // draw scene graph
		Entity* lastEntity = &myEntity;
		while (lastEntity->children.size())
		{
			shader.setMat4("model", lastEntity->transform.getModelMatrix());
			//lastEntity->pModel->Draw(shader);
			lastEntity->draw(shader);
			lastEntity = lastEntity->children.back().get();
		}
		// myEntity.transform.setLocalRotation({ 0.0f, myEntity.transform.getLocalRotation().y + 20 * deltaTime, 0.f });
		myEntity.updateSelfAndChild();

		// glfw swap buffers and poll io events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	
	if(firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
