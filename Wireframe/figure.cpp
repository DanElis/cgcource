#include "figure.h"

#include <cmath>


Figure::Figure(QObject *parent)
    : QObject(parent)
{
    S.setToIdentity();
    scale.setToIdentity();
    C.setToIdentity();

    setR((rand() % 361 - 180) * M_PI / 180, (rand() % 361 - 180) * M_PI / 180, (rand() % 361 - 180) * M_PI / 180);

    float x = (rand() % 201 - 100) / 100.0f;
    float y = (rand() % 201 - 100) / 100.0f;
    float z = (rand() % 201 - 100) / 100.0f;
    if (x < 0) x -= 1; else x += 1;
    if (y < 0) y -= 1; else y += 1;
    if (z < 0) z -= 1; else z += 1;
    setC(x, y, z);
}

Figure::Figure(const Figure &other)
{
    meridians = other.meridians;
    latitudes = other.latitudes;

    C = other.C;
    R = other.R;
    S = other.S;
    scale = other.scale;

    _spline = other._spline;
    rx = other.rx;
    ry = other.ry;
    rz = other.rz;
}

Figure::Figure(Figure &&other)
{
    meridians = std::move(other.meridians);
    latitudes = std::move(other.latitudes);

    C = std::move(other.C);
    R = std::move(other.R);
    S = std::move(other.S);
    scale = std::move(other.scale);

    _spline = other._spline;
    rx = other.rx;
    ry = other.ry;
    rz = other.rz;
}

Figure &Figure::operator=(const Figure &other)
{
    meridians = other.meridians;
    latitudes = other.latitudes;

    C = other.C;
    R = other.R;
    S = other.S;
    scale = other.scale;

    _spline = other._spline;
    rx = other.rx;
    ry = other.ry;
    rz = other.rz;

    return *this;
}

Figure &Figure::operator=(Figure &&other)
{
    meridians = std::move(other.meridians);
    latitudes = std::move(other.latitudes);

    C = std::move(other.C);
    R = std::move(other.R);
    S = std::move(other.S);
    scale = std::move(other.scale);

    _spline = other._spline;
    rx = other.rx;
    ry = other.ry;
    rz = other.rz;

    return *this;
}

void Figure::setSpline(BSpline &spline, Settings &settings)
{
    latitudes.clear();
    meridians.clear();

    _spline = &spline;

    auto &spline_dots = spline.bspline();

    if (spline_dots.isEmpty())
        return;


    applyScaleMatrix(spline.max());


    auto wh = spline.wh();
    auto hh = spline.hh();
    int n = settings.n * settings.k;
    int m = settings.m * settings.k;
    float step = (spline_dots.size() - 1.0f) / n;
    float angle_step = (settings.d - settings.c) / m;

    latitudes.resize(settings.n + 1);
    meridians.resize(settings.m + 1);


    float angle = settings.c;
    for (int im = 0; im <= m; ++im, angle += angle_step) {
        bool flag = (im % settings.k == 0);

        float d2tod3_data[16] = { 0, std::cos(angle), 0, 0,
                                  0, std::sin(angle), 0, 0,
                                  1, 0, 0, 0,
                                  0, 0, 0, 1 };
        Matrix4x4 d2tod3(d2tod3_data);


        float it = 0;
        for (int in = 0; in <= n; ++in, it += step) {
            if (!flag && in % settings.k)
                continue;

            auto d = spline_dots[(int) it];

            float data[4] = { d.first - wh, d.second - hh, 0, 1 };
            Matrix4x1 dot2d(data);

            Matrix4x1 dot3d = d2tod3 * dot2d;


            if (flag)
                meridians[im / settings.k].push_back(dot3d);
            if (in % settings.k == 0)
                latitudes[in / settings.k].push_back(dot3d);
        }
    }
}

const QVector<QList<Matrix4x1> > &Figure::getMeridians() const
{
    return meridians;
}

const QVector<QList<Matrix4x1> > &Figure::getLatitudes() const
{
    return latitudes;
}

Matrix4x4 Figure::getOwnTransform() const
{
    return C * R * S * scale;
}

const BSpline &Figure::spline() const
{
    return *_spline;
}

const Matrix4x4 &Figure::getC() const
{
    return C;
}

void Figure::getC(float &x, float &y, float &z) const
{
    x = C.data()[12];
    y = C.data()[13];
    z = C.data()[14];
}

void Figure::setC(float x, float y, float z)
{
    float data[16] = { 1, 0, 0, x,
                       0, 1, 0, y,
                       0, 0, 1, z,
                       0, 0, 0, 1 };
    C = Matrix4x4(data);
}

const Matrix4x4 &Figure::getR() const
{
    return R;
}

void Figure::getR(float &x, float &y, float &z) const
{
    x = rx;
    y = ry;
    z = rz;
}

void Figure::setR(float x, float y, float z)
{
    rx = x;
    ry = y;
    rz = z;

    float x_data[16] = { 1, 0, 0, 0,
                         0, std::cos(x), -std::sin(x), 0,
                         0, std::sin(x),  std::cos(x), 0,
                         0, 0, 0, 1 };
    float y_data[16] = { std::cos(y), 0, std::sin(y), 0,
                         0, 1, 0, 0,
                         -std::sin(y), 0, std::cos(y), 0,
                         0, 0, 0, 1 };
    float z_data[16] = { std::cos(z), -std::sin(z), 0, 0,
                         std::sin(z), std::cos(z), 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1 };
    Matrix4x4 _x(x_data);
    Matrix4x4 _y(y_data);
    Matrix4x4 _z(z_data);

    R = _z * _y * _x;
}

void Figure::setR(float *data)
{
    R = Matrix4x4(data);
}

void Figure::getS(float &x, float &y, float &z) const
{
    x = S.data()[0];
    y = S.data()[5];
    z = S.data()[10];
}

void Figure::setS(float x, float y, float z)
{
    float data[16] = { x, 0, 0, 0,
                       0, y, 0, 0,
                       0, 0, z, 0,
                       0, 0, 0, 1 };
    S = Matrix4x4(data);
}

void Figure::applyScaleMatrix(float max)
{
    float data[16] = { 1/max, 0, 0, 0,
                       0, 1/max, 0, 0,
                       0, 0, 1/max, 0,
                       0, 0, 0, 1 };
    scale = Matrix4x4(data);
}
