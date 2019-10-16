#ifndef SCENECANVAS_H
#define SCENECANVAS_H

#include "canvas.h"

#include "settings.h"
#include "figure.h"
#include "matrices.h"


class SceneCanvas : public Canvas
{
    Q_OBJECT
public:
    explicit SceneCanvas(QWidget *parent = nullptr);

    Settings settings;

    void init();
    void wireframeMode();
    void renderScene();
    bool saveImage(const QString &name);

    void goLeft();
    void goRight();
    void goUp();
    void goDown();

signals:
    void renderingProgress(int);
    void renderingDone();

public slots:
    void settingsChanged(void *settings);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void draw(QPainter &painter) override;

    void drawLine3D(int x0, int y0, float z0, int x1, int y1, float z1, uchar r, uchar g, uchar b);


private:
    int size = 800;
    bool mouse_pressed = false, ctrl_pressed = false;
    float mx0, my0;

    enum Mode {
        Wireframe, Render
    } mode = Wireframe;

    int wh, hh;


    void drawCoordinateSystem(const Matrix4x4 &Mres);
    void drawCurve(const QList<Matrix4x1> &dots, const Matrix4x4 &Mres, uchar r, uchar g, uchar b);

    inline void uniformVector(Matrix4x1 &v);
    inline void translateToScreenCoords(Matrix4x1 &v, int &x, int &y, float &z) const;


    Matrix4x4 getProjectiveMatrix() const;
    Matrix4x4 getCameraMatrix();
    Matrix4x4 getShiftMatrix() const;

    void changeRotationMatrix(float angleX, float angleY);
    void movement(float x, float y);
};

#endif // SCENECANVAS_H
