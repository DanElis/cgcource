#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>

#include "matrices.h"
class Settings;


class Figure : public QObject
{
    Q_OBJECT
public:
    explicit Figure(QObject *parent = nullptr);


    const QVector< QList<Matrix4x1> >& getWireframe() const;

    void setLightParameters(float KDr, float KDg, float KDb, float KSr, float KSg, float KSb, float power);

    virtual bool checkIntersect(const Vector3 &eye, const Vector3 &dir, float &x, float &y, float &z) const = 0;
    virtual Vector3 getNormal(const Vector3 &P) const = 0;
    virtual Vector3 reflectRay(const Vector3 &dir, const Vector3 &P) const;

    virtual void correctBox(float &xmin, float &xmax, float &ymin, float &ymax, float &zmin, float &zmax) const = 0;



public:
    float KDr, KDg, KDb, KSr, KSg, KSb, power;

protected:
    QVector< QList<Matrix4x1> > wireframe;
};

#endif // FIGURE_H
