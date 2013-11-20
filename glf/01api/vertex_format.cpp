//
// vertex_format.cpp
// Hongwei Li (lihw81@gmail.com)
//
//


#include "vertex_format.h"

VertexFormat::VertexFormat()
{
    _vertexBuffer = 0;
    _indexBuffer  = 0;
    _vertexArray  = 0;
}

void VertexFormat::destroy()
{
    if (glIsBuffer(_vertexBuffer))
    {
        glDeleteBuffers(1, &_vertexBuffer);
    }
    if (glIsBuffer(_indexBuffer))
    {
        glDeleteBuffers(1, &_indexBuffer);
    }
    if (glIsVertexArray(_vertexArray))
    {
        glDeleteVertexArrays(1, &_vertexArray);
    }
}

void VertexFormat::setVertexAttributes(VertexFormatDescription* description, const GLUSsizei count, const GLUSboolean gpuDeployed)
{
    if (!glIsVertexArray(_vertexArray))
    {
        glGenVertexArrays(1, &_vertexArray);
    }

    glBindVertexArray(_vertexArray);

    // Calculate the buffer size.
    GLUSsizei bufferSize = 0;
    GLUSsizei* strides = new GLUSsizei [count];
    GLUSsizei stride = 0;
    for (GLUSsizei i = 0; i < count; ++i)
    {
        if (_count != description[i].count && i != 0)
        {
#if defined _DEBUG
            glusLogPrint(GLUS_LOG_ERROR, "%s: vertex attributes are not in the same number", __FUNCTION__);
#endif
            delete [] strides;
            return ;
        }
        if (i == 0)
        {
            _count = description[i].count;
        }

        GLUSsizei nelements = description[i].size * description[i].count;

        GLUSsizei nElementBytes = 0;
        switch (description[i].type)
        {
            case GL_INT:
            case GL_UNSIGNED_INT:
            case GL_FLOAT:
                nElementBytes = 4; 
                break;
            case GL_SHORT:
            case GL_UNSIGNED_SHORT:
                nElementBytes = 2;
                break;
            case GL_BYTE:
            case GL_UNSIGNED_BYTE:
                nElementBytes = 1;
                break;
        }

        strides[i] = nElementBytes * description[i].size;
        stride += strides[i];

        bufferSize += nElementBytes * nelements;
    }

    if (gpuDeployed)
    {
        // Create the buffer.
        glGenBuffers(1, &_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, bufferSize, 0, GL_STATIC_DRAW);

        // Upload data to GPU.
        GLUSbyte* dataptr = (GLUSbyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
        //GLUSbyte* dataptr = (GLUSbyte*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        GLUSbyte* dstFirst = dataptr;
        for (GLUSsizei i = 0; i < count; i++)
        {
            GLUSbyte* dst = dstFirst;
            GLUSbyte* src = (GLUSbyte*)description[i].data; 
            for (GLUSint j = 0; j < description[i].count; j++)
            {
                memcpy(dst, src, strides[i]);
                src += strides[i];
                dst += stride;
            }
            dstFirst += strides[i];
        }

        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    // Specify the vertex format.
    GLUSbyte* offset = (GLUSbyte *)0;
    for (GLUSsizei i = 0; i < count; i++)
    {
        if (!gpuDeployed)
        {
            glVertexAttribPointer(i, description[i].size, description[i].type, GL_FALSE, 0, description[i].data);
        }
        else
        {
            glVertexAttribPointer(i, description[i].size, description[i].type, GL_FALSE, stride, offset);
            offset += strides[i];
        }

        glEnableVertexAttribArray(i);
    }

    if (gpuDeployed)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    delete [] strides;

    glBindVertexArray(0);
}


void VertexFormat::setIndexArray(GLUSvoid* indices, GLUSenum type, GLUSsizei count, const GLUSboolean gpuDeployed)
{
    if (!glIsVertexArray(_vertexArray))
    {
        glGenVertexArrays(1, &_vertexArray);
    }

    glBindVertexArray(_vertexArray);

    GLUSsizei typeSize = 0;
    if (gpuDeployed)
    {
        switch (type)
        {
            case GL_UNSIGNED_INT:
            case GL_INT:
                typeSize = 4;
                break;
            case GL_UNSIGNED_SHORT:
            case GL_SHORT:
                typeSize = 2;
                break;
            case GL_UNSIGNED_BYTE:
            case GL_BYTE:
                typeSize = 1;
                break;
        }
        glGenBuffers(1, &_indexBuffer);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, typeSize * count, indices, GL_STATIC_DRAW);

        _count = count;
        _type = type;
        _gpuDeployed = GLUS_TRUE;
    }
    else
    {
        _indexBuffer = (GLUSuint)indices;
        
        _count = count;
        _type = type;
        _gpuDeployed = GLUS_TRUE;
    }

    glBindVertexArray(0);
}

void VertexFormat::render(GLUSenum mode, GLUSsizei instancecount)
{
    if (!glIsBuffer(_vertexBuffer))
    {
        return ;
    }

    if (!glIsVertexArray(_vertexArray))
    {
#if defined _DEBUG
        glusLogPrint(GLUS_LOG_ERROR, "%s: no vertex array!", __FUNCTION__);
#endif
        return ;
    }

    glBindVertexArray(_vertexArray);

    if (_indexBuffer)
    {
        if (_gpuDeployed)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
            glDrawElements(mode, _count, _type, 0); 
            //glDrawElementsInstancedBaseInstance(mode, _count, _type, 0, instancecount, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            glDrawElementsInstancedBaseInstance(mode, _count, _type, (GLUSvoid *)_indexBuffer, instancecount, 0);
        }
    }
    else
    {
        glDrawArraysInstanced(mode, 0, _count, instancecount);
    }

    glBindVertexArray(0);
}
