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
#include "Model.h"

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
    Shader shader("resources/shaders/vertex_default.glsl", "resources/shaders/frag_default.glsl");
    Shader lightShader("resources/shaders/vertex_light.glsl", "resources/shaders/frag_light.glsl");

    float lightPos[3] = { 1.0f, 1.0f, 1.0f };

    Cube light(glm::vec3(lightPos[0], lightPos[1], lightPos[2]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.25f, 0.25f, 0.25f));
    light.SetIndexBuffer();
    Renderer renderer(ENABLE_DEPTH_TEST);

    Model model("resources/models/backpack.obj");
    Surface diffuseMap("resources/models/diffuse.jpg");
    Surface specularMap("resources/models/specular.jpg",1);
    float lightAmbient[3] = { 0.2f, 0.2f, 0.2f };
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
      
        processInput(window);
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        renderer.BeginFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
             
            ImGui::Begin("Debug");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
            ImGui::SliderFloat3("Light Position",&light.GetPosition().x, -30.0f, 30.0f);

            ImGui::End();

        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
       
       
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetFieldOfView()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,projNearPlane, projFarPlane);
        shader.use();
        shader.setVec3("light.ambient", lightAmbient[0], lightAmbient[1], lightAmbient[2]);
        shader.setVec3("light.diffuse", 1.0f, 0.8f, 0.2f); // darken diffuse light a bit
        shader.setVec3("light.specular", 1.0f,1.0f,1.0f);
        shader.setVec3("light.position", light.GetPosition());
        shader.setFloat("light.constant", 1.0f);
        shader.setFloat("light.linear", 0.09f);
        shader.setFloat("light.quadratic", 0.032f);
        shader.setFloat("material.shininess", 64.0f);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.GetPosition());
        shader.setMat4("view", camera.GetLookAt());
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        shader.setMat4("model", modelMatrix);
        model.Draw(shader);

        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", camera.GetLookAt());
        lightShader.setVec3("lightColor", 1.0f,1.0f,1.0f);
        light.Bind();
        light.UpdateModelMatrix();
        lightShader.setMat4("model", light.GetModelMatrix());


        renderer.Draw(0, 36);
        /* Swap front and back buffers */
        renderer.EndFrame(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
     
  
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