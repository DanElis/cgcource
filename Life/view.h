#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QPainter>
#include <stack>
#include <QtGui/QMouseEvent>
#include "utils.h"
#include "model.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
struct ViewSettings{
    int widthEdge;
    int lengthEdge;
    bool modeXOR;
};

class View : public QWidget
{
    Q_OBJECT
public:
    explicit View(QWidget *parent, Model **model, const ViewSettings & viewSettings);
    void paintEvent(QPaintEvent *event);
    ViewSettings getSettings();
    void setSettings(ViewSettings viewSettings);
    void setLineParameters(int widthEdge, int lengthEdge);
    void changeSizeScreen(int width, int height);
signals:
    void changeSizeImageX(int,int);
    void changeSizeImageY(int,int);
    void setVisiableVerticalScroll(bool);
    void setVisiableHorizontalScroll(bool);
public slots:

    void setXORMode(bool);
    void setImpact(bool);
    void setShiftScrollX(int x);
    void changeSizeScrolls();
    void setShiftScrollY(int y);
private:

    void changeSizeImage();
    QPoint getPositionHexagon( int x,int y);
    void drawHexagon(const int widthEdge,const QPoint centre,const int lineNumber, QImage *image, QPainter *painter);
    void setShiftX(const int lengthEdge);
    void setShiftY(const int lengthEdge);
    void createCopyImage();
    void drawField();
    int sign(const int i);
    void mousePressEvent ( QMouseEvent * event ) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void updateImage();

    int widthScreen;
    int heightScreen;
    int sizeImageX;
    int sizeImageY;
    static const QImage::Format format = QImage::Format_RGBA8888;
    const QRgb GREEN = qRgb(50, 220, 100);
    const QRgb BLACK = qRgb(0,0,0);
    const QRgb WHITE = qRgb(255,255,255);
    const int SIZE_IMAGE = 10000;
    const float WIDTH_SHIFT = 0.86602540378f;
    const float SQRT_3 = 1.73205081f;
    QPoint shiftScroll;
    QPoint shiftX;
    QPoint shiftY;
    QPoint lastModified;
    QImage image;
    QImage cpImage;
    ViewSettings viewSettings;
    QPainter *painter;
    Model *model;
    bool showImpact;
    const float EPSILON = 0.01;
};

#endif // VIEW_H
