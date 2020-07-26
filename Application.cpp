#include "Application.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <stdio.h>
#include <map>
#include <string.h>
#include <vector>

#define FILE_DELIMITER "!!@@BOOYAH@@!!"

#define FS_ORANGE_TRIANGLE 0
#define FS_YELLOW_TRIANGLE 1

#define VS_DRAW_VEC3_ARRAY 0

static void viewPortResize(GLFWwindow* win, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
 * Allocates adequate memory for inBuffer and dumps file at fileName into it.
 * Callers responsibility to free inBuffer
 */
static int dumpFilesIntoStrings(long type, std::vector<std::pair<long, std::string>>& in)
{
    std::ifstream shaderFile;
    std::stringstream inStream;
    std::string buffer;
    char cBuffer[1028];
    std::ofstream aFile;


    if (type == GL_VERTEX_SHADER)
    {
        shaderFile.open("TriangleVS.glsl");
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderFile.open("TriangleFS.glsl");
    }

    if (shaderFile.is_open())
    {
        while (getline(shaderFile, buffer))
        {
            if (buffer.compare(FILE_DELIMITER) == 0)
            {
                in.push_back({type, inStream.str()});
                inStream.str(std::string());
            }
            else
            {
                inStream << buffer << "\r\n";
            }
        }
    }
    else
    {
        aFile.open("C:\\Users\\Jacob\\source\\repos\\tetrs3\\aFile.txt", std::ios::out | std::ios::app);
        snprintf(cBuffer, sizeof(cBuffer), "%X didn't open\n", type);
        aFile.write(cBuffer, strlen(cBuffer));
        aFile.close();
        return 0;
    }
    in.push_back({type, inStream.str()});

    if (type == GL_VERTEX_SHADER)
    {
        if (in.size() < 1)
        {
            return 0;
        }
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        if (in.size() < 2)
        {
            return 0;
        }
    }
    return 1;
}

static int prepareShaders(std::vector<unsigned int>& shaderIds, const std::vector<std::pair<long, std::string>> source) 
{
    int success;
    char infoLog[512];
    char buffer[1028];
    char* bPtr = buffer;
    std::ofstream aFile;
    std::vector<std::pair<long, std::string>>::const_iterator vectIter;
    unsigned int shaderId;
    static const std::map<long, std::string> typeMap = { {GL_VERTEX_SHADER, "VERTEX"},{GL_FRAGMENT_SHADER, "FRAGMENT"} };

    for(vectIter = source.begin(); vectIter != source.end(); vectIter++)
    {
        shaderId = glCreateShader(vectIter->first);

        snprintf(buffer, sizeof(buffer), "%s", vectIter->second.c_str());

        glShaderSource(shaderId, 1, (const GLchar** const)(&bPtr), NULL);

        glCompileShader(shaderId);
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            aFile.open("C:\\Users\\Jacob\\source\\repos\\tetrs3\\aFile.txt", std::ios::out | std::ios::app);
            snprintf(buffer, sizeof(buffer), "%s failed to compile:\n%s\n", typeMap.at(vectIter->first).c_str(), infoLog);
            aFile.write(buffer, strlen(buffer));
            aFile.close();
            return 0;
        }
        shaderIds.push_back(shaderId);
    }

    return 1;
}

static int createProgram(unsigned int& programId, const std::vector<unsigned int> shaders)
{
    int success;
    char infoLog[512];
    char buffer[1028];
    std::ofstream aFile;
    std::vector<unsigned int>::const_iterator vectIter;

    programId = glCreateProgram();

    for (vectIter = shaders.begin(); vectIter != shaders.end(); vectIter++)
    {
        glAttachShader(programId, *vectIter);
    }
    glLinkProgram(programId);

    for (vectIter = shaders.begin(); vectIter != shaders.end(); vectIter++)
    {
        glDeleteShader(*vectIter);
    }

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        aFile.open("C:\\Users\\Jacob\\source\\repos\\tetrs3\\aFile.txt", std::ios::out | std::ios::app);
        snprintf(buffer, sizeof(buffer), "failed to link:\n%s\n", infoLog);
        aFile.write(buffer, strlen(buffer));
        aFile.close();
        return 0;
    }

    return 1;
}

static int save3DVertexArray
(
    GLuint& VAO, 
    GLuint& VBO, 
    GLuint& EBO, 
    const float* vertices, 
    const unsigned long vertSize, 
    const unsigned int* indices, 
    const unsigned long indicesSize
)
{
    char buffer[1028];
    std::ofstream aFile;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return 1;
}

void Application::mainLoop()
{
    float vertices[] = {-.5f, -.5f, 0.0f,
                        0.0f, -.5f, 0.0f,
                         .5f, -.5f, 0.0f,
                        0.0f,  .5f, 0.0f,
                        }; 
    unsigned int indices1[] = {0, 1, 3};
    unsigned int indices2[] = {1, 2, 3};
    GLuint VAO[2];
    GLuint VBO[2];
    unsigned int EBO[2];
    std::vector<unsigned int> shaderList;
    unsigned int triangleProgram[2];
    char* inBuffer;
    std::vector<std::pair<long, std::string>> fragmentShaderStrings;
    std::vector<std::pair<long, std::string>> vertexShaderStrings;
    std::vector<std::pair<long, std::string>> outShaders;
    char buffer[1028];
    std::ofstream aFile;

    if(!dumpFilesIntoStrings(GL_VERTEX_SHADER, vertexShaderStrings))return;
    if(!dumpFilesIntoStrings(GL_FRAGMENT_SHADER, fragmentShaderStrings))return;

    outShaders.push_back(vertexShaderStrings[VS_DRAW_VEC3_ARRAY]);
    outShaders.push_back(fragmentShaderStrings[FS_YELLOW_TRIANGLE]);

    if (!prepareShaders(shaderList, outShaders))return;
    if(!createProgram(triangleProgram[0], shaderList))return;

    std::vector<std::pair<long, std::string>>().swap(outShaders);
    std::vector<unsigned int>().swap(shaderList);

    outShaders.push_back(vertexShaderStrings[VS_DRAW_VEC3_ARRAY]);
    outShaders.push_back(fragmentShaderStrings[FS_ORANGE_TRIANGLE]);

    if (!prepareShaders(shaderList, outShaders))return;
    if (!createProgram(triangleProgram[1], shaderList))return;

    save3DVertexArray(VAO[0], VBO[0], EBO[0], vertices, sizeof(vertices), indices1, sizeof(indices1));
    save3DVertexArray(VAO[1], VBO[1], EBO[1], vertices, sizeof(vertices), indices2, sizeof(indices2));

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(triangleProgram[0]);
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glUseProgram(triangleProgram[1]);
        glBindVertexArray(VAO[1]);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    glDeleteProgram(triangleProgram[0]);
    glDeleteProgram(triangleProgram[1]);
}

int Application::showWindow()
{
    glewExperimental = true;
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(800, 600, "Tetris", NULL, NULL);

    if (!window)
    {
        //std::cout << "crap" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, viewPortResize);

    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        //std::cerr << "poop" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);

    return 0;
}

void Application::cleanup()
{
    glfwTerminate();
}

void Application::processInput(GLFWwindow* win)
{
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(win, true);
    }
}