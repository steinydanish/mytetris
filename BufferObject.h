#ifndef BUFFER_OBJECT_CLASS_H
#define BUFFER_OBJECT_CLASS_H

#include <vector>
#include <string>

#define NO_TYPE_SET -91479128

#define SIZE_MISMATCH_EXCEPTION 41
#define MEMORY_ALLOCATION_EXCEPTION 55

class BufferObject
{
public:
    BufferObject();
    BufferObject(long tp);
    void setVertexPart(std::vector<std::vector<float>>& vori);
    void setVertexPart(std::initializer_list<std::initializer_list<float>> vori);
    std::vector<std::vector<float>>& getVertexIndexPart();
    void setColorPart(std::vector<std::vector<float>> c);
    void setColorPart(std::initializer_list<std::initializer_list<float>> c);
    std::vector<std::vector<float>>& getColorPart();
    void setTexturePart(std::vector<std::vector<float>> t);
    void setTexturePart(std::initializer_list<std::initializer_list<float>> t);
    std::vector<std::vector<float>>& getTexturePart();
    void setIndexPart(std::vector<std::vector<unsigned int>>& vori);
    void setIndexPart(std::initializer_list<std::initializer_list<unsigned int>> vori);
    std::vector<std::vector<unsigned int>>& getIndexPart();
    size_t getColorOffset();
    size_t getTextureOffset();
    size_t getTotalSize();
    size_t getIndexSize();
    size_t getStride();
    long getType();
    unsigned int getId();
    float* const getFlattenedBufferObjectData();
    unsigned int* const getFlattenedIndexData();
    ~BufferObject();
private:
    std::vector<std::vector<unsigned int>> indexPart;
    std::vector<std::vector<float>> vertexPart;
    std::vector<std::vector<float>> colorPart;
    std::vector<std::vector<float>> texturePart;
    long type;
    size_t viSize;
    size_t colorSize;
    size_t textureSize;
    size_t indexSize;
    size_t totalSize;
    unsigned int id;
    float* flattenedData;
    unsigned int* flattenedIndexData;
    bool resetFlattenedData;
    bool resetIndexData;
    
};
#endif
