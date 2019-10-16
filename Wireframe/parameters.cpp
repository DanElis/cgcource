#include "parameters.h"
#include "ui_parameters.h"

#include <QMainWindow>
#include <QToolBar>
#include <QVBoxLayout>
#include <QDebug>


Parameters::Parameters(QList<BSpline> &splines, Settings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parameters),
    settings(&settings),
    splines(&splines)
{
    ui->setupUi(this);

    QMainWindow *mainWindow = new QMainWindow();
    mainWindow->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    QToolBar *myToolBar = new QToolBar();
    QAction *actionNew = new QAction("New B-Spline");
    QAction *actionDelete = new QAction("Delete B-Spline");
    actionAdd = new QAction("Add dot");
    actionAdd->setCheckable(true);
    actionAdd->setChecked(true);
    actionMove = new QAction("Move dot");
    actionMove->setCheckable(true);
    actionRemove = new QAction("Remove dot");
    actionRemove->setCheckable(true);
    QAction *actionZoomIn = new QAction("Zoom in");
    QAction *actionZoomOut = new QAction("Zoom out");
    connect(actionNew, SIGNAL(triggered()), this, SLOT(newSpline()));
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(deleteSpline()));
    connect(actionAdd, SIGNAL(triggered()), this, SLOT(addDot()));
    connect(actionMove, SIGNAL(triggered()), this, SLOT(moveDot()));
    connect(actionRemove, SIGNAL(triggered()), this, SLOT(removeDot()));
    connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
    connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
    myToolBar->addAction(actionNew);
    myToolBar->addAction(actionDelete);
    myToolBar->addSeparator();
    myToolBar->addAction(actionAdd);
    myToolBar->addAction(actionMove);
    myToolBar->addAction(actionRemove);
    myToolBar->addSeparator();
    myToolBar->addAction(actionZoomIn);
    myToolBar->addAction(actionZoomOut);
    mainWindow->addToolBar(myToolBar);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->addWidget(mainWindow);
    layout->addWidget(ui->content);
    setLayout(layout);


    ui->i_number->setValue(0);
    ui->i_number->setMaximum(splines.size() - 1);
    ui->canvas->setCurrent(&splines[0]);
    ui->i_R->setValue(ui->canvas->current().r());
    ui->i_G->setValue(ui->canvas->current().g());
    ui->i_B->setValue(ui->canvas->current().b());
    ui->i_n->setValue(settings.n);
    ui->i_m->setValue(settings.m);
    ui->i_k->setValue(settings.k);
    ui->i_a->setValue(settings.a);
    ui->i_b->setValue(settings.b);
    ui->i_c->setValue(settings.c);
    ui->i_d->setValue(settings.d);
    ui->i_sw->setValue(settings.sw);
    ui->i_sh->setValue(settings.sh);
    ui->i_zn->setValue(settings.zn);
    ui->i_zf->setValue(settings.zf);

    ui->i_a->setMaximum(ui->i_b->value() - 0.01);
    ui->i_b->setMinimum(ui->i_a->value() + 0.01);
    ui->i_c->setMaximum(ui->i_d->value() - 0.01);
    ui->i_d->setMinimum(ui->i_c->value() + 0.01);
    ui->i_zn->setMaximum(ui->i_zf->value() - 0.01);
    ui->i_zf->setMinimum(ui->i_zn->value() + 0.01);
}

Parameters::~Parameters()
{
    delete ui;
}

void Parameters::newSpline()
{
    splines->push_back(BSpline());

    ui->i_number->setMaximum(splines->size() - 1);
    ui->i_number->setValue(splines->size() - 1);
    ui->canvas->setCurrent(&splines->last());
    ui->i_R->setValue(ui->canvas->current().r());
    ui->i_G->setValue(ui->canvas->current().g());
    ui->i_B->setValue(ui->canvas->current().b());
}

void Parameters::deleteSpline()
{
    splines->removeAt(ui->i_number->value());
    if (splines->isEmpty())
        splines->push_back(BSpline());

    ui->i_number->setMaximum(splines->size() - 1);
    ui->i_number->setValue(0);
    ui->canvas->setCurrent(&splines->first());
    ui->i_R->setValue(ui->canvas->current().r());
    ui->i_G->setValue(ui->canvas->current().g());
    ui->i_B->setValue(ui->canvas->current().b());
}

void Parameters::addDot()
{
    ui->canvas->setMode(BSplineCanvas::ADD);

    auto mode = ui->canvas->mode();
    actionAdd->setChecked(mode == BSplineCanvas::ADD);
    actionMove->setChecked(mode == BSplineCanvas::MOVE);
    actionRemove->setChecked(mode == BSplineCanvas::REMOVE);
}

void Parameters::moveDot()
{
    ui->canvas->setMode(BSplineCanvas::MOVE);

    auto mode = ui->canvas->mode();
    actionAdd->setChecked(mode == BSplineCanvas::ADD);
    actionMove->setChecked(mode == BSplineCanvas::MOVE);
    actionRemove->setChecked(mode == BSplineCanvas::REMOVE);
}

void Parameters::removeDot()
{
    ui->canvas->setMode(BSplineCanvas::REMOVE);

    auto mode = ui->canvas->mode();
    actionAdd->setChecked(mode == BSplineCanvas::ADD);
    actionMove->setChecked(mode == BSplineCanvas::MOVE);
    actionRemove->setChecked(mode == BSplineCanvas::REMOVE);
}

void Parameters::zoomIn()
{
    ui->canvas->zoomIn();
}

void Parameters::zoomOut()
{
    ui->canvas->zoomOut();
}

void Parameters::on_i_a_editingFinished()
{
    ui->i_b->setMinimum(ui->i_a->value() + 0.01);

    ui->canvas->changeLimits(ui->i_a->value(), ui->i_b->value());
}

void Parameters::on_i_b_editingFinished()
{
    ui->i_a->setMaximum(ui->i_b->value() - 0.01);

    ui->canvas->changeLimits(ui->i_a->value(), ui->i_b->value());
}

void Parameters::on_i_c_editingFinished()
{
    ui->i_d->setMinimum(ui->i_c->value() + 0.01);
}

void Parameters::on_i_d_editingFinished()
{
    ui->i_c->setMaximum(ui->i_d->value() - 0.01);
}

void Parameters::on_i_zn_editingFinished()
{
    ui->i_zf->setMinimum(ui->i_zn->value() + 0.01);
}

void Parameters::on_i_zf_editingFinished()
{
    ui->i_zn->setMaximum(ui->i_zf->value() - 0.01);
}

void Parameters::on_i_R_editingFinished()
{
    ui->canvas->setColors(ui->i_R->value(), ui->i_G->value(), ui->i_B->value());
}

void Parameters::on_i_G_editingFinished()
{
    ui->canvas->setColors(ui->i_R->value(), ui->i_G->value(), ui->i_B->value());
}

void Parameters::on_i_B_editingFinished()
{
    ui->canvas->setColors(ui->i_R->value(), ui->i_G->value(), ui->i_B->value());
}

void Parameters::on_i_number_valueChanged(int value)
{
    ui->canvas->setCurrent(&(*splines)[value]);
    ui->i_R->setValue(ui->canvas->current().r());
    ui->i_G->setValue(ui->canvas->current().g());
    ui->i_B->setValue(ui->canvas->current().b());
}

void Parameters::on_buttonBox_clicked(QAbstractButton *button)
{
    settings->n = ui->i_n->value();
    settings->m = ui->i_m->value();
    settings->k = ui->i_k->value();
    settings->a = ui->i_a->value();
    settings->b = ui->i_b->value();
    settings->c = ui->i_c->value();
    settings->d = ui->i_d->value();
    settings->sw = ui->i_sw->value();
    settings->sh = ui->i_sh->value();
    settings->zn = ui->i_zn->value();
    settings->zf = ui->i_zf->value();

    emit parametersDone(settings, splines);

    if (button->text() == "&OK")
        close();
}
