#include "scenecanvas.h"

#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <cmath>


SceneCanvas::SceneCanvas(QWidget *parent) : Canvas(parent)
{
    Mrot.setToIdentity();
    splines.push_back(BSpline());
}

void SceneCanvas::zeroAngles()
{
    Mrot.setToIdentity();

    should_redraw = true;
    repaint();
}

void SceneCanvas::updateSplines(void *, void *)
{
    figures.resize(splines.size());
    for (int i = 0; i < splines.size(); ++i)
        figures[i].setSpline(splines[i], settings);

    if (settings.sw > settings.sh)
        setMinimumSize(size, size * settings.sh / settings.sw);
    else
        setMinimumSize(size * settings.sw / settings.sh, size);

    should_redraw = true;
    repaint();
}

void SceneCanvas::updateFigures(void *, void *)
{
    if (settings.sw > settings.sh)
        setMinimumSize(size, size * settings.sh / settings.sw);
    else
        setMinimumSize(size * settings.sw / settings.sh, size);

    should_redraw = true;
    repaint();
}

void SceneCanvas::mousePressEvent(QMouseEvent *event)
{
    mouse_pressed = true;

    float posX = event->localPos().x();
    float posY = event->localPos().y();

    mx0 = std::max(std::min(posX, width()-1.0f), 0.0f);
    my0 = std::max(std::min(posY, height()-1.0f), 0.0f);
}

void SceneCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_pressed = false;
}

void SceneCanvas::mouseMoveEvent(QMouseEvent *event)
{
    float posX = event->localPos().x();
    float posY = event->localPos().y();

    posX = std::max(std::min(posX, width()-1.0f), 0.0f);
    posY = std::max(std::min(posY, height()-1.0f), 0.0f);

    if (mouse_pressed) {
        float dx = posX - mx0;
        float dy = posY - my0;

        changeRotationMatrix(-dx / 180, dy / 180);

        mx0 = posX;
        my0 = posY;

        should_redraw = true;
        repaint();
    }
}

void SceneCanvas::wheelEvent(QWheelEvent *event)
{
    const static float step = 0.05f;

    if (event->delta() < 0) {
        settings.zn += step;
        settings.zf += step;
    } else {
        if (settings.zn >= step) {
            settings.zn -= step;
            settings.zf -= step;
        }
    }

    should_redraw = true;
    repaint();
}

void SceneCanvas::draw(QPainter &painter)
{
    std::fill((uint *) data, (uint *) data + width()*height(), settings.background_color);

    wh = width() / 2;
    hh = height() / 2;

    Matrix4x4 Mproj = getProjectiveMatrix();
    Matrix4x4 Mcam = getCameraMatrix();
    //Matrix4x4 Mrot = getRotationMatrix();

    Matrix4x4 Mprojcam = Mproj * Mcam * Mrot;


    // i j k
    drawCoordinateSystem(Mprojcam);

    for (auto &figure : figures) {
        Matrix4x4 Mres = Mprojcam * figure.getC();
        drawCoordinateSystem(Mres);
    }


    for (auto &figure : figures) {
        auto &meridians = figure.getMeridians();
        auto &latitudes = figure.getLatitudes();

        uchar r = figure.spline().r();
        uchar g = figure.spline().g();
        uchar b = figure.spline().b();
        Matrix4x4 Mres = Mprojcam * figure.getOwnTransform();

        for (auto &meridian : meridians)
            drawCurve(meridian, Mres, r, g, b);
        for (auto &latitude : latitudes)
            drawCurve(latitude, Mres, r, g, b);
    }


    drawLine(0, 0, width()-1, 0, 0, 0, 0);
    drawLine(0, 0, 0, height()-1, 0, 0, 0);
    drawLine(0, height()-1, width()-1, height()-1, 0, 0, 0);
    drawLine(width()-1, 0, width()-1, height()-1, 0, 0, 0);
}

void SceneCanvas::drawLine3D(int x0, int y0, float z0, int x1, int y1, float z1, uchar r, uchar g, uchar b)
{
    int absdx = std::abs(x1 - x0);
    int absdy = std::abs(y1 - y0);
    int dx2 = absdx * 2;
    int dy2 = absdy * 2;

    bool flag;
    int clip_step;
    int stepX, stepY, errStepX, errStepY;
    int err;
    float z_step;

    if (absdy > absdx) {
        if (x1 < x0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
        }
        flag = true;
        clip_step = (y1-y0 < 0) ? -1 : 1;
        stepX = (y1-y0 < 0) ? -line_length : line_length;
        stepY = 4;
        err = -absdy;
        errStepX = dx2;
        errStepY = dy2;
    } else {
        if (y1 < y0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
        }
        flag = false;
        clip_step = (x1-x0 < 0) ? -1 : 1;
        stepX = (x1-x0 < 0) ? -4 : 4;
        stepY = line_length;
        err = -absdx;
        errStepX = dy2;
        errStepY = dx2;
    }
    z_step = (z1 - z0) / (-err + 1.0f);


    auto *it = data + y0*line_length + x0*4ull;
    for (int i = -err; i >= 0; --i) {
        if (x0 >= 0 && y0 >= 0 && x0 < width() && y0 < height() && z0 >= 0 && z0 <= 1) {
            it[0] = b;
            it[1] = g;
            it[2] = r;
            it[3] = 255;
        }

        if (flag) y0 += clip_step; else x0 += clip_step;
        z0 += z_step;
        it += stepX;

        err += errStepX;
        if (err > 0) {
            err -= errStepY;
            if (flag) ++x0; else ++y0;
            it += stepY;
        }
    }
}

void SceneCanvas::drawCoordinateSystem(const Matrix4x4 &Mres)
{
    float p_data[4][4] = {
        { 0, 0, 0, 1 },
        { 1, 0, 0, 1 },
        { 0, 1, 0, 1 },
        { 0, 0, 1, 1 }
    };

    std::tuple<int, int, float> coords[4];

    for (int i = 0; i < 4; ++i) {
        Matrix4x1 p(p_data[i]);

        Matrix4x1 pproj = Mres * p;
        uniformVector(pproj);
        int x, y;
        float z;
        translateToScreenCoords(pproj, x, y, z);

        coords[i] = std::make_tuple(x, y, z);
    }

    drawLine3D(std::get<0>(coords[0]), std::get<1>(coords[0]), std::get<2>(coords[0]),
               std::get<0>(coords[1]), std::get<1>(coords[1]), std::get<2>(coords[1]),
               255, 0, 0);
    drawLine3D(std::get<0>(coords[0]), std::get<1>(coords[0]), std::get<2>(coords[0]),
               std::get<0>(coords[2]), std::get<1>(coords[2]), std::get<2>(coords[2]),
               0, 255, 0);
    drawLine3D(std::get<0>(coords[0]), std::get<1>(coords[0]), std::get<2>(coords[0]),
               std::get<0>(coords[3]), std::get<1>(coords[3]), std::get<2>(coords[3]),
               0, 0, 255);
}

void SceneCanvas::drawCurve(const QList<Matrix4x1> &dots, const Matrix4x4 &Mres, uchar r, uchar g, uchar b)
{
    if (dots.isEmpty())
        return;

    auto &p0 = dots[0];
    Matrix4x1 p0proj = Mres * p0;
    uniformVector(p0proj);
    int x0, y0;
    float z0;
    translateToScreenCoords(p0proj, x0, y0, z0);

    for (int i = 1; i < dots.size(); ++i) {
        auto &p = dots[i];
        Matrix4x1 pproj = Mres * p;
        uniformVector(pproj);
        int x, y;
        float z;
        translateToScreenCoords(pproj, x, y, z);

        drawLine3D(x0, y0, z0, x, y, z, r, g, b);

        x0 = x;
        y0 = y;
        z0 = z;
    }
}

void SceneCanvas::uniformVector(Matrix4x1 &v)
{
    float *vdata = v.data();
    vdata[0] /= vdata[3];
    vdata[1] /= vdata[3];
    vdata[2] /= vdata[3];
    //vdata[3] = 1;
}

void SceneCanvas::translateToScreenCoords(Matrix4x1 &v, int &x, int &y, float &z)
{
    float *vdata = v.data();
    x = vdata[0] * wh + wh;
    y = -vdata[1] * hh + hh;
    z = vdata[2];
}

Matrix4x4 SceneCanvas::getProjectiveMatrix() const
{
    float sw = settings.sw;
    float sh = settings.sh;
    float zn = settings.zn;
    float zf = settings.zf;

    float data[16] = { 2/sw * zn, 0, 0, 0,
                       0, 2/sh * zn, 0, 0,
                       0, 0, zf / (zf-zn), -zn * zf/(zf-zn),
                       0, 0, 1, 0 };
    return Matrix4x4(data);
}

Matrix4x4 SceneCanvas::getCameraMatrix() const
{
    static Vector3 Pcam(-10, 0, 0);
    static Vector3 Pview(10, 0, 0);
    static Vector3 Vup(0, 1, 0);

    static Vector3 k = (Pview - Pcam).normalized();
    static Vector3 i = Vector3::crossProduct(Vup.normalized(), k);
    static Vector3 j = Vector3::crossProduct(k, i);

    static float ijk_data[16] = { i.x(), i.y(), i.z(), 0,
                                  j.x(), j.y(), j.z(), 0,
                                  k.x(), k.y(), k.z(), 0,
                                  0, 0, 0, 1 };
    static Matrix4x4 ijk(ijk_data);

    static float shift_data[16] = { 1, 0, 0, -Pcam.x(),
                                    0, 1, 0, -Pcam.y(),
                                    0, 0, 1, -Pcam.z(),
                                    0, 0, 0, 1 };
    static Matrix4x4 shift(shift_data);

    static Matrix4x4 Mcam = ijk * shift;

    return Mcam;
}

void SceneCanvas::changeRotationMatrix(float angleX, float angleY)
{
    float x_data[16] = { std::cos(angleX), 0, std::sin(angleX), 0,
                         0, 1, 0, 0,
                         -std::sin(angleX), 0, std::cos(angleX), 0,
                         0, 0, 0, 1 };
    float y_data[16] = { std::cos(angleY), -std::sin(angleY), 0, 0,
                         std::sin(angleY), std::cos(angleY), 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1 };
    Matrix4x4 x(x_data);
    Matrix4x4 y(y_data);

    Mrot = y * x * Mrot;
}
