#include "FILE_OPERATOR.h"
#include <fstream>
#include <sstream>
#include "glew.h"
#include <string.h>

#define FILE_DELIMITER "!!@@BOOYAH@@!!"

int FILE_OPERATOR::dumpFilesIntoStrings(long type, std::vector<std::pair<long, std::string>>& in)
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
                in.push_back({ type, inStream.str() });
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
    in.push_back({ type, inStream.str() });

    if (type == GL_VERTEX_SHADER)
    {
        if (in.size() < 1)
        {
            aFile.open("C:\\Users\\Jacob\\source\\repos\\tetrs3\\aFile.txt", std::ios::out | std::ios::app);
            snprintf(cBuffer, sizeof(cBuffer), "too few vertex shaders\n");
            aFile.write(cBuffer, strlen(cBuffer));
            aFile.close();
            return 0;
        }
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        if (in.size() < 1)
        {
            aFile.open("C:\\Users\\Jacob\\source\\repos\\tetrs3\\aFile.txt", std::ios::out | std::ios::app);
            snprintf(cBuffer, sizeof(cBuffer), "too few fragment shaders\n");
            aFile.write(cBuffer, strlen(cBuffer));
            aFile.close();
            return 0;
        }
    }
    return 1;
}

void FILE_OPERATOR::printStringToErrorLog(std::string error)
{
    static int inited = 0;
    std::ofstream aFile;
    char cBuffer[2056];

    if (!inited)
    {
        aFile.open("aFile.txt", std::ios::out);
        inited = 1;
    }
    else
    {
        aFile.open("aFile.txt", std::ios::out | std::ios::app);
    }

    if (!aFile)
    {
        printf("Error log not found\n");
        fflush(stdout);
    }

    snprintf(cBuffer, sizeof(cBuffer), "%s", error.c_str());
    aFile.write(cBuffer, strlen(cBuffer));

    aFile.close();
}