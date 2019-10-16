#include "figure.h"

#include "settings.h"

#include <cmath>


Figure::Figure(QObject *parent)
    : QObject(parent)
{
}

const QVector<QList<Matrix4x1> > &Figure::getWireframe() const
{
    return wireframe;
}

void Figure::setLightParameters(float KDr, float KDg, float KDb, float KSr, float KSg, float KSb, float power)
{
    this->KDr = KDr;
    this->KDg = KDg;
    this->KDb = KDb;
    this->KSr = KSr;
    this->KSg = KSg;
    this->KSb = KSb;
    this->power = power;
}

Vector3 Figure::reflectRay(const Vector3 &dir, const Vector3 &P) const
{
    auto normal = getNormal(P);
    return 2.0f * Vector3::dotProduct(normal, -dir) * normal + dir;
}

