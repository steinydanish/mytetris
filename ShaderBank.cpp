#include "ShaderBank.h"
#include "FILE_OPERATOR.h"
#include "glew.h"
#include <fstream>
#include <string.h>

static unsigned int createShaderFromPair(std::pair<long, std::string> pair)
{
    int success;
    char infoLog[512];
    char buffer[2048];
    char* bPtr = buffer;
    std::ofstream aFile;
    unsigned int shaderId;
    static const std::map<long, std::string> typeMap = { {GL_VERTEX_SHADER, "VERTEX"},{GL_FRAGMENT_SHADER, "FRAGMENT"} };

    shaderId = glCreateShader(pair.first);

    snprintf(buffer, sizeof(buffer), "%s", pair.second.c_str());

    glShaderSource(shaderId, 1, (const GLchar** const)(&bPtr), NULL);

    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        aFile.open("aFile.txt", std::ios::out | std::ios::app);
        snprintf(buffer, sizeof(buffer), "%s failed to compile:\n%s\n", typeMap.at(pair.first).c_str(), infoLog);
        aFile.write(buffer, strlen(buffer));
        aFile.close();
        throw COMPILE_ERROR;
    }
    return shaderId;
}

ShaderBank* ShaderBank::getInstance()
{
    static ShaderBank* sBank = NULL;
    if (!sBank)
    {
        sBank = new ShaderBank;
    }
    return sBank;
}

std::vector<unsigned int> ShaderBank::getShaderGroup(unsigned int groupId)
{
    std::vector<unsigned int> returnVec;
    std::vector<std::pair<long, std::string>>::const_iterator tsIter;
    char buffer[512];
    std::ofstream aFile;

    if (groupId < FRAGMENT_SHADER_FIRST || (groupId % FRAGMENT_SHADER_FIRST == 0))
    {
        throw BAD_SHADERGROUP_ID;
    }
    unsigned int tracker = VERTEX_SHADER_FIRST;
    for (tsIter = vertexShaders.begin(); tsIter != vertexShaders.end() && tracker <= VERTEX_SHADER_LAST; tsIter++)
    {
        if (tracker & groupId)
        {
            returnVec.push_back(createShaderFromPair(*tsIter));
        }
        tracker << 1;
    }
    tracker = FRAGMENT_SHADER_FIRST;
    for (tsIter = fragmentShaders.begin(); tsIter != fragmentShaders.end() && tracker <= FRAGMENT_SHADER_LAST; tsIter++)
    {
        if (tracker & groupId)
        {
            returnVec.push_back(createShaderFromPair(*tsIter));
        }
        tracker << 1;
    }
    return returnVec;
}

ShaderBank::ShaderBank()
{
    if (!FILE_OPERATOR::dumpFilesIntoStrings(GL_VERTEX_SHADER, vertexShaders))throw SHADER_INIT_FAILED;
    if (!FILE_OPERATOR::dumpFilesIntoStrings(GL_FRAGMENT_SHADER, fragmentShaders))throw SHADER_INIT_FAILED;
}