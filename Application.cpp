#include "Application.h"
#include "Program.h"
#include "FILE_OPERATOR.h"
#include "ShaderBank.h"
#include "VertexArray.h"
#include "BufferObject.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <stdio.h>
#include <map>
#include <string.h>
#include <vector>
#include <math.h>


#define FS_COLOR_FROM_VECTOR 0

#define VS_DRAW_VEC3_ARRAY 0

void logError(int n)
{
    if (n == BUFFER_OBJECT_INVALID_TYPE)
    {
        FILE_OPERATOR::printStringToErrorLog("invalid buffer type\n");
    }
    else if (n == MEMORY_ALLOCATION_EXCEPTION)
    {
        FILE_OPERATOR::printStringToErrorLog("malloc failed\n");
    }
    else if (n == SIZE_MISMATCH_EXCEPTION)
    {
        FILE_OPERATOR::printStringToErrorLog("vector sizes were off\n");
    }
    else if (n == USED_UNLINKED_PROGRAM)
    {
        FILE_OPERATOR::printStringToErrorLog("tried to use unlinked program\n");
    }
    else if (n == COULD_NOT_LOAD_IMAGE_DATA)
    {
        FILE_OPERATOR::printStringToErrorLog("failed to load image data\n");
    }
    else if (n == BAD_SHADERGROUP_ID)
    {
        FILE_OPERATOR::printStringToErrorLog("bad shader group id\n");
    }
    else if (n == SHADER_INIT_FAILED)
    {
        FILE_OPERATOR::printStringToErrorLog("failed to initialize shaders\n");
    }
}

static void viewPortResize(GLFWwindow* win, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::mainLoop()
{

try
{
    VertexArray va;
    std::vector<unsigned int> shaderList;
    Program program;
    ShaderBank* sb = ShaderBank::getInstance();
    BufferObject vboData(GL_ARRAY_BUFFER);
    BufferObject eboData(GL_ELEMENT_ARRAY_BUFFER);

    FILE_OPERATOR::printStringToErrorLog("");

    vboData.setVertexIndexPart({{ 0.5f, 0.5f, 0.0f},
                                {0.5f, -0.5f, 0.0f},
                                {-0.5f, -0.5f, 0.0f},
                                {-0.5f,  0.5f, 0.0f}});

    vboData.setColorPart({{1.0f, 0.1f, 0.2f},
                          {0.1f, 1.0f, 0.3f}, 
                          {0.1f, 0.2f, 1.0f},
                          {0.7f, 1.0f, 0.1f}});

    //vboData.setTexturePart({{1.0f, 1.0f}, 
    //                        {1.0f, 0.0f}, 
    //                        {0.0f, 0.0f}, 
    //                        {0.0f, 1.0f}});

    eboData.setVertexIndexPart({{0, 1, 3},
                                {1, 2, 3}});

    shaderList = sb->getShaderGroup(SHADERBANK_GROUP_ALL);

    program.addShaders(shaderList);
    program.link();

    std::vector<unsigned int>().swap(shaderList);

    va.setBuffer(vboData);
    va.setBuffer(eboData);
  //  va.setTexture("images\\wall.jpg");
  //  program.use();

   // glUniform1i(glGetUniformLocation(program.getId(), "ourTexture"), 0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

//        va.bindTexture();
        program.use();

        va.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    }
catch (int n)
{
    logError(n);
}
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