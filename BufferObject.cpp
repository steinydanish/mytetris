#include "BufferObject.h"
#include "glew.h"
#include "FILE_OPERATOR.h"

#include <sstream>

BufferObject::BufferObject()
{
    totalSize = 0;
    viSize = 0;
    colorSize = 0;
    textureSize = 0;
    resetFlattenedData = false;
    flattenedData = NULL;
    type = NO_TYPE_SET;

    glGenBuffers(1, &id);
}

BufferObject::BufferObject(long tp):
type{tp}
{
    totalSize = 0;
    viSize = 0;
    colorSize = 0;
    textureSize = 0;
    resetFlattenedData = false;
    flattenedData = NULL;

    glGenBuffers(1, &id);
}

void BufferObject::setVertexIndexPart(std::initializer_list<std::initializer_list<float>> vori)
{
    vertexIndexPart.assign(vori.begin(), vori.end());

    totalSize -= viSize;
    viSize = vertexIndexPart.size() * vertexIndexPart.begin()->size() * sizeof(float);
    totalSize += viSize;
    resetFlattenedData = true;
}

void BufferObject::setVertexIndexPart(std::vector<std::vector<float>>& vori)
{
    std::vector<std::vector<float>>().swap(vertexIndexPart);
    vertexIndexPart = vori;
    std::vector<std::vector<float>>::const_iterator vfIter;
    size_t elZeroSize = vertexIndexPart.begin()->size();

    for (vfIter = vertexIndexPart.begin(); vfIter != vertexIndexPart.end(); vfIter++)
    {
        if (vfIter->size() != elZeroSize)
        {
            throw SIZE_MISMATCH_EXCEPTION;
        }
    }

    totalSize -= viSize;
    viSize = vertexIndexPart.size() * vertexIndexPart.begin()->size() * sizeof(float);
    totalSize += viSize;
    resetFlattenedData = true;
}

void BufferObject::setColorPart(std::initializer_list<std::initializer_list<float>> color)
{
    colorPart.assign(color.begin(), color.end());

    totalSize -= colorSize;
    colorSize = colorPart.size() * colorPart.begin()->size() * sizeof(float);
    totalSize += colorSize;
    resetFlattenedData = true;
}

void BufferObject::setColorPart(std::vector<std::vector<float>> color)
{
    std::vector<std::vector<float>>().swap(colorPart);
    colorPart = color;
    std::vector<std::vector<float>>::const_iterator vfIter;
    size_t elZeroSize = colorPart.begin()->size();

    for (vfIter = colorPart.begin(); vfIter != colorPart.end(); vfIter++)
    {
        if (vfIter->size() != elZeroSize)
        {
            throw SIZE_MISMATCH_EXCEPTION;
        }
    }

    totalSize -= colorSize;
    colorSize = colorPart.size() * colorPart.begin()->size() * sizeof(float);
    totalSize += colorSize;
    resetFlattenedData = true;
}

void BufferObject::setTexturePart(std::initializer_list<std::initializer_list<float>> texture)
{
    texturePart.assign(texture.begin(), texture.end());

    totalSize -= textureSize;
    textureSize = texturePart.size() * texturePart.begin()->size() * sizeof(float);
    totalSize += textureSize;
    resetFlattenedData = true;
}

void BufferObject::setTexturePart(std::vector<std::vector<float>> texture)
{
    std::vector<std::vector<float>>().swap(texturePart);
    texturePart = texture;
    std::vector<std::vector<float>>::const_iterator vfIter;
    size_t elZeroSize = texturePart.begin()->size();

    for (vfIter = texturePart.begin(); vfIter != texturePart.end(); vfIter++)
    {
        if (vfIter->size() != elZeroSize)
        {
            throw SIZE_MISMATCH_EXCEPTION;
        }
    }

    totalSize -= textureSize;
    textureSize = texturePart.size() * texturePart.begin()->size() * sizeof(float);
    totalSize += textureSize;
    resetFlattenedData = true;
}

std::vector<std::vector<float>>& BufferObject::getVertexIndexPart()
{
    return vertexIndexPart;
}

std::vector<std::vector<float>>& BufferObject::getColorPart()
{
    return colorPart;
}

std::vector<std::vector<float>>& BufferObject::getTexturePart()
{
    return texturePart;
}

void BufferObject::setType(long type)
{
    this->type = type;
}

long BufferObject::getType()
{
    return type;
}

size_t BufferObject::getTotalSize()
{
    return totalSize;
}

size_t BufferObject::getStride()
{
    size_t stride = 0;
    if (vertexIndexPart.size() > 0)
    {
        stride += vertexIndexPart.begin()->size();
    }
    if (colorPart.size() > 0)
    {
        stride += colorPart.begin()->size();
    }
    if (texturePart.size() > 0)
    {
        stride += texturePart.begin()->size();
    }
    return stride * sizeof(float);
}
size_t BufferObject::getColorOffset()
{
    if (colorPart.size() == 0)
    {
        return 0;
    }
    return (vertexIndexPart.size() > 0)?(vertexIndexPart.begin()->size() * sizeof(float)):(0);
}

size_t BufferObject::getTextureOffset()
{
    if (texturePart.size() == 0)
    {
        return 0;
    }
    return (((vertexIndexPart.size() > 0) ? (vertexIndexPart.begin()->size()):(0)) +
            ((colorPart.size() > 0)?(colorPart.begin()->size()):(0))) * sizeof(float);
}


float* const BufferObject::getFlattenedBufferObjectData()
{
    std::vector<std::vector<float>>::const_iterator viIter = vertexIndexPart.begin();
    std::vector<std::vector<float>>::const_iterator colorIter = colorPart.begin();
    std::vector<std::vector<float>>::const_iterator textureIter = texturePart.begin();
    std::string errMsg;
    std::stringstream sstr;
    size_t addressPtr = 0;

    if(resetFlattenedData)
    { 
        if (flattenedData)
        {
            free(flattenedData);
        }
        flattenedData = (float*)malloc(totalSize);
        if (!flattenedData)
        {
            throw MEMORY_ALLOCATION_EXCEPTION;
        }
        
        while (viIter != vertexIndexPart.end() || colorIter != colorPart.end() || textureIter != texturePart.end())
        {
            if(viIter != vertexIndexPart.end())
            {
                std::copy(viIter->begin(), viIter->end(), &flattenedData[addressPtr]);
                addressPtr += viIter->size();
                viIter++;
            }
            if(colorIter != colorPart.end())
            {
                std::copy(colorIter->begin(), colorIter->end(), &flattenedData[addressPtr]);
                addressPtr += colorIter->size();
                colorIter++;
            }
            if(textureIter != texturePart.end())
            {
                std::copy(textureIter->begin(), textureIter->end(), &flattenedData[addressPtr]);
                addressPtr += textureIter->size();
                textureIter++;
            }
        }
        resetFlattenedData = false;
    }
    return flattenedData;
}

unsigned int BufferObject::getId()
{
    return id;
}

BufferObject::~BufferObject()
{
    if(flattenedData)
    { 
        free(flattenedData);
    }
    glDeleteBuffers(1, &id);
}