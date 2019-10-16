#ifndef PARAMETERS2_H
#define PARAMETERS2_H

#include "settings.h"
#include "bspline.h"
#include "figure.h"

#include <QAbstractButton>
#include <QDialog>


namespace Ui {
class Parameters2;
}


class Parameters2 : public QDialog
{
    Q_OBJECT

public:
    explicit Parameters2(QVector<Figure> &figures, Settings &settings, QWidget *parent = nullptr);
    ~Parameters2();

signals:
    void parametersDone(void *settings, void *figures);

private slots:
    void on_i_number_valueChanged(int value);
    void on_i_a_editingFinished();
    void on_i_b_editingFinished();
    void on_i_c_editingFinished();
    void on_i_d_editingFinished();
    void on_i_zn_editingFinished();
    void on_i_zf_editingFinished();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_i_Cx_valueChanged(const QString &arg1);
    void on_i_Cy_valueChanged(const QString &arg1);
    void on_i_Cz_valueChanged(const QString &arg1);
    void on_i_Rx_valueChanged(const QString &arg1);
    void on_i_Ry_valueChanged(const QString &arg1);
    void on_i_Rz_valueChanged(const QString &arg1);
    void on_i_Sx_valueChanged(const QString &arg1);
    void on_i_Sy_valueChanged(const QString &arg1);
    void on_i_Sz_valueChanged(const QString &arg1);


private:
    Ui::Parameters2 *ui;
    Settings *settings;
    QVector<Figure> *figures;

    bool initializing = true;

    void saveSettings();
};

#endif // PARAMETERS2_H
