#ifndef BSPLINE_H
#define BSPLINE_H

#include <QObject>

#include "settings.h"


typedef QPair<float, float> Dot;


class BSpline : public QObject
{
    Q_OBJECT
public:
    explicit BSpline(QObject *parent = nullptr);
    BSpline(const BSpline &other);
    BSpline(BSpline &&other);

    BSpline& operator=(const BSpline &other);
    BSpline& operator=(BSpline &&other);

    void addLast(float x, float y);
    void add(int i, float x, float y);
    void removeLast();
    void remove(int i);
    void replace(int i, float x, float y);

    float max() const;
    float length() const;
    int wh() const;
    int hh() const;

    const QList<Dot>& skeleton() const;
    const QList<Dot>& bspline() const;
    int splineBegin() const;
    int splineEnd() const;

    void changeLimits(float a, float b);
    void setColors(uchar r, uchar g, uchar b);
    void setSizes(int wh, int hh);

    uchar r() const;
    uchar g() const;
    uchar b() const;


private:
    QList<Dot> dots;
    QList<Dot> bspline_dots;
    QList<float> length_parts;
    float _length = 0;

    float _max = 0;
    float _a = 0, _b = 1;
    int _begin = 0, _end = 0;
    int _wh, _hh;
    uchar c_r, c_g, c_b;

    int k = 100;

    void build_spline_part(const Dot &p1, const Dot &p2, const Dot &p3, const Dot &p4);
    void rebuild();
    void recalculate();
};

#endif // BSPLINE_H
