#include "settings.h"

Settings::Settings(QWidget *parent, ParametrsIsolines* parametrsIsolines):
    QDialog(parent),
    parametrs(parametrsIsolines)
{
    QGridLayout *mainLayout = new QGridLayout();
    QGroupBox* isolinesParameters = initParametrsLayout();
    QDialogButtonBox *buttons = initButtons();
    mainLayout->addWidget(isolinesParameters, 0, 0);
    mainLayout->addWidget(buttons, 1,0);
    setLayout(mainLayout);
    setWindowTitle("Settings");
}

QGroupBox* Settings::initParametrsLayout(){
    paramA = new QDoubleSpinBox(this);
    paramA->setMinimum(-500000);
    paramA->setRange(-500000, 500000);
    paramA->setValue(parametrs->a);

    paramB = new QDoubleSpinBox(this);
    paramB->setMinimum(-500000);
    paramB->setRange(-500000, 500000);
    paramB->setValue(parametrs->b);

    paramC = new QDoubleSpinBox(this);
    paramC->setMinimum(-500000);
    paramC->setRange(-500000, 500000);
    paramC->setValue(parametrs->c);

    paramD = new QDoubleSpinBox(this);
    paramD->setMinimum(-500000);
    paramD->setRange(-500000, 500000);
    paramD->setValue(parametrs->d);

    paramK = new QSpinBox(this);
    paramK->setRange(1, 500000);
    paramK->setValue(parametrs->k);

    paramM = new QSpinBox(this);
    paramM->setRange(1, 500000);
    paramM->setValue(parametrs->m);
    QGridLayout *parametersLayout= new QGridLayout();
    parametersLayout->addWidget(new QLabel("a", this), 0, 0);
    parametersLayout->addWidget(paramA, 0, 1);


    parametersLayout->addWidget(new QLabel("b", this), 1, 0);;
    parametersLayout->addWidget(paramB, 1, 1);

    parametersLayout->addWidget(new QLabel("c", this), 2, 0);
    parametersLayout->addWidget(paramC, 2, 1);

    parametersLayout->addWidget(new QLabel("d", this), 3, 0);
    parametersLayout->addWidget(paramD, 3, 1);


    parametersLayout->addWidget(new QLabel("k", this), 4, 0);
    parametersLayout->addWidget(paramK, 4, 1);

    parametersLayout->addWidget(new QLabel("m", this), 5, 0);
    parametersLayout->addWidget(paramM, 5, 1);

    QGroupBox* isolinesParameters = new QGroupBox("Isolines Parameters", this);
    isolinesParameters->setLayout(parametersLayout);
    return isolinesParameters;
}

QDialogButtonBox* Settings::initButtons(){
    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setOrientation(Qt::Horizontal);
    buttons->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(checkInput()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    return buttons;
}

void Settings::checkInput(){
    if (paramA->value() >= paramB->value() ||
            paramC->value() >= paramD->value()){
            QMessageBox message;
            message.setText("a < b, c < d");
            message.setIcon(QMessageBox::Warning);
            message.exec();
            return;
        }
    parametrs->a = paramA->value();
    parametrs->b = paramB->value();
    parametrs->c = paramC->value();
    parametrs->d = paramD->value();
    parametrs->k = paramK->value();
    parametrs->m = paramM->value();
    accept();
}
