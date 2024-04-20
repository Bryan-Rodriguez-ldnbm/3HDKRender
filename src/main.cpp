#include <iostream>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "render/shader.h"
#include "render/model.h"

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

        point_shader = std::make_shared<Shader>();
        
        
    }

    void initGeom()
    {

    }


    void shutdown()
    {
        scene->shutdown();
        // point_shader->free();
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

    std::string shader_dir = "../res/shaders";

    std::shared_ptr<Shader> point_shader = nullptr;

    std::unique_ptr<Model> model = nullptr;
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