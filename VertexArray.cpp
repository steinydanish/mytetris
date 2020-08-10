#include "VertexArray.h"
#include "FILE_OPERATOR.h"

#include "glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <sstream>
VertexArray::VertexArray()
{
    glGenVertexArrays(1, &vao);
}

static std::pair<std::string, unsigned int> findTextureByName
(const std::vector<std::pair<std::string, unsigned int>>& pairVector, const std::string& target)
{
    static std::pair<std::string, unsigned int> notFoundPair("NOTEXTURE", 0);
    std::vector<std::pair<std::string, unsigned int>>::const_iterator iter;
    
    for(iter = pairVector.begin(); iter != pairVector.end(); iter++)
    {
        if(iter->first == target)
        {
            return *iter;
        }
    }
    return notFoundPair;
}

int VertexArray::setBuffer(BufferObject& bufferData)
{
    unsigned int newVBO;
    int returnValue = 0;
    unsigned int vectorSize = 0;
    int i = 0;
    int offset = 0;
    BufferObject* bufferPtr;
    std::stringstream sstr;
    size_t stride;
    size_t size = (bufferData.getType() == GL_ARRAY_BUFFER)?
                  (bufferData.getTotalSize()):(bufferData.getIndexSize());
    void* dataPtr = (bufferData.getType() == GL_ARRAY_BUFFER)?
                  ((void*)bufferData.getFlattenedBufferObjectData()):
                  ((void*)bufferData.getFlattenedIndexData());

    //bufferPtr = getBufferObjectFromType(bufferData.getType());
    //*bufferPtr = bufferData;

    glBindVertexArray(vao);

    glBindBuffer(bufferData.getType(), bufferData.getId());
    glBufferData(bufferData.getType(), size, dataPtr, GL_STATIC_DRAW);

    if(bufferData.getType() == GL_ARRAY_BUFFER)
    { 
        stride = bufferData.getStride();
        if (bufferData.getVertexIndexPart().size() > 0)
        {
            glVertexAttribPointer(0, bufferData.getVertexIndexPart().begin()->size(), GL_FLOAT, GL_FALSE, stride, (void*)(0));
            glEnableVertexAttribArray(0);
        }

        if (bufferData.getColorPart().size() > 0)
        {
            glVertexAttribPointer(1, bufferData.getColorPart().begin()->size(), GL_FLOAT, GL_FALSE, stride, (void*)(bufferData.getColorOffset()));
            glEnableVertexAttribArray(1);
        }

        if (bufferData.getTexturePart().size() > 0)
        {
            glVertexAttribPointer(2, bufferData.getTexturePart().begin()->size(), GL_FLOAT, GL_FALSE, stride, (void*)(bufferData.getTextureOffset()));
            glEnableVertexAttribArray(2);
        }
    }
    return returnValue;
}

void VertexArray::bind()
{
    glBindVertexArray(vao);
}

BufferObject* VertexArray::getBufferObjectFromType(long type)
{
    if (type == GL_ARRAY_BUFFER)
    {
        return &vbo;
    }
    else if (type == GL_ELEMENT_ARRAY_BUFFER)
    {
        return &ebo;
    }
    else
    {
        throw BUFFER_OBJECT_INVALID_TYPE;
    }
}

void VertexArray::setTexture(std::string fileName, VAaccessType type)
{
    int width, height, numChannels;
    unsigned char* data;
    char fName[256];
    unsigned int texId;
    std::pair<std::string, unsigned int> pair;
    static unsigned int offset = 0;

    pair = findTextureByName(textureNames, fileName);
    if(pair.first == "NOTEXTURE")
    {
        stbi_set_flip_vertically_on_load(true);
        snprintf(fName, sizeof(fName), "%s", fileName.c_str());
        data = stbi_load(fName, &width, &height, &numChannels, 0);
    
        if (!data)
        {
            throw COULD_NOT_LOAD_IMAGE_DATA;
        }
        glGenTextures(1, &texId);  
        textureNames.push_back(std::pair<std::string, unsigned int>(fileName, texId));
        glActiveTexture(GL_TEXTURE0 + offset);
        glBindTexture(GL_TEXTURE_2D, texId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    }
    else
    {
        texId = pair.second;
        glActiveTexture(GL_TEXTURE0 + offset);
        glBindTexture(GL_TEXTURE_2D, texId);
    }
    
    if(type == VA_OVERWRITE)
    {
        offset = 0;
    }
    
    offset = (offset + 1) % 16;
}

void VertexArray::bindAllTextures()
{
    unsigned int offset = 0;
    std::vector<std::pair<std::string, unsigned int>>::const_iterator iter;
    for(iter = textureNames.begin(); iter != textureNames.end(); iter++)
    {
        glActiveTexture(GL_TEXTURE0 + offset);
        glBindTexture(GL_TEXTURE_2D, iter->second);
        
        offset = (offset + 1) % 16;
    }
}

VertexArray::~VertexArray()
{
    std::vector<std::pair<std::string, unsigned int>>::const_iterator iter;
    
    glDeleteVertexArrays(1, &vao);

    for(iter = textureNames.begin(); iter != textureNames.end(); iter++)
    {
        glDeleteTextures(1, &(iter->second));
    }
}

