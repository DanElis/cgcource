#include "light.h"


Light::Light(QObject *parent) : QObject(parent)
{
}

Light::Light(Vector3 position, float R, float G, float B, QObject *parent)
    : QObject(parent),
      position(std::move(position)),
      R(R), G(G), B(B)
{
}

Light::Light(const Light &other)
{
    position = other.position;
    R = other.R;
    G = other.G;
    B = other.B;
}

Light::Light(Light &&other)
{
    position = std::move(other.position);
    R = other.R;
    G = other.G;
    B = other.B;
}

Light &Light::operator=(const Light &other)
{
    position = other.position;
    R = other.R;
    G = other.G;
    B = other.B;

    return *this;
}

Light &Light::operator=(Light &&other)
{
    position = std::move(other.position);
    R = other.R;
    G = other.G;
    B = other.B;

    return *this;
}
