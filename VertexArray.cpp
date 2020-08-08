#include "VertexArray.h"
#include "FILE_OPERATOR.h"

#include "glew.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <sstream>
VertexArray::VertexArray()
{
    texIdSet = 0;
    texId = 0;
    glGenVertexArrays(1, &vao);
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

void VertexArray::setTexture(std::string fileName)
{
    int width, height, numChannels;
    unsigned char* data;
    char fName[256];
    snprintf(fName, sizeof(fName), "%s", fileName.c_str());
    data = stbi_load(fName, &width, &height, &numChannels, 0);
    if (!data)
    {
        throw COULD_NOT_LOAD_IMAGE_DATA;
    }

    if (texIdSet)
    {
        glDeleteTextures(1, &texId);
    }

    glGenTextures(1, &texId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    texIdSet = 1;

}

void VertexArray::bindTexture()
{
    if (texIdSet)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId);
    }
}
VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao);

    if(texIdSet)
    {
        glDeleteTextures(1, &texId);
    }
}

