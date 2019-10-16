#ifndef LOADINGZONE_H
#define LOADINGZONE_H

#include <QWidget>
#include <QObject>
#include "constant.h"
#include <QPainter>
class LoadingZone:public QWidget
{
    Q_OBJECT
public:
    LoadingZone(QString name, QWidget *parent);
    void setImage(QImage *image);

private:
    void paintEvent(QPaintEvent *event);
    QString nameZone;
    QImage image;
    float scaleFactor;
};

#endif // LOADINGZONE_H
