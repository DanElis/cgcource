#include "controller.h"


Controller::Controller(Model **model, View **view, QWidget *widget):
    model(*model),
    view(*view),
    widget(widget)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(run()));
}
void Controller::clear(){
    model->clear();
    view->update();
}
void Controller::run(){

    model->nextStep();
    view->update();
}

void Controller::changeTimer(bool state){
    if (state)
        timer.start(TIMER_PERIOD);
    else
        timer.stop();
}
void Controller::showInfo(){
    About about(widget);
    about.exec();
}


void Controller::nextState(){
    model->nextStep();
    view->update();
}


