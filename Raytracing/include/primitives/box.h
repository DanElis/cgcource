#ifndef BOX_H
#define BOX_H

#include <QObject>

#include "matrices.h"
#include "figure.h"
#include <cmath>

class Box : public Figure
{
    Q_OBJECT
public:
    explicit Box(Vector3 L, Vector3 H, QObject *parent = nullptr);

    bool checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const override;
    Vector3 getNormal(const Vector3 &P) const override;

    void correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const override;


private:
    Vector3 L, H;

    bool checkSides(float &tnear, float &tfar, float yc, float dc, float Lc, float Hc) const;
};

#endif // BOX_H
