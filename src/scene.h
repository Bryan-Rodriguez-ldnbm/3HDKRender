#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <string>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class EventCallbacks
{
    // Interface to circumvent C style global callbacks
    virtual void frameBuffer_size_callback(GLFWwindow* window, int width, int height) = 0;
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
	virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
	virtual void mousePos_callback(GLFWwindow* window, double xPosIn, double yPosIn) = 0;
	virtual void process_Input(GLFWwindow* window) = 0;
};

class Scene
{
public:
    Scene();
    ~Scene();

    bool init(int const width, int const height);
    void shutdown();

    const GLFWwindow* getWindow();

protected:
    // Allows us to implement singleton OOP design
    static Scene* instance;

    std::unique_ptr<GLFWwindow> window = nullptr;
    std::unique_ptr<EventCallbacks> eventCallbacks = nullptr;

private:
    static void resizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mousePosCallback(GLFWwindow* window, double xPosIn, double yPosIn);
	static void processInput(GLFWwindow* window);
};

#endif