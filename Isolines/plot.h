#ifndef PLOT_H
#define PLOT_H

#include <cmath>
#include <QWidget>
#include <settings.h>
#include <vector>
#include <QPainter>
#include <map>
#include <QMouseEvent>
#include <iterator>

class Plot: public QWidget
{
    Q_OBJECT
public:
    explicit Plot(QColor colorLine, std::vector <QColor> colors,ParametrsIsolines parametrsIsolines,bool modeColorMap, QWidget* parent);
    double f(double x, double y);
    void drawPlot();
    void interpolate();
    void grid();
    void drawIsolines();
    void calculateFunctionValue();
    std::vector<double> getIsolinesValue();
    bool isModeInterpolate();
    std::vector <QColor> getColors();
    ParametrsIsolines getParametrs();
    void setParametrs(ParametrsIsolines p);
    void setColors(std::vector <QColor> c, QColor newColorLine);
public slots:
    void setModeInterpolate(bool state);
    void setModeGrid(bool state);
    void setModeIsolines(bool state);
    void setModeColorMap(bool state);
signals:
    void changeStatusBar(double,double,double);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    void fourPointsIntersection(QPointF points[], double lineValue);
    std::vector<QPointF> findPointsIntersections(int numIntersections, double f_i[],QPointF points[], double lineValue);
    void findMinMax();
    QPointF convertCoordinatesImage(double x, double y);
    QPointF convertCoordinatesPlot(double u, double v);
    void buildIsolines();
    void setIsolines();
    QColor getColor(double value);

    QImage imageIsolines;
    QColor colorLine;
    QImage image;
    static const QImage::Format format = QImage::Format_RGBA8888;
    bool modeColorMap;
    bool modeInterpolate;
    bool modeIsolines;
    bool modeGrid;
    std::vector<QLineF> isolines;
    const int NUMBER_CORNERS_RECTANGLE = 4;
    const int NUMBER_CORNERS_TRIANGLE = 3;
    const double EPS = 0.001;
    std::vector <QColor> colors;
    ParametrsIsolines parametrs;
    std::vector<double> gridValue;
    std::vector<double> isolinesValue;
    double min;
    double max;
    bool updateImageIsolines;
    bool updateIsolines;
    bool firstPress;
    bool check;


};

#endif // PLOT_H
