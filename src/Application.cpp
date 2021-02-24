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
    Shader fragShader ("resources/shaders/default_fragment.glsl", FRAGMENT_SHADER);
  
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
        float rotZ = 0.0f, rotX = 0.0f, rotY = 0.0f, scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f,
            translateX = 0.0f, translateY = 0.0f, translateZ = 0.0f;
        std::string name;
    };

    std::vector<Model> Cubes;

    Model cube1;
    Model cube2;
    Model cube3;
    Model cube4;
    Model cube5;
    Model cube6;
    Model cube7;
    Model cube8;
    Model cube9;
    Model cube10;
    
    cube1.name = "cube 1";
    cube1.translateX = cubePositions[0].x;
    cube1.translateY = cubePositions[0].y;
    cube1.translateZ = cubePositions[0].z;

    cube2.name = "cube 2";
    cube2.translateX = cubePositions[1].x;
    cube2.translateY = cubePositions[1].y;
    cube2.translateZ = cubePositions[1].z;

    cube3.name = "cube 3";
    cube3.translateX = cubePositions[2].x;
    cube3.translateY = cubePositions[2].y;
    cube3.translateZ = cubePositions[2].z;

    cube4.name = "cube 4";
    cube4.translateX = cubePositions[3].x;
    cube4.translateY = cubePositions[3].y;
    cube4.translateZ = cubePositions[3].z;

    cube5.name = "cube 5";
    cube5.translateX = cubePositions[4].x;
    cube5.translateY = cubePositions[4].y;
    cube5.translateZ = cubePositions[4].z;

    cube6.name = "cube 6";
    cube6.translateX = cubePositions[5].x;
    cube6.translateY = cubePositions[5].y;
    cube6.translateZ = cubePositions[5].z;
    
    cube7.name = "cube 7";
    cube7.translateX = cubePositions[6].x;
    cube7.translateY = cubePositions[6].y;
    cube7.translateZ = cubePositions[6].z;

    cube8.name = "cube 8";
    cube8.translateX = cubePositions[7].x;
    cube8.translateY = cubePositions[7].y;
    cube8.translateZ = cubePositions[7].z;

    cube9.name = "cube 9";
    cube9.translateX = cubePositions[8].x;
    cube9.translateY = cubePositions[8].y;
    cube9.translateZ = cubePositions[8].z;

    cube10.name = "cube 10";
    cube10.translateX = cubePositions[9].x;
    cube10.translateY = cubePositions[9].y;
    cube10.translateZ = cubePositions[9].z;

    Cubes.push_back(cube1);
    Cubes.push_back(cube2);
    Cubes.push_back(cube3);
    Cubes.push_back(cube4);
    Cubes.push_back(cube5);
    Cubes.push_back(cube6);
    Cubes.push_back(cube7);
    Cubes.push_back(cube8);
    Cubes.push_back(cube9);
    Cubes.push_back(cube10);
    
    float projFOV = 45.0f, projNearPlane = 0.1f, projFarPlane = 100.0f;
    float viewRotZ = 0.0f, viewRotX = 0.0f, viewRotY = 0.0f, viewScale = 1.0f, viewTranslateX = 0.0f, viewTranslateY = 0.0f, viewTranslateZ = -3.0f;
       
    Renderer renderer(ENABLE_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 identity = glm::mat4(1.0f);

    ConstantBuffer modelMatrix(program.GetId(), "model");
    ConstantBuffer viewMatrix(program.GetId(), "view");
    ConstantBuffer projectionMatrix(program.GetId(), "projection");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
     
        renderer.BeginFrame();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            for(auto& c : Cubes)
            {
                ImGui::Begin(c.name.c_str());
                ImGui::SliderFloat("Rotate X", &c.rotX, -360.0f, 360.0f);
                ImGui::SliderFloat("Rotate Y", &c.rotY, -360.0f, 360.0f);
                ImGui::SliderFloat("Rotate Z", &c.rotZ, -360.0f, 360.0f);
                ImGui::SliderFloat("Scale X", &c.scaleX, -5.0f, 5.0f);
                ImGui::SliderFloat("Scale Y", &c.scaleY, -5.0f, 5.0f);
                ImGui::SliderFloat("Scale Z", &c.scaleZ, -5.0f, 5.0f);
                ImGui::SliderFloat("Translate X", &c.translateX, -1.0f, 1.0f);
                ImGui::SliderFloat("Translate Y", &c.translateY, -1.0f, 1.0f);
                ImGui::SliderFloat("Translate Z", &c.translateZ, -5.0f, 5.0f);
                ImGui::End();
            }

            ImGui::Begin("View");
            ImGui::SliderFloat("Rotate X", &viewRotX, -360.0f, 360.0f);
            ImGui::SliderFloat("Rotate Y", &viewRotY, -360.0f, 360.0f);
            ImGui::SliderFloat("Rotate Z", &viewRotZ, -360.0f, 360.0f);
            ImGui::SliderFloat("Scale ", &viewScale, 0.0f, 10.0f);
            
            ImGui::SliderFloat("Translate X", &viewTranslateX, -1.0f, 1.0f);
            ImGui::SliderFloat("Translate Y", &viewTranslateY, -1.0f, 1.0f);
            ImGui::SliderFloat("Translate Z", &viewTranslateZ, -50.0f, 50.0f);
            ImGui::End();

            ImGui::Begin("Projection");
            ImGui::SliderFloat("FOV", &projFOV, 0.0f, 100.0f);
            ImGui::SliderFloat("Near Plane", &projNearPlane, 0.1f, 95.0f);
            ImGui::SliderFloat("Far Plane", &projFarPlane, 0.5f, 100.0f);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
       
      
        glm::mat4 view = glm::translate(identity, glm::vec3(viewTranslateX, viewTranslateY, viewTranslateZ));
        view = glm::rotate(view, glm::radians(viewRotX), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(viewRotY), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::rotate(view, glm::radians(viewRotZ), glm::vec3(0.0f, 0.0f, 1.0f));
        view = glm::scale(view, glm::vec3(viewScale, viewScale, viewScale));

        glm::mat4 projection = glm::perspective(glm::radians(projFOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,projNearPlane, projFarPlane);
        
       
        viewMatrix.SetUniformMatrix4fv(view);
        projectionMatrix.SetUniformMatrix4fv(projection);
        for (uint32_t index = 0; index < Cubes.size(); index++)
        {
            const auto& c = Cubes.at(index);
           
            glm::mat4 model = glm::translate(identity, glm::vec3(c.translateX, c.translateY, c.translateZ));
            model = glm::rotate(model, glm::radians(c.rotX), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(c.rotY), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(c.rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
            if (index % 2 != 0)
            {
                float angle = 20.0f * index;
                angle = (float)glfwGetTime() * 25.0f;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            }
            model = glm::scale(model, glm::vec3(c.scaleX, c.scaleY, c.scaleZ));
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