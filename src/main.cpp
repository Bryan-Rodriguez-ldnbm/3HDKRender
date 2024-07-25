#include <iostream>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "render/shader.h"
#include "render/model.h"
#include "utils/camera.h"
#include "utils/matrix_stack.h"

class Application : public EventCallbacks
{
public:
    void frameBuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);

		aspect_ratio = (float)width / height;
        
        SCR_WIDTH = width;
        SCR_HEIGHT = height;

        title = std::to_string(SCR_WIDTH) + "x" + std::to_string(SCR_HEIGHT) + " @ " + std::to_string(fps);

        glfwSetWindowTitle(window, title.c_str());

		if (std::isnan(aspect_ratio))
		{
			aspect_ratio = 0.00001f;
			draw = false;
		}  
        else 
            draw = true;

		if (draw) 
            render();
    }

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(getWindow(), GL_TRUE);
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
		else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		{
			camera->resetOrientation();
		}
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera->processMouseScroll((float)yoffset);
    }

    void mousePos_callback(GLFWwindow* window, double xPosIn, double yPosIn)
    {
        float xPos = (float)xPosIn;
		float yPos = (float)yPosIn;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{   
            // debug output
            // std::cout << "response";
			if (first_mouse)
			{
				lastX = xPos;
				lastY = yPos;
				first_mouse = false;
			}

			float xOffset = xPos - lastX;
			float yOffset = lastY - yPos;

			camera->processMouseMovement(xOffset, yOffset);
		}
		lastX = xPos;
		lastY = yPos;
    }

    void process_Input(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->processKeyboard(FORWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->processKeyboard(BACKWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->processKeyboard(LEFT, delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->processKeyboard(RIGHT, delta_time);
    }

    GLFWwindow* getWindow() const
    {
        return scene->getWindow();
    }

    void init() 
    {
        scene = std::make_shared<Scene>();
        scene->init(SCR_WIDTH, SCR_HEIGHT);
        scene->setEventCallbacks(this);

        camera = std::make_unique<fps_Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

        model_shader = std::make_shared<Shader>();
        model_shader->init(shader_dir + "simple_vert.glsl", shader_dir + "simple_frag.glsl");
        
        this->initGeom();
    }

    void initGeom()
    {
        bunny = std::make_unique<Model>();
        bunny->loadModel("bunny");
        bunny->init();
        bunny->measure();
    }

    void shutdown()
    {
        scene->shutdown();
        model_shader->free();
    }

    void render()
    {
        float current_frame = (float) glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        this->process_Input(this->getWindow());

        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model_shader->use();

        auto projection = std::make_unique<MatrixStack>();
        auto model = std::make_unique<MatrixStack>();

        projection->pushMatrix();
        projection->perspective(glm::radians(camera->getZoom()), aspect_ratio, 0.1f, 100.f);

        glm::mat4 view = camera->getViewMatrix();
        model_shader->setMat4("view", view);

        model_shader->use();
        model_shader->setMat4("projection", projection->topMatrix());
        projection->popMatrix();

        model->pushMatrix();
        model_shader->setMat4("model", model->topMatrix());
        
        model_shader->setVec3("mat_amb", glm::vec3(1.0f, 1.0f, 1.0f));
        bunny->draw(model_shader);

        model->popMatrix();

        glfwSwapBuffers(this->getWindow());
        glfwPollEvents();
    }

private:
    std::shared_ptr<Scene> scene = nullptr;

    bool draw = true;

    int SCR_WIDTH = 1024;
    int SCR_HEIGHT = 720;

    float aspect_ratio = (float)SCR_WIDTH / SCR_HEIGHT;

    std::string shader_dir = "./res/shaders/";
    std::string obj_dir = "./res/models/";
    std::string texture_dir = "./res/textures/";

    std::shared_ptr<Shader> model_shader = nullptr;

    std::unique_ptr<fps_Camera> camera = nullptr;

    std::unique_ptr<Model> backpack = nullptr;
    std::unique_ptr<Model> bunny = nullptr;

    bool first_mouse = true;

    float lastX = SCR_WIDTH / 2.0f, 
          lastY = SCR_HEIGHT / 2.0f;

    float delta_time = 0.0f,
          last_frame = 0.0f;

    float fps = 0.0f, update = 0.0f;

    std::string title 
        = std::to_string(SCR_WIDTH) + "x" + std::to_string(SCR_HEIGHT) + " @ " + std::to_string(fps);

};

int main()
{
    std::shared_ptr<Application> app = std::make_shared<Application>();
    
    app->init();

    while (!glfwWindowShouldClose(app->getWindow()))
    {
        app->render();
    }

    app->shutdown();

    return 0;
}