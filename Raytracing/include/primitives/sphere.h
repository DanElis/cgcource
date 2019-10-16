#ifndef SPHERE_H
#define SPHERE_H

#include <QObject>

#include "matrices.h"
#include "figure.h"


class Sphere : public Figure
{
    Q_OBJECT
public:
    explicit Sphere(Vector3 C, float r, QObject *parent = nullptr);
    void correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const override;
    bool checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const override;
    Vector3 getNormal(const Vector3 &P) const override;


private:
    Vector3 C;
    float r,r2;
    const int details = 20;
};

#endif // SPHERE_H
