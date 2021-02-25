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

void processInput(GLFWwindow* window);

//not able to share vertices due to texture mapping
const std::vector<Vertex> triangle = {

    //front
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f}, //bottom left
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f}, //bottom right
    {0.5f, 0.5f, -0.5f, 1.0f, 1.0f }, //top right
    {-0.5f, 0.5f,-0.5f, 0.0f, 1.0f}, //top left

    //back
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}, //bottom left
    {0.5f, -0.5f, 0.5f, 1.0f, 0.0f}, //bottom right
    {0.5f, 0.5f, 0.5f, 1.0f, 1.0f }, //top right
    {-0.5f, 0.5f,0.5f, 0.0f, 1.0f}, //top left

    //right
    {0.5f, -0.5f, -0.5f, 0.0f, 0.0f}, //bottom left
    {0.5f, -0.5f, 0.5f, 1.0f, 0.0f}, //bottom right
    {0.5f, 0.5f, 0.5f, 1.0f, 1.0f }, //top right
    {0.5f, 0.5f, -0.5f, 0.0f, 1.0f }, //top left

    //left
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}, //bottom left
    {-0.5f, -0.5f, -0.5f, 1.0f, 0.0f}, //bottom right
    {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f}, //top right
    {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f}, //top left

    //top
    {-0.5f, 0.5f, -0.5f, 0.0f, 0.0f}, //bottom left
    {0.5f, 0.5f,  -0.5f, 1.0f, 0.0f }, //bottom right
    {0.5f, 0.5f, 0.5f, 1.0f, 1.0f }, //top right
    {-0.5f, 0.5f,0.5f, 0.0f, 1.0f}, //top left

    //bottom
     {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f}, //bottom left
    {0.5f, -0.5f, 0.5f, 1.0f, 0.0f}, //bottom right
    {0.5f, -0.5f, -0.5f, 1.0f, 1.0f}, //top right
     {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f},  //top left
};

const unsigned short indices[] = {
    //front face
    0,1,2,
    0,2,3,
    //back face
    4,5,6,
    4,6,7,
    //right face
    8,9,10,
    8,10,11,
    //left face
    12,13,14,
    12,14,15,
    //top
    16,17,18,
    16,18,19,
    //bottom
    20,21,22,
    20,22,23
};

#define VERTEX_SHADER 1
#define FRAGMENT_SHADER 2
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 1000
#define ENABLE_DEPTH_TEST true


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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

    IndexBuffer indexBuffer(indices, 36);

    vertexArray.AddLayout(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    vertexArray.AddLayout(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    vertexArray.EnableLayout(0);
    vertexArray.EnableLayout(1);

    //Shaders
    Shader vertexShader("resources/shaders/default_vertex.glsl", VERTEX_SHADER);
    Shader fragShader("resources/shaders/default_fragment.glsl", FRAGMENT_SHADER);

    ShaderProgram program(vertexShader.GetId(), fragShader.GetId());

    vertexShader.DeleteShader();
    fragShader.DeleteShader();

    program.UseProgram();

    Surface texture1("resources/textures/awesomeface.png");

    ConstantBuffer cbuff(program.GetId(), "texture1");

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    struct Model {
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
        glm::vec3 translation = { 0.0f,0.0f, 0.0f };
        std::string name;
    };

    std::vector<Model> Cubes;

    Model cube1;
    Model cube2;
    Model cube3;

    cube1.name = "cube 1";
    cube1.translation.x = cubePositions[0].x;
    cube1.translation.y = cubePositions[0].y;
    cube1.translation.z = cubePositions[0].z;

    cube2.name = "cube 2";
    cube2.translation.x = cubePositions[1].x;
    cube2.translation.y = cubePositions[1].y;
    cube2.translation.z = cubePositions[1].z;

    cube3.name = "cube 3";
    cube3.translation.x = cubePositions[2].x;
    cube3.translation.y = cubePositions[2].y;
    cube3.translation.z = cubePositions[2].z;


    Cubes.push_back(cube1);
    Cubes.push_back(cube2);
    Cubes.push_back(cube3);


    float projFOV = 45.0f, projNearPlane = 0.1f, projFarPlane = 100.0f;

    Renderer renderer(ENABLE_DEPTH_TEST);
    
    glm::mat4 identity = glm::mat4(1.0f);

    ConstantBuffer modelMatrix(program.GetId(), "model");
    ConstantBuffer viewMatrix(program.GetId(), "view");
    ConstantBuffer projectionMatrix(program.GetId(), "projection");

   

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
      
        processInput(window);
        /* Render here */
        
        renderer.BeginFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            for(auto& c : Cubes)
            {
                ImGui::Begin(c.name.c_str());
                ImGui::SliderFloat3("Rotation", &c.rotation.x, -360.0f, 360.0f);
                ImGui::SliderFloat3("Scale", &c.scale.x, -5.0f, 5.0f);
                ImGui::SliderFloat3("Translation", &c.translation.x, -5.0f, 5.0f);
               
                ImGui::End();
            }

            ImGui::Begin("Debug");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
            ImGui::SliderFloat3("Cam Pos", &cameraPos.x, -5.0f, 5.0f);
            ImGui::SliderFloat3("Cam Front", &cameraFront.x, -1.0f, 1.0f);
            ImGui::SliderFloat("FOV", &projFOV, 0.0f, 100.0f);
            ImGui::SliderFloat("Near Plane", &projNearPlane, 0.1f, 95.0f);
            ImGui::SliderFloat("Far Plane", &projFarPlane, 0.5f, 100.0f);
            ImGui::End();

        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      

        glm::mat4 projection = glm::perspective(glm::radians(projFOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,projNearPlane, projFarPlane);
        
       
        viewMatrix.SetUniformMatrix4fv(view);
        projectionMatrix.SetUniformMatrix4fv(projection);
        for (uint32_t index = 0; index < Cubes.size(); index++)
        {
            const auto& c = Cubes.at(index);
           
            glm::mat4 model = glm::translate(identity, c.translation);
            
            model = glm::rotate(model, glm::radians(c.rotation.x), glm::vec3(1.0f,0.0f,0.0f));
            model = glm::rotate(model, glm::radians(c.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(c.rotation.z), glm::vec3(0.0f, 0.0f,1.0f));
            if (index % 2 != 0)
            {
                float angle = 20.0f * index;
                angle = (float)glfwGetTime() * 25.0f;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
            }
            model = glm::scale(model, glm::vec3(c.scale.x, c.scale.y, c.scale.z));
            modelMatrix.SetUniformMatrix4fv(model);
            
            renderer.Draw(indexBuffer);
        }
        
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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}