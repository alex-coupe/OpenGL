#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>

#include "Shader.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ConstantBuffer.h"
#include "ShapeFactory.h"
#include "Cube.h"
#include "Renderer.h"
#include "Surface.h"
#include "Camera.h"


void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#define VERTEX_SHADER 1
#define FRAGMENT_SHADER 2
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1000
#define ENABLE_DEPTH_TEST 0x000001

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float projNearPlane = 0.1f, projFarPlane = 100.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2);

int main(void)
{
    GLFWwindow* window;

    const char* glsl_version = "#version 330";

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    GLenum err = glewInit();

    if (GLEW_OK != err)
    {
        spdlog::error("Unable to init Glew!");
    }

    spdlog::info("GLEW INNIT: {}", glewGetString(GLEW_VERSION));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    //Shaders
    Shader vertexShader("resources/shaders/vertex_default.glsl", VERTEX_SHADER);
    Shader textureFrag("resources/shaders/frag_texture.glsl", FRAGMENT_SHADER);
    Shader colorFrag("resources/shaders/frag_color.glsl", FRAGMENT_SHADER);
    ShaderProgram textureProgram(vertexShader.GetId(), textureFrag.GetId());
    ShaderProgram colorProgram(vertexShader.GetId(), colorFrag.GetId());
    textureProgram.UseProgram();

    vertexShader.DeleteShader();
    textureFrag.DeleteShader();
    colorFrag.DeleteShader();

    std::vector<std::unique_ptr<Cube>> cubes;
    cubes.emplace_back(ShapeFactory::Make<Cube>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    cubes.emplace_back(ShapeFactory::Make<Cube>(glm::vec3(4.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(3.0f, 3.0f, 3.0f)));
        

    cubes.front()->SetIndexBuffer();
    auto cubeIndexBuffer = cubes.front()->GetIndexBuffer();
   
    Surface texture1("resources/textures/wall.jpg");

    ConstantBuffer cbuff(textureProgram.GetId(), "texture1");
    ConstantBuffer modelMatrix(textureProgram.GetId(), "model");
    ConstantBuffer viewMatrix(textureProgram.GetId(), "view");
    ConstantBuffer projectionMatrix(textureProgram.GetId(), "projection");

    Renderer renderer(ENABLE_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
      
        processInput(window);
        /* Render here */
        
        renderer.BeginFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            
            for (uint32_t index = 0; index < cubes.size(); index++)
            {
                const auto& c = cubes.at(index);
                std::string name = "Cube ";
                name += std::to_string(index);
                ImGui::Begin(name.c_str());
                ImGui::SliderFloat3("Rotation", &c->GetRotation().x, -360.0f, 360.0f);
                ImGui::SliderFloat3("Scale", &c->GetScale().x, -5.0f, 5.0f);
                ImGui::SliderFloat3("Translation", &c->GetPosition().x, -5.0f, 5.0f);
                ImGui::End();
              
            }

            ImGui::Begin("Debug");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
            ImGui::End();

        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        
        glm::mat4 view = camera.GetLookAt();
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFieldOfView()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,projNearPlane, projFarPlane);
             
     
        for (uint32_t index = 0; index < cubes.size(); index++)
        {
            if (index > 0)
            {
                colorProgram.UseProgram();
                cbuff.SetProgram(colorProgram.GetId());
                modelMatrix.SetProgram(colorProgram.GetId());
                viewMatrix.SetProgram(colorProgram.GetId());
                projectionMatrix.SetProgram(colorProgram.GetId());

                viewMatrix.SetUniformMatrix4fv(view);
                projectionMatrix.SetUniformMatrix4fv(projection);
          }
            else
            {
                textureProgram.UseProgram();
                cbuff.SetProgram(textureProgram.GetId());
                modelMatrix.SetProgram(textureProgram.GetId());
                viewMatrix.SetProgram(textureProgram.GetId());
                projectionMatrix.SetProgram(textureProgram.GetId());

                viewMatrix.SetUniformMatrix4fv(view);
                projectionMatrix.SetUniformMatrix4fv(projection);
            }

            const auto& c = cubes.at(index);

            c->UpdateModelMatrix();
            modelMatrix.SetUniformMatrix4fv(c->GetModelMatrix());
            renderer.Draw(cubeIndexBuffer);
        }


        /* Swap front and back buffers */
        renderer.EndFrame(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    for (uint32_t index = 0; index < cubes.size(); index++)
    {
        const auto& c = cubes.at(index);

        c->CleanUp();
    }
    
    textureProgram.DeleteProgram();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.UpdateCameraInput(window, deltaTime);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.UpdateFieldOfView(window, xoffset, yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.UpdateCameraRotation(window, xpos, ypos);
}