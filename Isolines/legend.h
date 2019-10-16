#ifndef LEGEND_H
#define LEGEND_H


#include <QWidget>
#include <plot.h>
class Legend :public QWidget
{
    Q_OBJECT
public:
    explicit Legend(std::vector <QColor> colors,Plot* plot, QWidget *parent);
protected:
    void paintEvent(QPaintEvent *e) override;
private:
    std::vector<double> isolinesValue;
    Plot* plot;
    std::vector <QColor> colors;
    QColor getColor(double value);
};


#endif // LEGEND_H
