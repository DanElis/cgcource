#ifndef SETTINGS_H
#define SETTINGS_H
#include <QGridLayout>
#include <QDialog>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QObject>
#include <QWidget>

struct ParametrsIsolines{
    double a;
    double b;
    double c;
    double d;
    int k;
    int m;
};

class Settings : public QDialog
{
    Q_OBJECT
public:
    Settings(QWidget *parent, ParametrsIsolines* parametrsIsolines);
    QGroupBox* initParametrsLayout();
    QDialogButtonBox* initButtons();
public slots:
    void checkInput();
private:
    ParametrsIsolines* parametrs;
    QDoubleSpinBox* paramA;
    QDoubleSpinBox* paramB;
    QDoubleSpinBox* paramC;
    QDoubleSpinBox* paramD;
    QSpinBox* paramM;
    QSpinBox* paramK;
};

#endif // SETTINGS_H
