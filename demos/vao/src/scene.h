// scene.h
// The scene objects
// Created at 2014/2/12
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#ifndef SCENE_H
#define SCENE_H

#include <glf/glf.h>

class Scene
{
public:
    Scene();
    ~Scene();

    void render();

    GLF_INLINE const glf::Box& getAABB() const { return m_AABB; }

private:
    glf::Drawable* m_plane1;
    glf::Drawable* m_plane2;
    glf::Drawable* m_plane3;
    glf::Drawable* m_plane4;
    glf::Drawable* m_plane5;

    glf::Box m_AABB;
};





#endif // !SCENE_H
