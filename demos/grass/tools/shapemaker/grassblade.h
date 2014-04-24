// grassblade.h
//
// Created at 2013/12/11
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#ifndef GRASSBLADE_H
#define GRASSBLADE_H

#include <glf/glf.h>

#define BLADE_NUM_KNOTS     16

class GrassBlade 
{
public:
    GrassBlade();
    ~GrassBlade();

    void resetShape();

    virtual void render(GLuint times);

    glm::vec3 getKnotPosition(GLint i) const;
    void setKnotPosition(GLint i, const glm::vec3& position);

    void serialize(std::string& text);
    void unserialize(const std::string& text);

private:
    void updateGeometry();
    void updateVertexBuffer();

    struct Knot
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        GLfloat distance;
        glm::vec3 tangent;
    };

private:
    bool              m_dirty;
    GLint             m_numKnots;
    Knot*             m_knots;
    GLfloat           m_length;
    GLuint*           m_indices;
    glf::VertexArray  m_vertexArray;
};


#endif // !GRASSBLADE_H
