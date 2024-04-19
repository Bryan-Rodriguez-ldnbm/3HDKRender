#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene.h"

class Application : public EventCallbacks
{
public:
    void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        
    }

    void mousePos_callback(GLFWwindow* window, double xPosIn, double yPosIn)
    {
        
    }

    void process_Input(GLFWwindow* window)
    {
        
    }

    GLFWwindow* getWindow() const
    {
        return scene->getWindow();
    }

    void init() 
    {
        scene = std::make_shared<Scene>();
        scene->init(SCR_WIDTH, SCR_HEIGHT);
    }

    void shutdown()
    {
        scene->shutdown();
    }

    void render()
    {
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(this->getWindow());
        glfwPollEvents();
    }

        
private:
    std::shared_ptr<Scene> scene = nullptr;

    bool draw = true;

    int SCR_WIDTH = 1024;
    int SCR_HEIGHT = 720;

};

int main()
{
    std::unique_ptr<Application> app = std::make_unique<Application>();

    app->init();

    while (!glfwWindowShouldClose(app->getWindow()))
    {
        app->render();
    }

    app->shutdown();

    return 0;
}