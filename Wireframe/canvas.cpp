#include "canvas.h"

#include <QPainter>
#include <QResizeEvent>
#include <QDebug>


Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
}

void Canvas::updateCanvas()
{
    should_redraw = true;
    repaint();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    {
        QPainter painter(this);

        if (should_redraw) {
            QPainter image_painter(&image);

            should_redraw = false;
            data = image.bits();
            line_length = image.bytesPerLine();

            draw(image_painter);
        }

        painter.drawImage(event->rect(), image, event->rect());
    }

    afterDraw();
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    QSize new_size = event->size();

    QImage new_image(new_size, QImage::Format_ARGB32);
    image.swap(new_image);
    image.fill(0xFFFFFFFF);

    data = image.bits();
    line_length = image.bytesPerLine();
    should_redraw = true;
}

void Canvas::afterDraw()
{
}

void Canvas::drawLine(int x0, int y0, int x1, int y1, uchar r, uchar g, uchar b)
{
    bresenheim(x0, y0, x1, y1, r, g, b, 3, 2);
}

void Canvas::drawDashLine(int x0, int y0, int x1, int y1, int step1, int step2, uchar r, uchar g, uchar b)
{
    bresenheim(x0, y0, x1, y1, r, g, b, step1, step2);
}

void Canvas::drawFatDot(int x, int y, int w, int h, uchar r, uchar g, uchar b)
{
    auto *it = data + (y-2)*line_length + (x-2)*4ull;

    for (int i = -2; i <= 2; ++i, it += line_length - 20)
        for (int j = -2; j <= 2; ++j, it += 4) {
            if ((i == -2 && j == -2) || (i == -2 && j == 2) || (i == 2 && j == -2) || (i == 2 && j == 2) ||
                (i == 0 && j == -1) || (i == 0 && j == 0) || (i == 0 && j == 1) ||
                (i == -1 && j == 0) || (i == 1 && j == 0)) continue;

            if (y+i < 0 || x+j < 0 || y+i >= h || x+j >= w) continue;

            it[0] = b;
            it[1] = g;
            it[2] = r;
        }
}

void Canvas::bresenheim(int x0, int y0, int x1, int y1, uchar r, uchar g, uchar b, int limj1, int limj2)
{
    int absdx = std::abs(x1 - x0);
    int absdy = std::abs(y1 - y0);
    int dx2 = absdx * 2;
    int dy2 = absdy * 2;

#ifdef CLIPPING
    bool flag;
    int clip_step;
#endif
    int stepX, stepY, errStepX, errStepY;
    int err;

    if (absdy > absdx) {
        if (x1 < x0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
#ifdef CLIPPING
        flag = true;
        clip_step = (y1-y0 < 0) ? -1 : 1;
#endif
        stepX = (y1-y0 < 0) ? -line_length : line_length;
        stepY = 4;
        err = -absdy;
        errStepX = dx2;
        errStepY = dy2;
    } else {
        if (y1 < y0) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
#ifdef CLIPPING
        flag = false;
        clip_step = (x1-x0 < 0) ? -1 : 1;
#endif
        stepX = (x1-x0 < 0) ? -4 : 4;
        stepY = line_length;
        err = -absdx;
        errStepX = dy2;
        errStepY = dx2;
    }

    auto *it = data + y0*line_length + x0*4ull;
    for (int i = -err, j = 0; i >= 0; --i, ++j) {
#ifdef CLIPPING
        if (x0 >= 0 && y0 >= 0 && x0 < width() && y0 < height()) {
#endif
        if (j < limj1) {
            it[0] = b;
            it[1] = g;
            it[2] = r;
            it[3] = 255;
        }
        j %= limj2;
#ifdef CLIPPING
        }
#endif

#ifdef CLIPPING
        if (flag) y0 += clip_step; else x0 += clip_step;
#endif
        it += stepX;

        err += errStepX;
        if (err > 0) {
            err -= errStepY;
#ifdef CLIPPING
            if (flag) ++x0; else ++y0;
#endif
            it += stepY;
        }
    }
}
