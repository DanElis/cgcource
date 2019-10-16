#include "bspline.h"

#include <cmath>
#include <QList>
#include <QPair>
#include <QtAlgorithms>

#include "matrices.h"


BSpline::BSpline(QObject *parent) : QObject(parent)
{
    setColors(128 + rand() % 128, 128 + rand() % 128, 128 + rand() % 128);
}

BSpline::BSpline(const BSpline &other)
{
    dots = other.dots;
    bspline_dots = other.bspline_dots;
    length_parts = other.length_parts;

    _length = other._length;
    _max = other._max;
    _a = other._a;
    _b = other._b;
    _begin = other._begin;
    _end = other._end;
    _wh = other._wh;
    _hh = other._hh;
    c_r = other.c_r;
    c_g = other.c_g;
    c_b = other.c_b;
    k = other.k;
}

BSpline::BSpline(BSpline &&other)
{
    dots = std::move(other.dots);
    bspline_dots = std::move(other.bspline_dots);
    length_parts = std::move(other.length_parts);

    _length = other._length;
    _max = other._max;
    _a = other._a;
    _b = other._b;
    _begin = other._begin;
    _end = other._end;
    _wh = other._wh;
    _hh = other._hh;
    c_r = other.c_r;
    c_g = other.c_g;
    c_b = other.c_b;
    k = other.k;
}

BSpline &BSpline::operator=(const BSpline &other)
{
    dots = other.dots;
    bspline_dots = other.bspline_dots;
    length_parts = other.length_parts;

    _length = other._length;
    _max = other._max;
    _a = other._a;
    _b = other._b;
    _begin = other._begin;
    _end = other._end;
    _wh = other._wh;
    _hh = other._hh;
    c_r = other.c_r;
    c_g = other.c_g;
    c_b = other.c_b;
    k = other.k;

    return *this;
}

BSpline &BSpline::operator=(BSpline &&other)
{
    dots = std::move(other.dots);
    bspline_dots = std::move(other.bspline_dots);
    length_parts = std::move(other.length_parts);

    _length = other._length;
    _max = other._max;
    _a = other._a;
    _b = other._b;
    _begin = other._begin;
    _end = other._end;
    _wh = other._wh;
    _hh = other._hh;
    c_r = other.c_r;
    c_g = other.c_g;
    c_b = other.c_b;
    k = other.k;

    return *this;
}

void BSpline::addLast(float x, float y)
{
    dots.append(qMakePair(x, y));
    if (abs(x - _wh) > _max) _max = abs(x - _wh);
    if (abs(y - _hh) > _max) _max = abs(y - _hh);

    if (dots.size() >= 4)
        build_spline_part(dots[dots.size()-4], dots[dots.size()-3], dots[dots.size()-2], dots[dots.size()-1]);

    recalculate();
}

void BSpline::add(int i, float x, float y)
{
    if (i < 0 || i > dots.size()) return;

    dots.insert(i, qMakePair(x, y));
    rebuild();
}

void BSpline::removeLast()
{
    if (!dots.isEmpty()) {
        if (dots.size() >= 4) {
            for (int i = 0; i <= k; ++i) {
                bspline_dots.pop_back();
                length_parts.pop_back();
            }

            _length = length_parts.isEmpty() ? 0 : length_parts.last();
        }

        dots.pop_back();
        _max = 0;
        for (auto &d : dots) {
            if (abs(d.first - _wh) > _max) _max = abs(d.first - _wh);
            if (abs(d.second - _hh) > _max) _max = abs(d.second - _hh);
        }

        recalculate();
    }
}

void BSpline::remove(int i)
{
    if (i < 0 || i >= dots.size()) return;

    dots.removeAt(i);
    rebuild();
}

void BSpline::replace(int i, float x, float y)
{
    if (i < 0 || i > dots.size()) return;

    dots[i] = qMakePair(x, y);
    rebuild();
}

float BSpline::max() const
{
    return _max;
}

float BSpline::length() const
{
    return _length;
}

int BSpline::wh() const
{
    return _wh;
}

int BSpline::hh() const
{
    return _hh;
}

const QList<Dot> &BSpline::skeleton() const
{
    return dots;
}

const QList<Dot> &BSpline::bspline() const
{
    return bspline_dots;
}

int BSpline::splineBegin() const
{
    return _begin;
}

int BSpline::splineEnd() const
{
    return _end;
}

void BSpline::changeLimits(float a, float b)
{
    this->_a = a;
    this->_b = b;
    recalculate();
}

void BSpline::setColors(uchar r, uchar g, uchar b)
{
    this->c_r = r;
    this->c_g = g;
    this->c_b = b;
}

void BSpline::setSizes(int wh, int hh)
{
    this->_wh = wh;
    this->_hh = hh;
}

uchar BSpline::r() const
{
    return c_r;
}

uchar BSpline::g() const
{
    return c_g;
}

uchar BSpline::b() const
{
    return c_b;
}


#define dot(d) d.first, d.second

void BSpline::build_spline_part(const Dot &p1, const Dot &p2, const Dot &p3, const Dot &p4)
{
    static float ms_data[16] = { -1.0f/6,  3.0f/6, -3.0f/6,  1.0f/6,
                                  3.0f/6, -6.0f/6,  3.0f/6,  0.0f/6,
                                 -3.0f/6,  0.0f/6,  3.0f/6,  0.0f/6,
                                  1.0f/6,  4.0f/6,  1.0f/6,  0.0f/6 };
    static Matrix4x4 Ms(ms_data);


    float gs_data[8] = { dot(p1), dot(p2), dot(p3), dot(p4) };
    Matrix4x2 Gs(gs_data);


    float t = 0;
    float step = 1.0f / k;
    float x0, y0;

    for (int i = 0; i <= k; ++i, t += step) {
        float data[4] = { t*t*t, t*t, t, 1.0f };
        Matrix1x4 T(data);

        Matrix1x2 result = T * Ms * Gs;

        float *xy = result.data();
        bspline_dots.push_back(qMakePair(xy[0], xy[1]));

        if (i > 0) {
            float len = std::sqrt(std::pow(xy[0] - x0, 2.0f) + std::pow(xy[1] - y0, 2.0f));
            _length += len;
        }
        length_parts.push_back(_length);
        x0 = xy[0];
        y0 = xy[1];
    }
}

void BSpline::rebuild()
{
    bspline_dots.clear();
    _length = 0;
    length_parts.clear();

    for (int i = 0; i < dots.size()-3; ++i)
        build_spline_part(dots[i], dots[i+1], dots[i+2], dots[i+3]);

    recalculate();

    _max = 0;
    for (auto &d : dots) {
        if (abs(d.first - _wh) > _max) _max = abs(d.first - _wh);
        if (abs(d.second - _hh) > _max) _max = abs(d.second - _hh);
    }
}

void BSpline::recalculate()
{
    if (dots.size() < 4) return;

    float la = _a * _length;
    float lb = _b * _length;

    auto la_it = std::lower_bound(length_parts.begin(), length_parts.end(), la);
    auto lb_it = std::upper_bound(length_parts.begin(), length_parts.end(), lb);

    if (la_it == length_parts.begin() && abs(*la_it - la) < 10e-8)
        _begin = -1;
    else {
        if (abs(*la_it - la) < 10e-8)
            --la_it;
        _begin = la_it - length_parts.begin();
    }
    if (lb_it == length_parts.begin() && abs(*lb_it - lb) < 10e-8)
        _end = -1;
    else {
        if (abs(*lb_it - lb) < 10e-8)
            ++lb_it;
        _end = lb_it - length_parts.begin();
    }
}
