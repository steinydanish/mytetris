#ifndef VERTEX_ARRAY_CLASS_H
#define VERTEX_ARRAY_CLASS_H

#include <vector>
#include <string>

#include "BufferObject.h"

#define BUFFER_OBJECT_ALREADY_EXISTED 2
#define BUFFER_OBJECT_NOT_FOUND 3
#define BUFFER_OBJECT_INVALID_TYPE 9
#define COULD_NOT_LOAD_IMAGE_DATA 11


class VertexArray
{
    private:
        BufferObject bufferData;
        BufferObject* getBufferObjectFromType(long type);
        unsigned int vao;
        BufferObject vbo;
        BufferObject ebo;
        unsigned int texId;
        int texIdSet;
    public: 
        VertexArray();
        ~VertexArray();
        int setBuffer(BufferObject& bufferData);
        void setTexture(std::string fileName);
        void bindTexture();
        void bind();
};

#endif
