#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <spdlog\spdlog.h>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"
#include <fstream>
#include <string>
#include <sstream>

const float triangleOne[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f, 0.5f, 0.0f
 
};

const float triangleTwo[] = {
    0.1f, -0.9f, 0.0f,
    0.9f, -0.9f, 0.0f,
    0.5f, 0.5f, 0.0f
};


const unsigned short indices[] = {
    0, 1, 2
};


#define VERTEX_SHADER 1
#define FRAGMENT_SHADER 2

std::string source;


static void ParseShader(const std::string& filepath)
{
    std::ifstream input_file(filepath);
    if (!input_file.is_open()) 
    {
        spdlog::error("ERROR::PARSE_SHADER::OPEN_FILE");
    }

    std::string line;
    std::stringstream ss;
    while (getline(input_file, line))
    {
        ss << line << '\n';
    }
    
    source = ss.str();
}

uint32_t CreateShader(unsigned short type, const std::string& filepath)
{
    int success;
    char infoLog[512];
    ParseShader(filepath);
    const char* shaderSource = source.c_str();

    switch (type)
    {
    case VERTEX_SHADER:
      
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &shaderSource, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            spdlog::critical("ERROR::SHADER::VERTEX::COMPILATION_FAILED: {}", infoLog);
        }
        return vertexShader;
    case FRAGMENT_SHADER:
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &shaderSource, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            spdlog::critical("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: {}", infoLog);
        }
        return fragmentShader;
    default:
        spdlog::error("ERROR::CREATESHADER::INVALID_SHADER_TYPE_PROVIDED");
        return 0;
    }
}

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
      
    unsigned int vbos[2], vaos[2], indexBuffer;

    //vertex buffer and vertex array object
    glGenVertexArrays(2, vaos);
    glGenBuffers(2, vbos);

    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);
    
    //bind vertex buffer to vao
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //no need to unbind and binding to another straight away
    glBindVertexArray(vaos[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Shaders
    uint32_t vertShader = CreateShader(VERTEX_SHADER, "resources/shaders/default_vertex.glsl");
    uint32_t fragShader = CreateShader(FRAGMENT_SHADER, "resources/shaders/default_fragment.glsl");
    uint32_t blueFragShader = CreateShader(FRAGMENT_SHADER, "resources/shaders/blue_fragment.glsl");

    unsigned int shaderProgram, shaderProgramTwo;
    shaderProgram = glCreateProgram();
 
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER_PROGRAM: {}", infoLog);
    }

    shaderProgramTwo = glCreateProgram();
    glAttachShader(shaderProgramTwo, vertShader);
    glAttachShader(shaderProgramTwo, blueFragShader);
    glLinkProgram(shaderProgramTwo);

    glGetProgramiv(shaderProgramTwo, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramTwo, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER_PROGRAMTWO: {}", infoLog);
    }
   
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    glDeleteShader(blueFragShader);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        float time = glfwGetTime();
        float green = (sin(time) / 2) + 0.5f;
        int vertexColourLocation = glGetUniformLocation(shaderProgram, "myColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColourLocation, 0.0f, green, 0.0f, 1.0f);
        glBindVertexArray(vaos[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
       
       
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}