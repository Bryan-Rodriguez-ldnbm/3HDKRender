#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <memory>

#include "shader.h"

class Geometry
{
public:
    virtual void init() = 0;

    virtual void initTextured() = 0;

    virtual void draw(std::shared_ptr<Shader> shader) = 0;

    virtual void drawTextured(std::shared_ptr<Shader> shader) = 0;

protected:
    std::vector<float> vertices;

};

#endif