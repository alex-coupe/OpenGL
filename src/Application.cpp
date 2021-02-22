#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <spdlog\spdlog.h>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"
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
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
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
    //Surface texture2()

    Renderer renderer;

    ConstantBuffer cbuff(program.GetId(), "u_Texture");

    cbuff.SetUniform1i(0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
     
        renderer.BeginFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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