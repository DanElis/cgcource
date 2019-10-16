#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "settings.h"
#include "bspline.h"

#include <QAbstractButton>
#include <QDialog>


namespace Ui {
class Parameters;
}


class Parameters : public QDialog
{
    Q_OBJECT

public:
    explicit Parameters(QList<BSpline> &splines, Settings &settings, QWidget *parent = nullptr);
    ~Parameters();

signals:
    void parametersDone(void *settings, void *splines);

private slots:
    void newSpline();
    void deleteSpline();
    void addDot();
    void moveDot();
    void removeDot();
    void zoomIn();
    void zoomOut();

    void on_i_number_valueChanged(int value);
    void on_i_a_editingFinished();
    void on_i_b_editingFinished();
    void on_i_c_editingFinished();
    void on_i_d_editingFinished();
    void on_i_zn_editingFinished();
    void on_i_zf_editingFinished();
    void on_i_R_editingFinished();
    void on_i_G_editingFinished();
    void on_i_B_editingFinished();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Parameters *ui;
    Settings *settings;
    QList<BSpline> *splines;

    QAction *actionAdd, *actionMove, *actionRemove;
};

#endif // PARAMETERS_H
