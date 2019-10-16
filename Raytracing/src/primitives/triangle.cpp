#include "primitives/triangle.h"


Triangle::Triangle(Vector3 p0, Vector3 p1, Vector3 p2, QObject *parent)
    : Figure(parent),
      p0(std::move(p0)),
      p1(std::move(p1)),
      p2(std::move(p2)),
      l0(p1 - p0),
      l1(p2 - p1),
      l2(p0 - p2)
{
    normal = Vector3::crossProduct(p1 - p0, p2 - p0).normalized();
    mD = Vector3::dotProduct(normal, p0);

    xmin = std::min(std::min(p0.x(), p1.x()), p2.x()) - 10e-3f;
    xmax = std::max(std::max(p0.x(), p1.x()), p2.x()) + 10e-3f;
    ymin = std::min(std::min(p0.y(), p1.y()), p2.y()) - 10e-3f;
    ymax = std::max(std::max(p0.y(), p1.y()), p2.y()) + 10e-3f;
    zmin = std::min(std::min(p0.z(), p1.z()), p2.z()) - 10e-3f;
    zmax = std::max(std::max(p0.z(), p1.z()), p2.z()) + 10e-3f;


    wireframe.resize(1);

    auto &w = wireframe[0];

    w.push_back(convertVector3(p0));
    w.push_back(convertVector3(p1));
    w.push_back(convertVector3(p2));
}

void Triangle::correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const
{
    xmin = std::min(xmin, this->xmin);
    xmax = std::max(xmax, this->xmax);
    ymin = std::min(ymin, this->ymin);
    ymax = std::max(ymax, this->ymax);
    zmin = std::min(zmin, this->zmin);
    zmax = std::max(zmax, this->zmax);
}

bool Triangle::checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const
{
    float vd = Vector3::dotProduct(normal, dir);

    if (vd >= 0) return false;

    float v0 = -Vector3::dotProduct(normal, eye) + mD;
    float t = v0 / vd;

    if (t < 0) return false;

    Vector3 P = eye + t * dir;


    if (P.x() < xmin || P.x() > xmax || P.y() < ymin || P.y() > ymax || P.z() < zmin || P.z() > zmax)
        return false;


    if (Vector3::dotProduct(Vector3::crossProduct(l0, P - p0), normal) < 0) return false;
    if (Vector3::dotProduct(Vector3::crossProduct(l1, P - p1), normal) < 0) return false;
    if (Vector3::dotProduct(Vector3::crossProduct(l2, P - p2), normal) < 0) return false;


    x = P.x();
    y = P.y();
    z = P.z();

    return true;
}

Vector3 Triangle::getNormal(const Vector3 &P) const
{
    return normal;
}

bool Triangle::isContains(const Vector3 &P) const
{
    return std::abs(Vector3::dotProduct(normal, P) - mD) < 10e-6f;
}
