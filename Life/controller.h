#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "model.h"
#include "view.h"
#include "about.h"
#include "settingsview.h"
#include <QTimer>
class Controller: public QObject
{
    Q_OBJECT
public:
    Controller(Model **model, View **view, QWidget *widget);
public slots:
    void changeTimer(bool state);
    void nextState();
    void run();
    void clear();
    void showInfo();
private:
    QTimer timer;
    Model *model;
    View *view;
    QWidget *widget;
    const int TIMER_PERIOD = 1000;
};

#endif // CONTROLLER_H
