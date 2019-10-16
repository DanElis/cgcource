#ifndef UTILS_H
#define UTILS_H

#include <stack>
#include <QWidget>
namespace Utils {
    struct Span{
        int x0;
        int x1;
        int y;
    };

    void drawLine(const QPoint p0,const QPoint p1, QRgb color, QImage *image);
    void spanFill(const QPoint start, const QRgb newColor, QImage *image);
    void drawLineBresenhamX(int deltaX, int deltaY, int signX,int signY,int x, int y, QRgb color, QImage *image);
    void drawLineBresenhamY(int deltaX, int deltaY, int signX,int signY,int x, int y, QRgb color, QImage *image);
    void getNewSpan(int minX,int maxX ,int y, QRgb *pixels, QRgb color, std::stack<Span> &spanStack,QImage *image);
    void up(QRgb *pixels, QRgb oldColor, QRgb newColor,std::stack<Span> spanStack, QImage *image);
    void down(QRgb *pixels, QRgb oldColor, QRgb newColor,std::stack<Span> spanStack,QImage *image);
    int findLeftBound(int minX, int y, QRgb color, QRgb *pixels, int imageWidth);
    int findRightBound( int maxX , int y, int leftBound,QRgb color, QRgb *pixels,int imageWidth);
    const QRgb BLACK = qRgb(0,0,0);
};

#endif // UTILS_H
