// grassblade.cpp
//
// Created at 2013/12/11
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "grassblade.h"    

#include <string>
#include <sstream>

#define BLADE_NUM_KNOTS     8
#define BLADE_LENGTH        4.0f

GrassBlade::GrassBlade()
{
    // Initialize the grass blade geometry
    m_numKnots = BLADE_NUM_KNOTS;
    m_length = BLADE_LENGTH;
    
    m_knots = new Knot [m_numKnots * 3];
    m_indices = new GLuint [(m_numKnots - 1) * 2];
    for (GLint i = 0; i < m_numKnots - 1; ++i)
    {
        m_indices[i * 2 + 0] = i;
        m_indices[i * 2 + 1] = i + 1;
    } 
    
    resetShape();
    updateGeometry();

    // Create the vertex buffer 
    glf::VertexAttribDescriptor vertexAttribs[] =
    {
        {glf::VERTEX_ATTRIB_POSITION, GL_FLOAT, 3}, 
        {glf::VERTEX_ATTRIB_NORMAL,   GL_FLOAT, 3}, 
        {glf::VERTEX_ATTRIB_COLOR,    GL_FLOAT, 3}, 
        {glf::VERTEX_ATTRIB_UNNAMED0, GL_FLOAT, 1}, 
        {glf::VERTEX_ATTRIB_UNNAMED1, GL_FLOAT, 3}, 
    };
    m_vertexArray.create(GL_LINES, 
            (GLfloat*)m_knots, 
            m_numKnots,
            vertexAttribs,
            5,
            m_indices,
            2 * (m_numKnots - 1));

    
    m_dirty = false;
}

void GrassBlade::resetShape()
{
    for (GLint i = 0; i < m_numKnots; ++i)
    {
        Knot* k = &m_knots[i];

        k->position.x = 0;
        k->position.y = m_length * (GLfloat)i / (GLfloat)(m_numKnots - 1);
        k->position.z = 0;
    }

    m_dirty = true;
}

GrassBlade::~GrassBlade()
{
    delete [] m_knots;
    delete [] m_indices;
}

void GrassBlade::render(GLuint times)
{
    if (m_dirty)
    {
        // Update the normal and other vertex information.
        updateGeometry();

        // Update the vertex buffer
        updateVertexBuffer();

        m_dirty = false;
    }

    m_vertexArray.render(1);
}

glm::vec3 GrassBlade::getKnotPosition(GLint i) const
{
    return glm::vec3(m_knots[i].position[0], 
                     m_knots[i].position[1],
                     m_knots[i].position[2]);
}

void GrassBlade::setKnotPosition(GLint i, const glm::vec3& position) 
{
    m_dirty = true;

    m_knots[i].position[0] = position.x;
    m_knots[i].position[1] = position.y;
    m_knots[i].position[2] = position.z;
}

void GrassBlade::serialize(std::string& text)
{
    std::stringstream stream;

    for (GLuint i = 0; i < m_numKnots; ++i)
    {
        stream << "(" << m_knots[i].position[0] << "," <<  m_knots[i].position[1] << "," << m_knots[i].position[2] << ")";
    }

    stream >> text;
}

void GrassBlade::unserialize(const std::string& text)
{
    const char* p = text.c_str();
    for (GLuint i  = 0; i < m_numKnots; ++i)
    {
        const char *end = strchr(p, ')');
        char buffer[1024];
        strncpy(buffer, p, end - p + 1);
        buffer[end - p + 1] = 0;
        sscanf(buffer, "(%f,%f,%f)", 
            &m_knots[i].position[0],
            &m_knots[i].position[1],
            &m_knots[i].position[2]);
        p = end + 1;
    }

    m_dirty = true;
}

void GrassBlade::updateGeometry()
{
    glm::vec3 b(0, 0, -1.0f);

    GLfloat length = 0;

    // Recompute the normal and tangent of each knot.
    for (GLuint i = 0; i < m_numKnots; ++i)
    {
        if (i == 0)
        {
            glm::vec3 t = m_knots[1].position - m_knots[0].position;
            m_knots[0].normal = glm::normalize(glm::cross(t, b));
            m_knots[0].tangent = glm::normalize(t);
        }
        else if (i == m_numKnots - 1)
        {
            glm::vec3 t = m_knots[m_numKnots - 1].position - m_knots[m_numKnots - 2].position;
            m_knots[m_numKnots - 1].normal = glm::normalize(glm::cross(t, b));
            m_knots[m_numKnots - 1].tangent = glm::normalize(t);
        }
        else
        {
            glm::vec3 t0 = m_knots[i + 1].position - m_knots[i].position;
            glm::vec3 t1 = m_knots[i].position - m_knots[i - 1].position;
            glm::vec3 t = t0 + t1;
            m_knots[i].tangent = glm::normalize(t);
            m_knots[i].normal = glm::normalize(glm::cross(t, b));
        }

        if (i >= 1)
        {
            m_knots[i].distance = m_knots[i - 1].distance + glm::distance(m_knots[i - 1].position, m_knots[i].position);
        }
        else
        {
            m_knots[i].distance = 0;
        }
    }

    for (GLuint i = 0; i < m_numKnots; ++i)
    {
        m_knots[i].distance /= m_knots[m_numKnots - 1].distance;
    }

    m_knots[m_numKnots - 1].distance = 1.0f;
}

void GrassBlade::updateVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexArray.getVertexBufferHandle());
    GLfloat* v = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(v, m_knots, sizeof(GLfloat) * sizeof(Knot) * m_numKnots);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

