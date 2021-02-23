#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <spdlog\spdlog.h>
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_glfw.h>
#include <Imgui/imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Surface.h"


struct Vertex {
    struct {
        float x, y, z;
    } position;
    struct {
        float u,v;
    } tex;
};

const std::vector<Vertex> triangle = {
    {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f},
    {0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
    {0.5f, 0.5f, 0.0f, 1.0f, 1.0f },
    {-0.5f, 0.5f,0.0f, 0.0f, 1.0f}
};

const unsigned short indices[] = {
    0,1,2,
    0,2,3
};

#define VERTEX_SHADER 1
#define FRAGMENT_SHADER 2
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


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
      
    VertexArray vertexArray;
    vertexArray.Bind();

    VertexBuffer vertexBuffer;
    vertexBuffer.Bind(triangle, GL_STATIC_DRAW);

    IndexBuffer indexBuffer(indices, 6);
    
    vertexArray.AddLayout(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    vertexArray.AddLayout(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    vertexArray.EnableLayout(0);
    vertexArray.EnableLayout(1);
    
    //Shaders
    Shader vertexShader("resources/shaders/default_vertex.glsl", VERTEX_SHADER);
    Shader fragShader ("resources/shaders/default_fragment.glsl", FRAGMENT_SHADER);
  
    ShaderProgram program(vertexShader.GetId(), fragShader.GetId());
  
    vertexShader.DeleteShader();
    fragShader.DeleteShader();
  
    program.UseProgram();

    Surface texture1("resources/textures/container.jpg");

    ConstantBuffer cbuff(program.GetId(), "texture1");

    Surface texture2("resources/textures/awesomeface.png", 1);

    ConstantBuffer cbuff2(program.GetId(), "texture2");

    cbuff2.SetUniform1i(1);

    ConstantBuffer cbuff3(program.GetId(), "mixValue");

    float blend = 0.5f;

    float rotZ = 0.0f;
    float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
    float translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;
       
    Renderer renderer;

    ConstantBuffer transformation(program.GetId(), "transform");
    unsigned int transformLoc = glGetUniformLocation(program.GetId(), "transform");
  
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
     
        renderer.BeginFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Hello, world!");       
            ImGui::SliderFloat("Blend", &blend, 0.0f, 1.0f);
            ImGui::End();

            ImGui::Begin("Rotation");
            ImGui::SliderFloat("Rotate Z", &rotZ, 0.0f, 360.0f);
            ImGui::End();

            ImGui::Begin("Scaling");
            ImGui::SliderFloat("Scale X", &scaleX, -5.0f, 5.0f);
            ImGui::SliderFloat("Scale Y", &scaleY, -5.0f, 5.0f);
           // ImGui::SliderFloat("Scale Z", &scaleZ, -5.0f, 5.0f);
            ImGui::End();

            ImGui::Begin("Translation");
            ImGui::SliderFloat("Translate X", &translateX, -1.0f, 1.0f);
            ImGui::SliderFloat("Translate Y", &translateY, -1.0f, 1.0f);
         //   ImGui::SliderFloat("Translate Z", &translateZ, -5.0f, 5.0f);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       
        //create a transformation matrix each frame and bind it to the pipeline as a shader uniform
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(translateX, translateY, translateZ));
        trans = glm::scale(trans, glm::vec3(scaleX, scaleY, scaleZ));
        trans = glm::rotate(trans, glm::radians(rotZ), glm::vec3(0.0, 0.0, 1.0));
        cbuff3.SetUniform1f(blend);
        transformation.SetUniformMatrix4fv(trans);
       
        renderer.Draw(indexBuffer);
       
        /* Swap front and back buffers */
        renderer.EndFrame(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    vertexArray.Unbind();
    program.DeleteProgram();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}