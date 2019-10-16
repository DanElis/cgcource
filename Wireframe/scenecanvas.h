#ifndef SCENECANVAS_H
#define SCENECANVAS_H

#include "canvas.h"

#include "settings.h"
#include "bspline.h"
#include "figure.h"
#include "matrices.h"


class SceneCanvas : public Canvas
{
    Q_OBJECT
public:
    explicit SceneCanvas(QWidget *parent = nullptr);

    Matrix4x4 Mrot;

    QList<BSpline> splines;
    QVector<Figure> figures;
    Settings settings;

    void zeroAngles();


public slots:
    void updateSplines(void *settings, void *splines);
    void updateFigures(void *settings, void *);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void draw(QPainter &painter) override;

    void drawLine3D(int x0, int y0, float z0, int x1, int y1, float z1, uchar r, uchar g, uchar b);


private:
    int size = 800;
    bool mouse_pressed = false;
    float mx0, my0;

    int wh, hh;

    void drawCoordinateSystem(const Matrix4x4 &Mres);
    void drawCurve(const QList<Matrix4x1> &dots, const Matrix4x4 &Mres, uchar r, uchar g, uchar b);

    inline void uniformVector(Matrix4x1 &v);
    inline void translateToScreenCoords(Matrix4x1 &v, int &x, int &y, float &z);


    Matrix4x4 getProjectiveMatrix() const;
    Matrix4x4 getCameraMatrix() const;

    void changeRotationMatrix(float angleX, float angleY);
};

#endif // SCENECANVAS_H
