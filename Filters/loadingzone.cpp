#include "loadingzone.h"

LoadingZone::LoadingZone(QString name, QWidget *parent):
    QWidget(parent),
    nameZone(name)
{

}


void LoadingZone::paintEvent(QPaintEvent *event){
    QPainter painter(this);
        QRect size(0, 0, width() - 1, height() - 1);

        if (!image.isNull())
            painter.drawImage(0, 0, image);
        else
            painter.drawText(size, Qt::AlignCenter, nameZone);

        painter.drawRect(size);
}
void LoadingZone::setImage(QImage *newImage){
    if (newImage->width() > width() || newImage->height() > height()){
            image = (*newImage).scaled(width(), height(), Qt::KeepAspectRatio);
            scaleFactor = (float)image.height() / newImage->height();
        } else{
            scaleFactor = 1.0f;
            image = *newImage;
        }
    this->update();
}
