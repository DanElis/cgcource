#include "primitives/box.h"


Box::Box(Vector3 L, Vector3 H, QObject *parent)
    : Figure(parent),
      L(std::move(L)),
      H(std::move(H))
{
    wireframe.resize(6);

    wireframe[0].push_back(convertVector3(Vector3(L)));
    wireframe[0].push_back(convertVector3(Vector3(H.x(), L.y(), L.z())));
    wireframe[0].push_back(convertVector3(Vector3(H.x(), H.y(), L.z())));
    wireframe[0].push_back(convertVector3(Vector3(L.x(), H.y(), L.z())));

    wireframe[1].push_back(convertVector3(Vector3(L)));
    wireframe[1].push_back(convertVector3(Vector3(H.x(), L.y(), L.z())));
    wireframe[1].push_back(convertVector3(Vector3(H.x(), L.y(), H.z())));
    wireframe[1].push_back(convertVector3(Vector3(L.x(), L.y(), H.z())));

    wireframe[2].push_back(convertVector3(Vector3(L)));
    wireframe[2].push_back(convertVector3(Vector3(L.x(), H.y(), L.z())));
    wireframe[2].push_back(convertVector3(Vector3(L.x(), H.y(), H.z())));
    wireframe[2].push_back(convertVector3(Vector3(L.x(), L.y(), H.z())));

    wireframe[3].push_back(convertVector3(Vector3(H)));
    wireframe[3].push_back(convertVector3(Vector3(L.x(), H.y(), H.z())));
    wireframe[3].push_back(convertVector3(Vector3(L.x(), L.y(), H.z())));
    wireframe[3].push_back(convertVector3(Vector3(H.x(), L.y(), H.z())));

    wireframe[4].push_back(convertVector3(Vector3(H)));
    wireframe[4].push_back(convertVector3(Vector3(L.x(), H.y(), H.z())));
    wireframe[4].push_back(convertVector3(Vector3(L.x(), H.y(), L.z())));
    wireframe[4].push_back(convertVector3(Vector3(H.x(), H.y(), L.z())));

    wireframe[5].push_back(convertVector3(Vector3(H)));
    wireframe[5].push_back(convertVector3(Vector3(H.x(), L.y(), H.z())));
    wireframe[5].push_back(convertVector3(Vector3(H.x(), L.y(), L.z())));
    wireframe[5].push_back(convertVector3(Vector3(H.x(), H.y(), L.z())));
}

bool Box::checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const
{
    float tnear = std::numeric_limits<float>::min();
    float tfar = std::numeric_limits<float>::max();

    if (!(checkSides(tnear, tfar, eye.x(), dir.x(), L.x(), H.x()) &&
          checkSides(tnear, tfar, eye.y(), dir.y(), L.y(), H.y()) &&
          checkSides(tnear, tfar, eye.z(), dir.z(), L.z(), H.z())))
    {
        return false;
    }

    Vector3 P = eye + tnear * dir;
    x = P.x();
    y = P.y();
    z = P.z();

    return true;
}

Vector3 Box::getNormal(const Vector3 &P) const
{
    if (std::abs(P.x() - L.x()) < 10e-6f) return Vector3(-1, 0, 0);
    if (std::abs(P.x() - H.x()) < 10e-6f) return Vector3( 1, 0, 0);
    if (std::abs(P.y() - L.y()) < 10e-6f) return Vector3(0, -1, 0);
    if (std::abs(P.y() - H.y()) < 10e-6f) return Vector3(0,  1, 0);
    if (std::abs(P.z() - L.z()) < 10e-6f) return Vector3(0, 0, -1);
    if (std::abs(P.z() - H.z()) < 10e-6f) return Vector3(0, 0,  1);

    qDebug() << L << P << H;
    exit(228);
}

void Box::correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const
{
    xmin = std::min(xmin, L.x());
    xmax = std::max(xmax, H.x());
    ymin = std::min(ymin, L.y());
    ymax = std::max(ymax, H.y());
    zmin = std::min(zmin, L.z());
    zmax = std::max(zmax, H.z());
}

bool Box::checkSides(float &tnear, float &tfar, float yc, float dc, float Lc, float Hc) const
{
    if (std::abs(dc) < 10e-6f)
        return yc >= Lc && yc <= Hc;

    float t1 = (Lc - yc) / dc;
    float t2 = (Hc - yc) / dc;

    if (t2 < t1) std::swap(t1, t2);

    if (t1 > tnear) tnear = t1;
    if (t2 < tfar) tfar = t2;

    if (tnear > tfar || tfar < 0) return false;

    return true;
}
