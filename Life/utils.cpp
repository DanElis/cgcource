#include "utils.h"


void Utils::drawLineBresenhamX(int deltaX, int deltaY, int signX,int signY,int x, int y, QRgb color, QImage *image){
    int err = 0;
    for (int i = 0; i < deltaX; i++) {
        image->setPixel(x, y,color);
        x += signX;
        err += 2*deltaY;
        if(err > deltaX) {
            err -= 2*deltaX;
            y += signY;
        }
    }
}
void Utils::drawLineBresenhamY(int deltaX, int deltaY, int signX,int signY,int x, int y, QRgb color, QImage *image){
    int err = 0;
    for (int i = 0; i < deltaY; i++) {
        image->setPixel(x, y,color);
        y += signY;
        err += 2*deltaX;
        if(err > deltaY) {
            err -= 2*deltaY;
            x += signX;
        }
    }
}


void Utils::drawLine(const QPoint p0, const QPoint p1, QRgb color, QImage *image){
    int x1 = p0.x();
    int y1 = p0.y();
    int x2 = p1.x();
    int y2 = p1.y();
    int deltaX = abs(x2 - x1);
    int deltaY = abs(y2 - y1);
    int signX = x1 < x2 ? 1 : -1;
    int signY = y1 < y2 ? 1 : -1;
    if (deltaX > deltaY){
        drawLineBresenhamX(deltaX,deltaY,signX,signY,x1,y1,color,image);
    }
    else {
        drawLineBresenhamY(deltaX,deltaY,signX,signY,x1,y1,color,image);
    }

}
void Utils::spanFill(const QPoint start,const QRgb newColor, QImage *image){
    if (start.y() < 0 || start.x() < 0 || start.y() >= image->height() || start.x() >= image->width()) {
        return;
    }

    QRgb *pixels = reinterpret_cast<QRgb *>(image->bits());
    QRgb oldColor = pixels[start.x() + start.y() * image->width()];
    if(oldColor == BLACK)
        return;
    Span span;
    span.y = start.y();
    for(int i = start.x(); i <= image->width();i++){
        if(pixels[i + start.y()*image->width()] != oldColor){
            span.x1 = i - 1;
            break;
        }
    }
    for(int i = start.x(); i >= 0; i--){
        if(pixels[i + start.y()*image->width()] != oldColor){
            span.x0 = i + 1;
            break;
        }
    }
    std::stack<Span> spanStack;

    spanStack.push(span);
    up(pixels,oldColor,newColor,spanStack, image);

    spanStack.push(span);
    down(pixels,oldColor,newColor,spanStack, image);

}
void Utils::up(QRgb *pixels, QRgb oldColor, QRgb newColor,std::stack<Span> spanStack, QImage *image){
    while(!spanStack.empty()){
        Span span = spanStack.top();
        spanStack.pop();

        getNewSpan(span.x0,span.x1,span.y+1,pixels,oldColor, spanStack,image);
        for (int i = span.x0; i <= span.x1; i++) {
            image->setPixel(i, span.y,newColor);
        }
    }
}
void Utils::down(QRgb *pixels, QRgb oldColor, QRgb newColor,std::stack<Span> spanStack, QImage *image){
    while(!spanStack.empty()){
        Span span = spanStack.top();
        spanStack.pop();

        getNewSpan(span.x0,span.x1,span.y-1,pixels,oldColor, spanStack,image);

        for (int i = span.x0; i <= span.x1; i++) {
            image->setPixel(i, span.y,newColor);
        }
        bool isRight = span.x0 <= span.x1;
        if(isRight){
            isRight = false;
        }
    }
}
void Utils::getNewSpan(int minX, int maxX, int y, QRgb *pixels, QRgb color, std::stack<Span> &spanStack, QImage *image){
    Span newSpan;
    newSpan.y = y;
    newSpan.x0 = findLeftBound(minX,y,color,pixels,image->width());
    if(newSpan.x0 == -1)
        return;
    newSpan.x1 = findRightBound(maxX,y,newSpan.x0,color, pixels, image->width());
    if(newSpan.x1 == -1)
        return;
    spanStack.push(newSpan);
}

int Utils::findLeftBound(int minX, int y, QRgb color, QRgb *pixels, int imageWidth){
    int leftBound = -1;
    if(pixels[minX+y*imageWidth] == color){
        for(int i = minX; i >= 0; i--){
            if(pixels[i + y*imageWidth] != color){
                leftBound = i + 1;
                break;
            }
        }
    }
    else {
        for(int i = minX; i <= imageWidth; i++){
            if(pixels[i + y*imageWidth] == color){
                leftBound = i;
                break;
            }
        }
    }
    return leftBound;
}
int Utils::findRightBound( int maxX , int y, int leftBound,QRgb color, QRgb *pixels, int imageWidth){
    int rightBound = -1;
    if(pixels[maxX+y*imageWidth] == color){
        for(int i = maxX; i <= imageWidth; i++){
            if(pixels[i + y*imageWidth] != color){
                rightBound = i - 1;
                break;
            }
        }
    }
    else {
        for(int i = leftBound; i <= maxX; i++){
            if(pixels[i + y*imageWidth]!= color){
                rightBound = i - 1;
                break;
            }
        }
    }
    return rightBound;
}
