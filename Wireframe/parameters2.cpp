#include "parameters2.h"
#include "ui_parameters2.h"

#include <QMainWindow>
#include <QToolBar>
#include <QVBoxLayout>
#include <QDebug>

#include <cmath>


Parameters2::Parameters2(QVector<Figure> &figures, Settings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Parameters2),
    settings(&settings),
    figures(&figures)
{
    ui->setupUi(this);

    ui->i_number->setMaximum(figures.size() - 1);
    ui->i_number->setValue(0);
    on_i_number_valueChanged(0);
}

Parameters2::~Parameters2()
{
    delete ui;
}

void Parameters2::on_i_a_editingFinished()
{
    ui->i_b->setMinimum(ui->i_a->value() + 0.01);
}

void Parameters2::on_i_b_editingFinished()
{
    ui->i_a->setMaximum(ui->i_b->value() - 0.01);
}

void Parameters2::on_i_c_editingFinished()
{
    ui->i_d->setMinimum(ui->i_c->value() + 0.01);
}

void Parameters2::on_i_d_editingFinished()
{
    ui->i_c->setMaximum(ui->i_d->value() - 0.01);
}

void Parameters2::on_i_zn_editingFinished()
{
    ui->i_zf->setMinimum(ui->i_zn->value() + 0.01);
}

void Parameters2::on_i_zf_editingFinished()
{
    ui->i_zn->setMaximum(ui->i_zf->value() - 0.01);
}

void Parameters2::on_i_number_valueChanged(int value)
{
    const Figure& figure = (*figures)[value];

    initializing = true;

    ui->i_R->setValue(figure.spline().r());
    ui->i_G->setValue(figure.spline().g());
    ui->i_B->setValue(figure.spline().b());
    ui->i_n->setValue(settings->n);
    ui->i_m->setValue(settings->m);
    ui->i_k->setValue(settings->k);
    ui->i_a->setValue(settings->a);
    ui->i_b->setValue(settings->b);
    ui->i_c->setValue(settings->c);
    ui->i_d->setValue(settings->d);
    ui->i_sw->setValue(settings->sw);
    ui->i_sh->setValue(settings->sh);
    ui->i_zn->setValue(settings->zn);
    ui->i_zf->setValue(settings->zf);

    ui->i_a->setMaximum(ui->i_b->value() - 0.01);
    ui->i_b->setMinimum(ui->i_a->value() + 0.01);
    ui->i_c->setMaximum(ui->i_d->value() - 0.01);
    ui->i_d->setMinimum(ui->i_c->value() + 0.01);
    ui->i_zn->setMaximum(ui->i_zf->value() - 0.01);
    ui->i_zf->setMinimum(ui->i_zn->value() + 0.01);

    float x, y, z;
    figure.getC(x, y, z);
    ui->i_Cx->setValue(x);
    ui->i_Cy->setValue(y);
    ui->i_Cz->setValue(z);
    figure.getR(x, y, z);
    ui->i_Rx->setValue(x * 180.0 / M_PI);
    ui->i_Ry->setValue(y * 180.0 / M_PI);
    ui->i_Rz->setValue(z * 180.0 / M_PI);
    figure.getS(x, y, z);
    ui->i_Sx->setValue(x);
    ui->i_Sy->setValue(y);
    ui->i_Sz->setValue(z);

    initializing = false;
}

void Parameters2::on_buttonBox_clicked(QAbstractButton *button)
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

    saveSettings();

    if (button->text() == "&OK")
        close();
}

void Parameters2::saveSettings()
{
    if (initializing) return;

    auto &figure = (*figures)[ui->i_number->value()];

    figure.setC(ui->i_Cx->value(), ui->i_Cy->value(), ui->i_Cz->value());
    figure.setR(ui->i_Rx->value() * M_PI / 180.0, ui->i_Ry->value() * M_PI / 180.0, ui->i_Rz->value() * M_PI / 180.0);
    figure.setS(ui->i_Sx->value(), ui->i_Sy->value(), ui->i_Sz->value());

    emit parametersDone(settings, figures);
}

void Parameters2::on_i_Cx_valueChanged(const QString &arg1)
{
    saveSettings();
}

void Parameters2::on_i_Cy_valueChanged(const QString &arg1)
{
    saveSettings();
}

void Parameters2::on_i_Cz_valueChanged(const QString &arg1)
{
    saveSettings();
}

void Parameters2::on_i_Rx_valueChanged(const QString &arg1)
{
    saveSettings();
}

void Parameters2::on_i_Ry_valueChanged(const QString &arg1)
{
    saveSettings();
}

void Parameters2::on_i_Rz_valueChanged(const QString &arg1)
{
    saveSettings();
}

void Parameters2::on_i_Sx_valueChanged(const QString &arg1)
{
    saveSettings();
}

void Parameters2::on_i_Sy_valueChanged(const QString &arg1)
{
    saveSettings();
}

void Parameters2::on_i_Sz_valueChanged(const QString &arg1)
{
    saveSettings();
}
