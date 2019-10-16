#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include "view.h"
#include "model.h"

class SettingsView : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsView(ViewSettings *viewSettings, Settings *modelSettings, QWidget *parent = 0);

private slots:
    void checkInput();

private:
    QGroupBox* initMouseMode();
    QGroupBox* initSizeField();
    QGroupBox* initLineParameters();
    QGroupBox* initGameOptions();
    QDialogButtonBox* initButtons();
    void setValue();
    void convertStrToFloat(QString string, float *res);

    QSlider *lengthEdgeSlider;
    ViewSettings *viewSettings;
    Settings *modelSettings;

    QRadioButton *buttonReplace;
    QRadioButton *buttonXOR;

    QSpinBox *m;
    QSpinBox *n;
    QSpinBox *lengthEdge;
    QSpinBox *widthEdge;

    QLineEdit *firstImpact;
    QLineEdit *secondImpact;
    QLineEdit *liveBegin;
    QLineEdit *liveEnd;
    QLineEdit *birthBegin;
    QLineEdit *birthEnd;

};


#endif // SETTINGSDIALOG_H
