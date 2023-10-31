#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/shader.h"
#include "utils/camera.h"

#include <stb/stb_image.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	// create window object
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "#", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// callback function to set viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// callback function for key input
	glfwSetKeyCallback(window, key_callback);
	// callback function for mouse input
	glfwSetCursorPosCallback(window, mouse_callback);
	// callback function for scroll input
	glfwSetScrollCallback(window, scroll_callback);

	// check GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("resources/shaders/vert/shader.vert", "resources/shaders/frag/shader.frag");

	glEnable(GL_DEPTH_TEST);
	// float cubeVertices[] = {
	// 	// positions          //texture
	//     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	//      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	//      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	//     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	//     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	//      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	//      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	//      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	//      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	//     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	// };

	// cube with color and color only
	float cubeVertices[] = {
		// positions          // color (Indian Red)
		-0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f,  0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,

		-0.5f, -0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f, -0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f, -0.5f,  0.5f,  0.804f, 0.361f, 0.361f,

		-0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f,  0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f, -0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,

		 0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f, -0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,

		-0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f, -0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f, -0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f, -0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f, -0.5f, -0.5f,  0.804f, 0.361f, 0.361f,

		-0.5f,  0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f, -0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		 0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f,  0.5f,  0.5f,  0.804f, 0.361f, 0.361f,
		-0.5f,  0.5f, -0.5f,  0.804f, 0.361f, 0.361f
	};

	float triVertices[] = {
		// positions       // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
	};

	float rectVertices[] = {
		// positions       // colors         // texture
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right 
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
	   -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // top left
	};

	unsigned int triIndices[] = {
		0, 1, 2
	};

	unsigned int rectIndices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);

	// Commented out are for rendering quads and whatnot

	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// glEnableVertexAttribArray(2);

	// texture gen
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture, and gen mipmaps
	stbi_set_flip_vertically_on_load(true);
	int width, height, nChannels;
	unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture." << std::endl;
	}
	stbi_image_free(data);

	// ourShader.use();
	// ourShader.setInt("texture1", 0);
	// ourShader.setInt("texture2", 1);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = (float) glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, texture1);
		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture2);

		ourShader.use();

		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);
		glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f);
		ourShader.setMat4("projection", projection);
		glm::mat4 view = camera.getViewMatrix();
		ourShader.setMat4("view", view);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	ourShader.free();

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int scr_width, int scr_height)
{
	glViewport(0, 0, scr_width, scr_height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
}

void processInput(GLFWwindow* window)
{
	//float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		camera.Front = glm::vec3(0.0f, 0.0f, 0.0f);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll((float)yoffset);
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = (float)xPosIn;
	float yPos = (float)yPosIn;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{	
		//std::cout << camera.Front.x << " " << camera.Front.y << " " << camera.Front.z << std::endl;
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		float xOffset = xPos - lastX;
		float yOffset = lastY - yPos;

		lastX = xPos;
		lastY = yPos;

		camera.processMouseMovement(xOffset, yOffset);
	}
}
