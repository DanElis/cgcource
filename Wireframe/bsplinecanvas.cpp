#include "bsplinecanvas.h"

#include <cmath>
#include <QResizeEvent>
#include <QDebug>


#define CLIP_MOUSE \
    float posx = std::min(std::max(offset + .0, event->localPos().x()), width() - offset + .0); \
    float posy = std::min(std::max(offset + .0, event->localPos().y()), height() - offset + .0);



BSplineCanvas::BSplineCanvas(QWidget *parent) : Canvas(parent)
{
}

void BSplineCanvas::changeLimits(float a, float b)
{
    _current->changeLimits(a, b);
    should_redraw = true;
    repaint();
}

void BSplineCanvas::setColors(uchar r, uchar g, uchar b)
{
    _current->setColors(r, g, b);
    should_redraw = true;
    repaint();
}

void BSplineCanvas::setCurrent(BSpline *current)
{
    this->_current = current;
    _current->setSizes(width() / 2, height() / 2);
    should_redraw = true;
    repaint();
}

BSpline &BSplineCanvas::current()
{
    return *_current;
}

BSplineCanvas::Mode BSplineCanvas::mode() const
{
    return _mode;
}

void BSplineCanvas::setMode(BSplineCanvas::Mode mode)
{
    this->_mode = mode;
}

void BSplineCanvas::zoomIn()
{
    if (scale < 11) {
        ++scale;
        should_redraw = true;
        repaint();
    }
}

void BSplineCanvas::zoomOut()
{
    if (scale > 1) {
        --scale;
        should_redraw = true;
        repaint();
    }
}

void BSplineCanvas::mousePressEvent(QMouseEvent *event)
{
    CLIP_MOUSE

    switch (_mode) {
    case ADD:
        rescale_coords(posx, posy);
        _current->addLast(posx, posy);
        mouseMoveEvent(event);
        break;
    case MOVE:
        {
            auto &skeleton = _current->skeleton();
            for (int i = 0; i < skeleton.size(); ++i) {
                auto d = skeleton[i];
                scale_coords(d.first, d.second);
                if (abs(d.first - posx) + abs(d.second - posy) < 10) {
                    choosen = i;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
}

void BSplineCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    CLIP_MOUSE

    switch (_mode) {
    case REMOVE:
        {
            auto &skeleton = _current->skeleton();
            for (int i = 0; i < skeleton.size(); ++i) {
                auto d = skeleton[i];
                scale_coords(d.first, d.second);
                if (abs(d.first - posx) + abs(d.second - posy) < 20) {
                    _current->remove(i);
                    should_redraw = true;
                    repaint();
                    break;
                }
            }
        }
        break;
    case MOVE:
        choosen = -1;
        break;
    default:
        break;
    }
}

void BSplineCanvas::mouseMoveEvent(QMouseEvent *event)
{
    CLIP_MOUSE

    switch (_mode) {
    case ADD:
        rescale_coords(posx, posy);
        _current->removeLast();
        _current->addLast(posx, posy);
        should_redraw = true;
        repaint();
        break;
    case MOVE:
        if (choosen != -1) {
            rescale_coords(posx, posy);
            _current->replace(choosen, posx, posy);
            should_redraw = true;
            repaint();
        }
        break;
    default:
        break;
    }
}

void BSplineCanvas::draw(QPainter &painter)
{
    std::fill((uint *) data, (uint *) data + width()*height(), 0xFF000000);

    wh = width() / 2;
    hh = height() / 2;

    drawLine(0, hh, width()-1, hh, 128, 128, 128);
    drawLine(wh, 0, wh, height()-1, 128, 128, 128);
    for (int i = 10; i < std::max(wh, hh); i += 10) {
        drawLine(wh + i*scale, hh - 2*scale, wh + i*scale, hh + 2*scale, 128, 128, 128);
        drawLine(wh - i*scale, hh - 2*scale, wh - i*scale, hh + 2*scale, 128, 128, 128);
        drawLine(wh - 2*scale, hh + i*scale, wh + 2*scale, hh + i*scale, 128, 128, 128);
        drawLine(wh - 2*scale, hh - i*scale, wh + 2*scale, hh - i*scale, 128, 128, 128);
    }


    auto &skeleton = _current->skeleton();
    for (int i = 0; i < skeleton.size(); ++i) {
        auto d1 = skeleton[i];
        scale_coords(d1.first, d1.second);
        drawFatDot(d1.first, d1.second, width(), height(), 170, 0, 0);

        if (i < skeleton.size()-1) {
            auto d2 = skeleton[i+1];
            scale_coords(d2.first, d2.second);
            drawLine(d1.first, d1.second, d2.first, d2.second, 170, 0, 0);
        }
    }

    auto &dots = _current->bspline();
    int ia = _current->splineBegin();
    int ib = _current->splineEnd();
    uchar r = _current->r(), g = _current->g(), b = _current->b();
    for (int i = 0; i < dots.size()-1; ++i) {
        auto d1 = dots[i];
        auto d2 = dots[i+1];
        scale_coords(d1.first, d1.second);
        scale_coords(d2.first, d2.second);

        if (i <= ia || i >= ib)
            drawDashLine(d1.first, d1.second, d2.first, d2.second, 1, 3, 128, 128, 128);
        else
            drawLine(d1.first, d1.second, d2.first, d2.second, r, g, b);
    }
}

void BSplineCanvas::scale_coords(float &x, float &y)
{
    x = (x - wh) * scale + wh;
    y = (y - hh) * scale + hh;
}

void BSplineCanvas::rescale_coords(float &x, float &y)
{
    x = (x - wh) / scale + wh;
    y = (y - hh) / scale + hh;
}

