#ifndef PROGRAM_CLASS_H
#define PROGRAM_CLASS_H

#include <vector>
#include <string>
#include <map>

#define USED_UNLINKED_PROGRAM 222

class Program
{
    public:
        Program();
        void addShader(unsigned int shaderId);
        void addShaders(unsigned int* shaderIds, int numShaders);
        void addShaders(std::vector<unsigned int> shaderIds);
        void use();
        int link();
        unsigned int getId();
        ~Program();
    private:
        unsigned int programid;
        std::vector<unsigned int> shaders;
        bool linked;
};

#endif