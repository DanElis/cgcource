#include "parameters.h"
#include "ui_parameters.h"

#include <QMainWindow>
#include <QToolBar>
#include <QVBoxLayout>
#include <QDebug>

#include <cmath>


Parameters::Parameters(Settings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parameters),
    settings(&settings)
{
    ui->setupUi(this);

    ui->i_R->setValue((int) (255.0f * settings.Br + 0.5f));
    ui->i_G->setValue((int) (255.0f * settings.Br + 0.5f));
    ui->i_B->setValue((int) (255.0f * settings.Br + 0.5f));
    ui->i_gamma->setValue(settings.gamma);
    ui->i_depth->setValue(settings.depth);

    ui->i_quality->insertItem(0, "Rough");
    ui->i_quality->insertItem(1, "Normal");
    ui->i_quality->insertItem(2, "Fine");
    ui->i_quality->setCurrentIndex(settings.quality);
}

Parameters::~Parameters()
{
    delete ui;
}

void Parameters::on_buttonBox_clicked(QAbstractButton *button)
{
    if (button->text() == "Cancel")
        close();

    settings->Br = ui->i_R->value() / 255.0f;
    settings->Bg = ui->i_G->value() / 255.0f;
    settings->Bb = ui->i_B->value() / 255.0f;
    settings->gamma = ui->i_gamma->value();
    settings->depth = ui->i_depth->value();
    settings->quality = (Settings::Quality) ui->i_quality->currentIndex();

    emit parametersDone(settings);
    close();
}

