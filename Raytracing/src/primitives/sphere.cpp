#include "primitives/sphere.h"

#include <cmath>


Sphere::Sphere(Vector3 C, float r, QObject *parent)
    : Figure(parent),
      C(std::move(C)),
      r(r), r2(r*r)
{
    wireframe.resize(2*details + 2);

    float angle_step = 2*M_PI / details;
    float angle = 0;

    for (int im = 0; im <= details; ++im, angle += angle_step) {
        float d2tod3_data[16] = { 0, r * std::cos(angle), 0, C.x(),
                                  0, r * std::sin(angle), 0, C.y(),
                                  r, 0, 0, C.z(),
                                  0, 0, 0, 1 };
        Matrix4x4 d2tod3(d2tod3_data);


        auto &w = wireframe[im];


        float angle2 = 0;

        for (int in = 0; in <= details; ++in, angle2 += angle_step) {
            float data[4] = { std::cos(angle2), std::sin(angle2), 0, 1 };
            Matrix4x1 dot2d(data);

            Matrix4x1 dot3d = d2tod3 * dot2d;

            w.push_back(dot3d);
            wireframe[details + 1 + in].push_back(dot3d);
        }
    }
}

bool Sphere::checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const
{
    Vector3 OC = C - eye;
    float OC2 = Vector3::dotProduct(OC, OC);

    if (OC2 < r2 - 10e-9f)
        return false;

    float tsv = Vector3::dotProduct(OC, dir);
    if (tsv < 0)
        return false;

    float thc2 = r2 - OC2 + tsv*tsv;
    if (thc2 < 0)
        return false;

    float t = tsv - std::sqrt(thc2);


    Vector3 P = eye + t * dir;
    x = P.x();
    y = P.y();
    z = P.z();

    return true;
}

Vector3 Sphere::getNormal(const Vector3 &P) const
{
    return (P - C).normalized();
}

void Sphere::correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const
{
    xmin = std::min(xmin, C.x() - r);
    xmax = std::max(xmax, C.x() + r);
    ymin = std::min(ymin, C.y() - r);
    ymax = std::max(ymax, C.y() + r);
    zmin = std::min(zmin, C.z() - r);
    zmax = std::max(zmax, C.z() + r);
}
