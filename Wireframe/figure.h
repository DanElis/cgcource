#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>

#include "settings.h"
#include "bspline.h"
#include "matrices.h"


class Figure : public QObject
{
    Q_OBJECT
public:
    explicit Figure(QObject *parent = nullptr);
    Figure(const Figure &other);
    Figure(Figure &&other);
    Figure& operator=(const Figure &other);
    Figure& operator=(Figure &&other);

    void setSpline(BSpline &spline, Settings &settings);

    const QVector< QList<Matrix4x1> >& getMeridians() const;
    const QVector< QList<Matrix4x1> >& getLatitudes() const;
    Matrix4x4 getOwnTransform() const;
    const BSpline& spline() const;

    const Matrix4x4& getC() const;
    void getC(float& x, float& y, float& z) const;
    void setC(float x, float y, float z);
    const Matrix4x4& getR() const;
    void getR(float& x, float& y, float& z) const;
    void setR(float x, float y, float z);
    void setR(float *data);
    void getS(float& x, float& y, float& z) const;
    void setS(float x, float y, float z);


private:
    BSpline *_spline = nullptr;

    QVector< QList<Matrix4x1> > meridians;
    QVector< QList<Matrix4x1> > latitudes;

    float rx, ry, rz;
    Matrix4x4 C, R, S, scale;

    void applyScaleMatrix(float max);
};

#endif // FIGURE_H
