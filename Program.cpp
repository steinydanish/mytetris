#include "Program.h"
#include "FILE_OPERATOR.h"

#include "glew.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

Program::Program()
{
   programid = glCreateProgram();
   linked = 0;
}
Program::~Program()
{
    glDeleteProgram(programid);
}

unsigned int Program::getId()
{
    return programid;
}

void Program::addShader(unsigned int shaderId)
{
    shaders.push_back(shaderId);
}
void Program::addShaders(unsigned int* shaderIds, int numShaders)
{
    for (int i = 0; i < numShaders; i++)
    {
        shaders.push_back(shaderIds[i]);
    }
}
void Program::addShaders(std::vector<unsigned int> shaderIds)
{
    std::vector<unsigned int>::const_iterator sidIter;
    
    for (sidIter = shaderIds.begin(); sidIter != shaderIds.end(); sidIter++)
    {
        shaders.push_back(*sidIter);
    }
}
void Program::use()
{
    if(linked)
    {
        glUseProgram(programid);
    }
    else
    {
        throw USED_UNLINKED_PROGRAM;
    }
}

/*
 * prepars program for use, clears stored shaders
 */
int Program::link()
{
    char buffer[512];
    char infoLog[512];
    int success;
    std::ofstream aFile;
    std::vector<unsigned int>::const_iterator vectIter;

    for (vectIter = shaders.begin(); vectIter != shaders.end(); vectIter++)
    {
        glAttachShader(programid, *vectIter);
    }
    glLinkProgram(programid);

    for (vectIter = shaders.begin(); vectIter != shaders.end(); vectIter++)
    {
        glDeleteShader(*vectIter);
    }
    std::vector<unsigned int>().swap(shaders);

    glGetProgramiv(programid, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programid, 512, NULL, infoLog);
        aFile.open("C:\\Users\\schec\\myWorkspace\\tetris\\aFile.txt", std::ios::out | std::ios::app);
        snprintf(buffer, sizeof(buffer), "failed to link:\n%s\n", infoLog);
        aFile.write(buffer, strlen(buffer));
        aFile.close();
        throw LINK_FAILURE;
    }
    linked = 1;
    return 1;
}