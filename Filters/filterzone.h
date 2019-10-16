#ifndef FILTERZONE_H
#define FILTERZONE_H
#include "constant.h"
#include <QWidget>

class FilterZone: public QWidget
{
    Q_OBJECT
public:
    FilterZone(QString name,QWidget *parent);
    void setImage(QImage *image);

private:
    QString nameZone;

};

#endif // FILTERZONE_H
