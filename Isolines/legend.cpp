#include "legend.h"

Legend::Legend(std::vector <QColor> colors,Plot* plot, QWidget *parent) :
    QWidget(parent),
    plot(plot),
    colors(colors)
{
}

void Legend::paintEvent(QPaintEvent *e)
{
    isolinesValue.clear();
    isolinesValue = plot->getIsolinesValue();
    QWidget::paintEvent(e);
    QPainter painter(this);

    QPointF rect(40, 15);
    double padding = (double)(width() - 75) / (isolinesValue.size() - 1);
    for(unsigned i = 0; i < isolinesValue.size(); ++i){
        QPointF center(i*padding + 40, 15);
        painter.drawText(QRectF(center - rect, center + rect), Qt::AlignCenter, QString::number(isolinesValue[i], 'f', 2));
    }
    for(int i = 0; i < width(); i++){
        for(int j = 30; j < height(); j++){
            painter.setPen(getColor(i));
            painter.drawPoint(i,j);
        }
    }

}
QColor Legend::getColor(double value){
    double min = 0;
    double max = width();
    if(!plot->isModeInterpolate()){
        double stepSize = (max - min)/colors.size();

        int idx = (int)((value - min)/stepSize);

        if (idx < 0)
            idx = 0;
        if (idx > colors.size() - 1)
            idx = colors.size() - 1;

        return colors[idx];
    }
    else{
        double stepSize = (max - min)/(colors.size() - 1);

        int idx = (int)(value/stepSize);
        if (idx < 0)
            idx = 0;
        if (idx >= (int)colors.size() - 1)
            idx = (int)colors.size() - 2;

        QColor c1 = colors[idx];
        QColor c2 = colors[idx + 1];

        double w1 = value/stepSize - idx;
        double w2 = 1 - w1;

        int r = c1.red()*w2 + c2.red()*w1;
        int g = c1.green()*w2 + c2.green()*w1;
        int b = c1.blue()*w2 + c2.blue()*w1;
        return QColor(r,g,b);
    }
}
