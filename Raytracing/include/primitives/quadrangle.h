#ifndef QUADRANGLE_H
#define QUADRANGLE_H

#include <QObject>

#include "matrices.h"
#include "figure.h"

#include "triangle.h"


class Quadrangle : public Figure
{
    Q_OBJECT
public:
    explicit Quadrangle(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3, QObject *parent = nullptr);
    void correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const override;
    bool checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const override;
    Vector3 getNormal(const Vector3 &P) const override;


private:
    Triangle t1, t2;
};

#endif // QUADRANGLE_H
