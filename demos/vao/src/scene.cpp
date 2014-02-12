// scene.cpp
// The grass objects
// Created at 2014/2/12
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "scene.h"

Scene::Scene()
{
    GLfloat q1[] = 
    {
        -1.5f, 0.1f, -1.5f, 
        -1.5f, 0.1f,  1.5f, 
         1.5f, 0.1f,  1.5f, 
         1.5f, 0.1f, -1.5f, 
    };
    GLfloat q2[] = 
    {
        -1.25f, 0.5f, -1.25f, 
        -1.25f, 0.5f,  1.25f, 
         1.25f, 0.5f,  1.25f, 
         1.25f, 0.5f, -1.25f, 
    };
    GLfloat q3[] = 
    {
        -1.0f, 0.9f, -1.0f, 
        -1.0f, 0.9f,  1.0f, 
         1.0f, 0.9f,  1.0f, 
         1.0f, 0.9f, -1.0f, 
    };
    GLfloat q4[] = 
    {
        -0.8f, 1.3f, -0.8f, 
        -0.8f, 1.3f,  0.8f, 
         0.8f, 1.3f,  0.8f, 
         0.8f, 1.3f, -0.8f, 
    };
    GLfloat q5[] = 
    {
        -0.5f, 1.7f, -0.5f, 
        -0.5f, 1.7f,  0.5f, 
         0.5f, 1.7f,  0.5f, 
         0.5f, 1.7f, -0.5f, 
    };

    m_plane1 = new glf::Drawable(new glf::Quad(q1));
    m_plane2 = new glf::Drawable(new glf::Quad(q2));
    m_plane3 = new glf::Drawable(new glf::Quad(q3));
    m_plane4 = new glf::Drawable(new glf::Quad(q4));
    m_plane5 = new glf::Drawable(new glf::Quad(q5));

    glm::vec3 minPos = glm::vec3(-1.5f, 0.1f, -1.5f);
    glm::vec3 maxPos = glm::vec3( 1.5f, 1.7f,  1.5f);

    m_AABB.setMinMax(minPos, maxPos);
}

Scene::~Scene()
{
    delete m_plane1;
    delete m_plane2;
    delete m_plane3;
    delete m_plane4;
    delete m_plane5;
}

void Scene::render()
{
    m_plane1->render(1);
    m_plane2->render(1);
    m_plane3->render(1);
    m_plane4->render(1);
    m_plane5->render(1);
}
