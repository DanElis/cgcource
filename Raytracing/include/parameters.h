#ifndef PARAMETERS2_H
#define PARAMETERS2_H

#include "settings.h"

#include <QAbstractButton>
#include <QDialog>


namespace Ui {
class Parameters;
}


class Parameters : public QDialog
{
    Q_OBJECT

public:
    explicit Parameters(Settings &settings, QWidget *parent = nullptr);
    ~Parameters();

signals:
    void parametersDone(void *settings);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Parameters *ui;
    Settings *settings;
};

#endif // PARAMETERS2_H
