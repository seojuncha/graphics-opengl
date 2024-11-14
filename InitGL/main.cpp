#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char* vertexShaderSource =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 460 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

unsigned int vertexShader = 0;
unsigned int fragmentShader = 0;
unsigned int shaderProgram = 0;

unsigned int vbo = 0;
unsigned int vao = 0;

bool ErrorInCompileShader(GLuint shader)
{
    int success;
    char log[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success) return true;

    glGetShaderInfoLog(shader, 512, NULL, log);
    std::cout << "error shader compile: " << log << std::endl;

    return false;
}

void CompileVertextShader()
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    ErrorInCompileShader(vertexShader);
}

void CompileFragmentShader()
{
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    ErrorInCompileShader(fragmentShader);
}

void CreateShaderProgram()
{
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

void VertexLinking()
{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void CreateVBO()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void CreateVAO()
{
    glGenVertexArrays(1, &vao);
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Initialize()
{
    CompileVertextShader();
    CompileFragmentShader();
    CreateShaderProgram();
}

void Cleanup()
{
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Normalize
GLfloat N_RGBA(float value)
{
    return (value / 255.0f);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "opengl test", NULL, NULL);
    if (!window) {
        std::cout << "failed to create window";
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return 1;
    }

    glViewport(0, 0, 600, 600);
    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
    
    while (!glfwWindowShouldClose(window)) {
        glClearColor(N_RGBA(79.0f), N_RGBA(58.0f), N_RGBA(69.0f), 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Drawing...

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}