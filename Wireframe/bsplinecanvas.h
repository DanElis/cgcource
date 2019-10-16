#ifndef BSPLINECANVAS_H
#define BSPLINECANVAS_H

#include "canvas.h"

#include "bspline.h"


class BSplineCanvas : public Canvas
{
    Q_OBJECT
public:
    enum Mode {
        ADD, MOVE, REMOVE
    };

    explicit BSplineCanvas(QWidget *parent = nullptr);

    void changeLimits(float a, float b);
    void setColors(uchar r, uchar g, uchar b);

    void setCurrent(BSpline *_current);
    BSpline& current();

    Mode mode() const;
    void setMode(Mode _mode);
    void zoomIn();
    void zoomOut();


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void draw(QPainter &painter) override;


private:
    static const int offset = 5;

    int scale = 5;
    Mode _mode = ADD;
    int choosen = -1;
    int wh, hh;

    BSpline *_current = nullptr;

    inline void scale_coords(float &x, float &y);
    inline void rescale_coords(float &x, float &y);
};

#endif // BSPLINECANVAS_H
