#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QObject>

#include "matrices.h"
#include "figure.h"
#include <cmath>

class Triangle : public Figure
{
    Q_OBJECT
public:
    explicit Triangle(Vector3 p0, Vector3 p1, Vector3 p2, QObject *parent = nullptr);
    void correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const override;
    bool checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const override;
    Vector3 getNormal(const Vector3 &P) const override;

    bool isContains(const Vector3 &P) const;


private:
    Vector3 p0, p1, p2;
    Vector3 l0, l1, l2;
    Vector3 normal;
    float mD;

    float xmin, xmax, ymin, ymax, zmin, zmax;
};

#endif // TRIANGLE_H
