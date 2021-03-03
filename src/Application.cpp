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
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
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
    Shader lightingShader("resources/shaders/vertex_default.glsl", "resources/shaders/frag_default.glsl");
    Shader lightCubeShader("resources/shaders/vertex_light.glsl", "resources/shaders/frag_light.glsl");
  
  
    std::unique_ptr<Cube> cube =  ShapeFactory::Make<Cube>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    cube->SetIndexBuffer();

    std::unique_ptr<Cube> light = ShapeFactory::Make<Cube>(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f));
    light->SetIndexBuffer();
      
    Surface diffuseMap("resources/textures/container2.png");
    Surface specularMap("resources/textures/container2_specular.png", 1);
    Surface emissionsMap("resources/textures/matrix.jpg", 2);

    Renderer renderer(ENABLE_DEPTH_TEST);

    float lightCol[3] = { 0.5f, 0.5f, 0.5f };

    float objectColour[3] = { 1.0f, 0.5f, 0.31f };
   
    float specular[3] = { 0.5f, 0.5f, 0.5f };
    float lightAmbient[3] = { 0.2f, 0.2f, 0.2f };
    float lightSpecular[3] = { 1.0f, 1.0f, 1.0f };
    float shine = 64.0f;
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
            
          
            ImGui::Begin("Cube");
            ImGui::SliderFloat3("Rotation", &cube->GetRotation().x, -360.0f, 360.0f);
            ImGui::SliderFloat3("Scale", &cube->GetScale().x, -5.0f, 5.0f);
            ImGui::SliderFloat3("Translation", &cube->GetPosition().x, -5.0f, 5.0f);
            ImGui::ColorPicker3("Color", objectColour);
            ImGui::End();

            ImGui::Begin("Light");
            ImGui::SliderFloat3("Translation", &light->GetPosition().x, -5.0f, 5.0f);
            ImGui::SliderFloat3("Ambient", lightAmbient, 0.0f, 1.0f);
            ImGui::SliderFloat3("Specular", lightSpecular, 0.0f, 1.0f);
            ImGui::ColorPicker3("Color", lightCol);
            ImGui::End();

            ImGui::Begin("Material");
            ImGui::SliderFloat("Shine", &shine, 0.0f, 512.0f);
            ImGui::End();
            
         
            ImGui::Begin("Debug");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
            ImGui::End();

        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        
        glm::mat4 view = camera.GetLookAt();
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFieldOfView()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,projNearPlane, projFarPlane);
                    
        lightingShader.use();
        lightingShader.setVec3("objectColor",objectColour[0], objectColour[1], objectColour[2]);
        lightingShader.setVec3("lightColor", lightCol[0], lightCol[1], lightCol[2]);
        lightingShader.setVec3("light.ambient", lightAmbient[0], lightAmbient[1], lightAmbient[2]);
        lightingShader.setVec3("light.diffuse",lightCol[0], lightCol[1], lightCol[2]); // darken diffuse light a bit
        lightingShader.setVec3("light.specular", lightSpecular[0], lightSpecular[1], lightSpecular[2]);
        lightingShader.setFloat("material.shininess", shine);
        lightingShader.setVec3("light.position", light->GetPosition());
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setFloat("time", glfwGetTime());
        lightingShader.setInt("material.specular", 1);
        lightingShader.setInt("material.emission", 2);
        lightingShader.setVec3("viewPos", camera.GetPosition());
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        cube->Bind();
        cube->UpdateModelMatrix();

        lightingShader.setMat4("model", cube->GetModelMatrix());
       
        renderer.Draw(0,36);
       
     

        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setVec3("lightColor", lightCol[0], lightCol[1], lightCol[2]);
        light->Bind();
        light->UpdateModelMatrix();
        lightCubeShader.setMat4("model", light->GetModelMatrix() );
      
        
        renderer.Draw(0,36);


        /* Swap front and back buffers */
        renderer.EndFrame(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    cube->CleanUp();
    light->CleanUp();
 
  
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