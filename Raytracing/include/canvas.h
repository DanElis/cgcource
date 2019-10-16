#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>

typedef unsigned long long ull;

#define CLIPPING


class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

public slots:
    void updateCanvas();


protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    virtual void draw(QPainter &painter) = 0;
    virtual void afterDraw();

    virtual void drawLine(int x0, int y0, int x1, int y1, uchar r, uchar g, uchar b);
    virtual void drawDashLine(int x0, int y0, int x1, int y1, int step1, int step2, uchar r, uchar g, uchar b);
    virtual void drawFatDot(int x, int y, int w, int h, uchar r, uchar g, uchar b);


    QImage image;
    bool should_redraw = false;
    uchar *data = nullptr;
    ull line_length;


private:
    void bresenheim(int x0, int y0, int x1, int y1, uchar r, uchar g, uchar b, int limj1, int limj2);
};

#endif // CANVAS_H
