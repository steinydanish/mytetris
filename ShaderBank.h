#ifndef SHADER_BANK_CLASS_H
#define SHADER_BANK_CLASS_H

#include <vector>
#include <iostream>
#include <map>

#define COMPILE_ERROR 17

#define BAD_SHADERGROUP_ID 33
#define SHADER_INIT_FAILED 34

#define VERTEX_SHADER_FIRST 0x1
#define VERTEX_SHADER_LAST  0x40

#define FRAGMENT_SHADER_FIRST 0x80
#define FRAGMENT_SHADER_LAST  0x400

typedef enum
{
    SHADERBANK_GROUP_NONE = 0x0,
    SHADERBANK_GROUP_ALL = 0x4FF
}shaderBankGroupId;

class ShaderBank
{
    public: 
        static ShaderBank* getInstance();
        std::vector<unsigned int> getShaderGroup(unsigned int groupId);
    private:
        ShaderBank();
        std::vector<std::pair<long, std::string>> vertexShaders;
        std::vector<std::pair<long, std::string>> fragmentShaders;
};

#endif
