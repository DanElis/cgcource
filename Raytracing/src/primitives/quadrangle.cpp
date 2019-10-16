#include "primitives/quadrangle.h"


Quadrangle::Quadrangle(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, QObject *parent)
    : Figure(parent),
      t1(p0, p1, p2),
      t2(p2, p3, p0)
{
    wireframe.resize(1);

    auto &w = wireframe[0];

    w.push_back(convertVector3(p0));
    w.push_back(convertVector3(p1));
    w.push_back(convertVector3(p2));
    w.push_back(convertVector3(p3));
}
void Quadrangle::correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const
{
    t1.correctBox(xmin, xmax, ymin, ymax, zmin, zmax);
    t2.correctBox(xmin, xmax, ymin, ymax, zmin, zmax);
}
bool Quadrangle::checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const
{
    if (!t1.checkIntersect(eye, dir, x, y, z))
        return t2.checkIntersect(eye, dir, x, y, z);
    return true;
}

Vector3 Quadrangle::getNormal(const Vector3 &P) const
{
    if (t1.isContains(P))
        return t1.getNormal(P);
    return t2.getNormal(P);
}
