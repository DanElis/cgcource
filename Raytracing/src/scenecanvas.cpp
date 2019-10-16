#include "scenecanvas.h"

#include <QThread>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include <QDebug>

#include <cmath>

#include "raytracing.h"


SceneCanvas::SceneCanvas(QWidget *parent) : Canvas(parent)
{
}



void SceneCanvas::init()
{
    if (settings.deny)
        return;


    float xmin, xmax, ymin, ymax, zmin, zmax;

    xmin = ymin = zmin = std::numeric_limits<float>::max();
    xmax = ymax = zmax = -xmin;

    for (auto &figure : settings.figures)
        figure->correctBox(xmin, xmax, ymin, ymax, zmin, zmax);

    if (xmin > xmax) xmin = xmax = 0;
    if (ymin > ymax) ymin = ymax = 0;
    if (zmin > zmax) zmin = zmax = 0;


    float centerx = (xmax + xmin) / 2;
    float centery = (ymax + ymin) / 2;
    float centerz = (zmax + zmin) / 2;
    xmin = centerx + 1.05f * (xmin - centerx);
    xmax = centerx + 1.05f * (xmax - centerx);
    ymin = centery + 1.05f * (ymin - centery);
    ymax = centery + 1.05f * (ymax - centery);
    zmin = centerz + 1.05f * (zmin - centerz);
    zmax = centerz + 1.05f * (zmax - centerz);


    settings.should_init = false;
    settings.gamma = 1;
    settings.depth = 2;
    settings.quality = Settings::Normal;
    settings.view = Vector3(centerx, centery, centerz);
    settings.up = Vector3(0, 0, 1);
    settings.eye = Vector3(xmin - std::sqrt(3.0f) * std::max(centery - ymin, centerz - zmin), centery, centerz);
    settings.zn = (xmin - settings.eye.x()) / 2;
    settings.zf = xmax - settings.eye.x() + centerx;
    settings.sh = std::max(centery - ymin, centerz - zmin);
    double ratio = (width() + 0.0f) / height();
    settings.sw = settings.sh * ratio;


    mode = Wireframe;

    should_redraw = true;
    repaint();
}

void SceneCanvas::wireframeMode()
{
    mode = Wireframe;

    should_redraw = true;
    repaint();
}

void SceneCanvas::renderScene()
{
    mode = Render;

    auto *thread = QThread::create([this]() {
        Raytracing raytracing(settings, width(), height());
        connect(&raytracing, SIGNAL(renderingProgress(int)), this, SIGNAL(renderingProgress(int)));

        auto *data = raytracing.startRaytracing();

        auto *data_it = data;
        auto *it = image.bits();
        for (int i = 0; i < width() * height(); ++i, data_it += 3, it += 4) {
            it[0] = (uchar) (data_it[0] * 255 + 0.5);
            it[1] = (uchar) (data_it[1] * 255 + 0.5);
            it[2] = (uchar) (data_it[2] * 255 + 0.5);
            it[3] = 255;
        }

        delete[] data;
    });

    connect(thread, SIGNAL(finished()), this, SLOT(repaint()));
    connect(thread, SIGNAL(finished()), this, SIGNAL(renderingDone()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

bool SceneCanvas::saveImage(const QString &name)
{
    return image.save(name);
}

void SceneCanvas::goLeft()
{
    movement(-0.1f, 0);
    should_redraw = true;
    repaint();
}

void SceneCanvas::goRight()
{
    movement(0.1f, 0);
    should_redraw = true;
    repaint();
}

void SceneCanvas::goUp()
{
    movement(0, 0.1f);
    should_redraw = true;
    repaint();
}

void SceneCanvas::goDown()
{
    movement(0, -0.1f);
    should_redraw = true;
    repaint();
}



void SceneCanvas::settingsChanged(void *_settings)
{
    settings = *((Settings *) _settings);

    if (settings.should_init)
        init();
    else {
        double ratio = (width() + 0.0f) / height();
        if (std::abs(settings.sw / settings.sh - ratio) > 10e-9)
            settings.sw = settings.sh * ratio;

        should_redraw = true;
        repaint();
    }
}



// Mouse
void SceneCanvas::mousePressEvent(QMouseEvent *event)
{
    if (mode != Wireframe)
        return;

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
    if (mode != Wireframe)
        return;

    float posX = event->localPos().x();
    float posY = event->localPos().y();

    posX = std::max(std::min(posX, width()-1.0f), 0.0f);
    posY = std::max(std::min(posY, height()-1.0f), 0.0f);

    if (mouse_pressed) {
        float dx = posX - mx0;
        float dy = posY - my0;

        changeRotationMatrix(dy / 360, dx / 360);

        mx0 = posX;
        my0 = posY;

        should_redraw = true;
        repaint();
    }
}

void SceneCanvas::wheelEvent(QWheelEvent *event)
{
    if (mode != Wireframe)
        return;


    if (QApplication::keyboardModifiers() & Qt::CTRL) {
        const static float step = 0.001f;

        Vector3 k = (event->delta() * step) * (settings.view - settings.eye).normalized();

        settings.eye += k;
        settings.view += k;
    } else {
        const static float step = 0.98f;

        if (event->delta() < 0) {
            settings.zn *= step;
            settings.zf *= step;
        } else {
            settings.zn /= step;
            settings.zf /= step;
        }
    }

    should_redraw = true;
    repaint();
}



// Drawing
void SceneCanvas::draw(QPainter &painter)
{
    if (mode != Wireframe)
        return;

    std::fill((uint *) data, (uint *) data + width()*height(), 0xFFFFFFFF);

    if (settings.deny)
        return;

    wh = width() / 2;
    hh = height() / 2;

    Matrix4x4 Mproj = getProjectiveMatrix();
    Matrix4x4 Mcam = getCameraMatrix();
    Matrix4x4 Mshift = getShiftMatrix();

    Matrix4x4 Mprojcam = Mproj * Mcam;
    Matrix4x4 Mres = Mprojcam * Mshift;


    drawCoordinateSystem(Mprojcam);


    for (auto &figure : settings.figures) {
        auto &wireframe = figure->getWireframe();

        for (auto &curve : wireframe)
            drawCurve(curve, Mres, 128, 128, 128);
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

    int xl = x0, yl = y0;
    float zl = z0;

    for (int i = 1; i < dots.size(); ++i) {
        auto &p = dots[i];
        Matrix4x1 pproj = Mres * p;
        uniformVector(pproj);
        int x, y;
        float z;
        translateToScreenCoords(pproj, x, y, z);

        drawLine3D(xl, yl, zl, x, y, z, r, g, b);

        xl = x;
        yl = y;
        zl = z;
    }

    drawLine3D(x0, y0, z0, xl, yl, zl, r, g, b);
}



// Matrices
void SceneCanvas::uniformVector(Matrix4x1 &v)
{
    float *vdata = v.data();

    if (vdata[3] < 10e-9f) {
        vdata[2] = -std::numeric_limits<float>::min();
        return;
    }

    vdata[0] /= vdata[3];
    vdata[1] /= vdata[3];
    vdata[2] /= vdata[3];
    //vdata[3] = 1;
}

void SceneCanvas::translateToScreenCoords(Matrix4x1 &v, int &x, int &y, float &z) const
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

Matrix4x4 SceneCanvas::getCameraMatrix()
{
    Vector3 forward = settings.view - settings.eye;

    Vector3 k = forward.normalized();
    Vector3 i = Vector3::crossProduct(k, settings.up).normalized();
    Vector3 j = Vector3::crossProduct(i, k);

    settings.up = j;


    float ijk_data[16] = { i.x(), i.y(), i.z(), 0,
                           j.x(), j.y(), j.z(), 0,
                           k.x(), k.y(), k.z(), 0,
                           0, 0, 0, 1 };
    Matrix4x4 ijk(ijk_data);


    float shift_data[16] = { 1, 0, 0, forward.x(),
                             0, 1, 0, forward.y(),
                             0, 0, 1, forward.z(),
                             0, 0, 0, 1 };
    Matrix4x4 shift(shift_data);


    return ijk * shift;
}

Matrix4x4 SceneCanvas::getShiftMatrix() const
{
    float shift2_data[16] = { 1, 0, 0, -settings.view.x(),
                              0, 1, 0, -settings.view.y(),
                              0, 0, 1, -settings.view.z(),
                              0, 0, 0, 1 };
    return Matrix4x4(shift2_data);
}

void SceneCanvas::changeRotationMatrix(float angleX, float angleY)
{
    float x_data[16] = { 1, 0, 0, 0,
                         0, std::cos(angleX), -std::sin(angleX), 0,
                         0, std::sin(angleX), std::cos(angleX), 0,
                         0, 0, 0, 1 };
    float y_data[16] = { std::cos(angleY), 0, std::sin(angleY), 0,
                         0, 1, 0, 0,
                         -std::sin(angleY), 0, std::cos(angleY), 0,
                         0, 0, 0, 1 };
    Matrix4x4 x(x_data);
    Matrix4x4 y(y_data);


    Vector3 forward = settings.eye - settings.view;

    Vector3 k = -forward.normalized();
    Vector3 i = Vector3::crossProduct(k, settings.up).normalized();
    Vector3 j = Vector3::crossProduct(i, k);

    float ijk_data[16] = { i.x(), i.y(), i.z(), 0,
                           j.x(), j.y(), j.z(), 0,
                           k.x(), k.y(), k.z(), 0,
                           0, 0, 0, 1 };
    Matrix4x4 ijk(ijk_data);


    Matrix4x1 rotated_m = ijk.transposed() * y * x * ijk * convertVector3(forward);
    Vector3 rotated = convertMatrix4x1(rotated_m);

    settings.eye = rotated + settings.view;
}

void SceneCanvas::movement(float x, float y)
{
    Vector3 k = (settings.view - settings.eye).normalized();
    Vector3 i = Vector3::crossProduct(k, settings.up).normalized();
    Vector3 j = Vector3::crossProduct(i, k);

    settings.eye += x*i + y*j;
    settings.view += x*i + y*j;
}
