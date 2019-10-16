#include "plot.h"

Plot::Plot(QColor colorLine,std::vector <QColor> colors,ParametrsIsolines parametrsIsolines, bool modeColorMap, QWidget* parent):
    QWidget(parent),
    colorLine(colorLine),
    modeColorMap(modeColorMap),
    modeIsolines(false),
    modeGrid(false),
    colors(colors),
    parametrs(parametrsIsolines),
    gridValue(parametrsIsolines.k*parametrsIsolines.m),
    min(std::numeric_limits<double>::max()),
    max(-std::numeric_limits<double>::max()),
    updateImageIsolines(false),
    updateIsolines(false),
    firstPress(true)
{
    calculateFunctionValue();
}
void Plot::paintEvent(QPaintEvent *event){
    QPainter painterImage(this);
    if(updateImageIsolines){
        painterImage.drawImage(0,0,image);
        painterImage.drawImage(0,0,imageIsolines);
        updateImageIsolines = false;
        return;
    }
    else if (updateIsolines) {
        updateIsolines = false;
        memset(imageIsolines.bits(), 0,imageIsolines.byteCount());
        buildIsolines();
        drawIsolines();
        painterImage.drawImage(0,0,image);
        painterImage.drawImage(0,0,imageIsolines);
        return;
    }
    memset(image.bits(), 0,image.byteCount());
    memset(imageIsolines.bits(), 0,imageIsolines.byteCount());
    drawPlot();
    if(modeGrid){
        grid();
    }
    buildIsolines();
    if(modeIsolines){
        drawIsolines();
    }
    painterImage.drawImage(0,0,image);
    painterImage.drawImage(0,0,imageIsolines);
}
void Plot::resizeEvent(QResizeEvent *event){
    image = QImage(size(), format);
    imageIsolines = QImage(size(),format);
    calculateFunctionValue();
    update();
}
double Plot::f(double x, double y){
    return tanh(x) + sin(y);
}

void Plot::drawPlot(){
    QPainter painterPoint(&image);
    for(int i = 0; i < width(); i++){
        for(int j = 0; j < height(); j++){
            QPointF p = convertCoordinatesImage(i,j);
            double F = f(p.x(),p.y());
            QColor color = getColor(F);
            painterPoint.setPen(color);
            painterPoint.drawPoint(QPoint(i,j));
        }
    }
}

QColor Plot::getColor(double value){
    if(modeColorMap){
        double stepSize = (max - min)/colors.size();

        int idx = (int)((value - min)/stepSize);

        if (idx < 0)
            idx = 0;
        if (idx > colors.size() - 1)
            idx = colors.size() - 1;

        return colors[idx];
    }
    else{
        value -= min;
        double stepSize = (max - min)/(colors.size() - 1);

        int idx = (int)((value)/stepSize);
        if (idx < 0)
            idx = 0;
        if (idx >= (int)colors.size() - 1)
            idx = (int)colors.size() - 2;

        QColor c1 = colors[idx];
        QColor c2 = colors[idx + 1];
        int r1 = c1.red();
        int g1 = c1.green();
        int b1 = c1.blue();
        int r2 = c2.red();
        int g2 = c2.green();
        int b2 = c2.blue();
        double w1 = value/stepSize - idx;
        double w2 = 1 - w1;
        int r = int(r1*w2 + r2*w1);
        int g = int(g1*w2 + g2*w1);
        int b = int(b1*w2 + b2*w1 );
        return QColor(r,g,b);
    }
}

void Plot::interpolate(){
    QPainter painterPoint(&image);
    for(int i = 0; i < width(); i++){
        for(int j = 0; j < height(); j++){
            QPointF p = convertCoordinatesImage(i,j);
            double F = f(p.x(),p.y());
            QColor color = getColor(F);
            painterPoint.setPen(color);
            painterPoint.drawPoint(QPoint(i,j));
        }
    }
}

void Plot::drawIsolines(){
    QPainter painterLine(&imageIsolines);
    painterLine.setPen(colorLine);
    for(auto line: isolines){
        painterLine.drawLine(line);
    }
}

void Plot::grid(){
    int x = 0;
    int y = 0;
    QPainter painterLine(&image);
    for(unsigned long i = 0; i < parametrs.k + 1; i++ ){
        x = (width()-1)*i/parametrs.k;
        painterLine.drawLine(QLine(QPoint(x,0),QPoint(x, height())));
    }
    for(unsigned long j = 0; j< parametrs.m + 1;j++ ){
        y = (height() - 1)*j/parametrs.m;
        painterLine.drawLine(QLine(QPoint(0,y),QPoint(width(), y)));
    }
}

void Plot::calculateFunctionValue(){
    for(unsigned long i = 0; i < width(); i++){
        for(unsigned long j = 0; j < height(); j++){
            QPointF p = convertCoordinatesImage(i,j);
            double value = f(p.x(),p.y());
            if(value < min){
                min = value;
            }
            if(value > max){
                max = value;
            }
        }
    }
    setIsolines();
}
void Plot::buildIsolines(){
    isolines.clear();
    unsigned long  k = parametrs.k;
    unsigned long  m = parametrs.m;
    for(unsigned long i = 0; i < k ; i++){
        for(unsigned long j = 0; j < m ; j++){
            QPointF pointsOnImage[4] = {
                QPointF(width()*i/k,height()* j/m),
                QPointF(width()*(i+1)/k, height()*j/m),
                QPointF(width()*(i+1)/k, height()*(j+1)/m),
                QPointF(width()*i/k, height()*(j+1)/m)
            };
            QPointF pointsOnPlot[4] = {
                convertCoordinatesImage(pointsOnImage[0].x(),pointsOnImage[0].y()),
                convertCoordinatesImage(pointsOnImage[1].x(),pointsOnImage[1].y()),
                convertCoordinatesImage(pointsOnImage[2].x(),pointsOnImage[2].y()),
                convertCoordinatesImage(pointsOnImage[3].x(),pointsOnImage[3].y()),
            };

            double f_i[4] = {
                f(pointsOnPlot[0].x(), pointsOnPlot[0].y()),
                f(pointsOnPlot[1].x(), pointsOnPlot[1].y()),
                f(pointsOnPlot[2].x(), pointsOnPlot[2].y()),
                f(pointsOnPlot[3].x(), pointsOnPlot[3].y())
            };
            for(auto lineValue:isolinesValue){
                int numIntersections = 0;
                for(int k = 0; k < 4; k++){
                    if (f_i[k] > lineValue)
                        numIntersections++;
                }
                if(numIntersections == 0){
                    continue;
                }
                if(numIntersections == 4)
                    continue;
                std::vector<QPointF> pointsIntersections = findPointsIntersections(NUMBER_CORNERS_RECTANGLE, f_i,pointsOnPlot, lineValue);
                if(pointsIntersections.size() == 2){
                    QLineF newLine = QLineF(pointsIntersections[0],pointsIntersections[1]);
                    isolines.push_back(newLine);
                }
                else if (pointsIntersections.size() == 3) {
                    pointsIntersections = findPointsIntersections(NUMBER_CORNERS_RECTANGLE, f_i,pointsOnPlot, lineValue+EPS);
                    if(pointsIntersections.size() == 2){
                        QLineF newLine = QLineF(pointsIntersections[0],pointsIntersections[1]);
                        isolines.push_back(newLine);
                    }
                    else if (pointsIntersections.size() == 4) {

                        fourPointsIntersection(pointsOnPlot,lineValue+EPS);
                    }
                }
                else if(pointsIntersections.size() == 4){
                    fourPointsIntersection(pointsOnPlot,lineValue);
                }
            }
        }
    }
}
void Plot::setIsolines(){
    isolinesValue.clear();
    int j = 1;
    double step = (max-min)/(colors.size());
    for(;j < colors.size();j++){
        isolinesValue.push_back(min+j*step);
    }
}
std::vector<QPointF> Plot::findPointsIntersections(int numPoints, double f_i[],QPointF *points, double lineValue){
    std::vector<QPointF> pointsIntersections;

    for(int i = 0; i < numPoints; i++){
        double currF = f_i[i];
        double nextF = f_i[(i+1)%numPoints];
        if(!(currF > lineValue && nextF > lineValue) &&
                !(currF < lineValue && nextF < lineValue) ){
            QPointF p1 = convertCoordinatesPlot(points[i].x(),points[i].y());
            QPointF p2 = convertCoordinatesPlot(points[(i+1)%numPoints].x(),points[(i+1)%numPoints].y());
            double dx = p1.x() - p2.x();
            double dy = p1.y() - p2.y();
            if(abs(dx) > EPS){
                double x = dx * abs(lineValue - f_i[i])/abs(f_i[i]-f_i[(i+1)%numPoints]);
                pointsIntersections.push_back(QPointF((p1.x()-x), (p1.y())));
            }
            else if (abs(dy) > EPS) {
                double y = dy * abs(lineValue - f_i[i])/abs(f_i[i]-f_i[(i+1)%numPoints]);

                pointsIntersections.push_back(QPointF(p1.x(), p1.y() - y));
            }
        }
    }
    return pointsIntersections;
}
QPointF Plot::convertCoordinatesPlot(double x, double y){
    double u =(width() - 0)*(x-parametrs.a)/(parametrs.b-parametrs.a) + 0.5;
    double v = (height() - 0)*(-y + parametrs.c)/(parametrs.d-parametrs.c) - 1 + height();
    QPointF res = QPointF(u,v);
    return res;
}
QPointF Plot::convertCoordinatesImage(double u, double v){
    double x = (parametrs.b - parametrs.a)* (u - 0)/(width() - 0) + parametrs.a;
    double y = (parametrs.d - parametrs.c)* (height() - v - 1)/(height() - 0) + parametrs.c;
    QPointF res = QPointF(x,y);
    return res;
}
void Plot::fourPointsIntersection(QPointF *points, double lineValue){
    QPointF centre = QPointF((points[0].x()+points[1].x())/2,(points[1].y()+points[2].y())/2);
    for(int k = 0; k < 4; k++){
        QPointF pointsTriangle[3] = {
            points[k],
            points[(k+1)%4],
            centre
        };
        double f_i[3] = {
            f(points[0].x(), points[0].y()),
            f(points[1].x(), points[1].y()),
            f(points[2].x(), points[2].y())
        };

        std::vector<QPointF> pointsIntersections = findPointsIntersections(NUMBER_CORNERS_TRIANGLE, f_i,pointsTriangle, lineValue);
        if(pointsIntersections.size() == 2){
            QLineF newLine = QLineF(pointsIntersections[0],pointsIntersections[1]);
            isolines.push_back(newLine);
        }
        else if (pointsIntersections.size() == 3) {
            pointsIntersections = findPointsIntersections(NUMBER_CORNERS_TRIANGLE, f_i,pointsTriangle, lineValue+EPS);
            if(pointsIntersections.size() == 2){
                QLineF newLine = QLineF(pointsIntersections[0],pointsIntersections[1]);
                isolines.push_back(newLine);
            }
        }
    }
}


void Plot::setModeColorMap(bool state){
    modeColorMap = state;
    modeInterpolate = !state;
    update();
}

void Plot::setModeGrid(bool state){
    modeGrid = state;
    update();
}

void Plot::setModeIsolines(bool state){
    modeIsolines = state;
    updateImageIsolines = true;
    if(modeIsolines){
        drawIsolines();
        update();
    }
    else {
        memset(imageIsolines.bits(), 0,imageIsolines.byteCount());
        update();
    }
}

void Plot::setModeInterpolate(bool state){
    modeInterpolate = state;
    modeColorMap = !state;
    update();
}


std::vector<double> Plot::getIsolinesValue(){
    return isolinesValue;
}

bool Plot::isModeInterpolate(){
    return !modeColorMap;
}

std::vector <QColor> Plot::getColors(){
    return colors;
}

void Plot::mouseMoveEvent(QMouseEvent *event){
    QPointF p = convertCoordinatesImage(event->x(),event->y());
    emit changeStatusBar(p.x(),p.y(),f(p.x(),p.y()));
    if((event->buttons()&Qt::LeftButton) && modeIsolines){
        double F = f(p.x(),p.y());
        if(firstPress){
            isolinesValue.push_back(F);
            check = false;
        }
        else{
            if(!check){
                isolinesValue.pop_back();
            }
            check = true;
            isolinesValue[isolinesValue.size() - 1] = F;
        }
        updateIsolines = true;
        firstPress = false;
        update();
    }

}

ParametrsIsolines Plot::getParametrs(){
    return parametrs;
}

void Plot::setParametrs(ParametrsIsolines p){
    parametrs = p;
    calculateFunctionValue();
    update();
}
void Plot::setColors(std::vector <QColor> c,QColor newColorLine){
    colors = c;
    colorLine = newColorLine;
    update();
}
void Plot::mouseReleaseEvent(QMouseEvent* event){
    if(modeIsolines){
        firstPress = true;
        if(check)
            isolinesValue.pop_back();
        updateIsolines = true;
        update();
    }
}
void Plot::mousePressEvent(QMouseEvent* event){
    if(modeIsolines){
        QPointF p = convertCoordinatesImage(event->x(),event->y());
        double F = f(p.x(),p.y());
        isolinesValue.push_back(F);
        updateIsolines = true;
        check = false;
        update();
    }
}








