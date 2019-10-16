#include "settingsview.h"

#include <numeric>
#include <string>
#include "utils.h"

SettingsView::SettingsView(ViewSettings *viewSettings, Settings *modelSettings, QWidget *parent) :
    QDialog(parent),
    viewSettings(viewSettings),
    modelSettings(modelSettings)
{

    QGroupBox *mode = initMouseMode();
    QGroupBox *sizeField = initSizeField();
    QGroupBox *lineParameters = initLineParameters();
    QGroupBox *gameOptions= initGameOptions();
    QDialogButtonBox *buttons = initButtons();

    QGridLayout *mainLayout = new QGridLayout();

    mainLayout->addWidget(sizeField, 0, 0);
    mainLayout->addWidget(mode, 0, 1);
    mainLayout->addWidget(lineParameters, 0, 2);
    mainLayout->addWidget(gameOptions, 1, 0, 1, 3);
    mainLayout->addWidget(buttons, 2, 0, 1, 3);

    setLayout(mainLayout);
    setWindowTitle("Settings");

    setValue();

}


void SettingsView::setValue(){
    buttonXOR->setChecked(viewSettings->modeXOR);
    buttonReplace->setChecked(!viewSettings->modeXOR);
    m->setValue(modelSettings->m);
    n->setValue(modelSettings->n);
    lengthEdge->setValue(viewSettings->lengthEdge);
    widthEdge->setValue(viewSettings->widthEdge);

    QLocale locale;
    QChar point = locale.decimalPoint();

    firstImpact->setText(QString::number((double)modelSettings->fstImpact).replace(".", point));
    secondImpact->setText(QString::number((double)modelSettings->sndImpact).replace(".", point));
    liveBegin->setText(QString::number((double)modelSettings->liveBegin).replace(".", point));
    liveEnd->setText(QString::number((double)modelSettings->liveEnd).replace(".", point));
    birthBegin->setText(QString::number((double)modelSettings->birthBegin).replace(".", point));
    birthEnd->setText(QString::number((double)modelSettings->birthEnd).replace(".", point));
}
void SettingsView::checkInput()
{
    convertStrToFloat(firstImpact->text(), &modelSettings->fstImpact);
    convertStrToFloat(secondImpact->text(), &modelSettings->sndImpact);
    convertStrToFloat(liveBegin->text(), &modelSettings->liveBegin);
    convertStrToFloat(liveEnd->text(), &modelSettings->liveEnd);
    convertStrToFloat(birthBegin->text(), &modelSettings->birthBegin);
    convertStrToFloat(birthEnd->text(), &modelSettings->birthEnd);

    if (modelSettings->liveBegin > modelSettings->birthBegin ||
            modelSettings->birthBegin > modelSettings->birthEnd ||
            modelSettings->birthEnd > modelSettings->liveEnd){
        QMessageBox message;
        message.setText("LIVE_BEGIN ≤ BIRTH_BEGIN ≤ BIRTH_END ≤ LIVE_END");
        message.setIcon(QMessageBox::Warning);
        message.exec();
        return;
    }

    viewSettings->modeXOR = buttonXOR->isChecked();
    viewSettings->lengthEdge = lengthEdge->value();
    viewSettings->widthEdge = widthEdge->value();
    modelSettings->m = m->value();
    modelSettings->n = n->value();

    accept();
}



void SettingsView::convertStrToFloat(QString string, float *num)
{
    double temp = std::stod(string.toStdString());
    *num = qRound(10*temp);
    *num /= 10;
}

QGroupBox* SettingsView::initMouseMode(){
    QVBoxLayout *mouseModeLayout = new QVBoxLayout();
    buttonReplace = new QRadioButton("Replace", this);
    buttonXOR = new QRadioButton("XOR", this);

    mouseModeLayout->addWidget(buttonReplace);
    mouseModeLayout->addWidget(buttonXOR);


    QGroupBox* mouseMode = new QGroupBox("Mode", this);
    mouseMode->setLayout(mouseModeLayout);
    return mouseMode;
}

QGroupBox* SettingsView::initSizeField(){
    QGridLayout *sizeFieldLayout = new QGridLayout();
    m = new QSpinBox(this);
    n = new QSpinBox(this);
    m->setRange(2, 100);
    n->setRange(2, 100);

    sizeFieldLayout->addWidget(new QLabel("m", this), 0, 0);
    sizeFieldLayout->addWidget(new QLabel("n", this), 1, 0);
    sizeFieldLayout->addWidget(m, 0, 1);
    sizeFieldLayout->addWidget(n, 1, 1);
    QGroupBox *sizeField = new QGroupBox("Size Field", this);
    sizeField->setLayout(sizeFieldLayout);
    return sizeField;
}

QGroupBox* SettingsView::initLineParameters(){
    QGridLayout *lineParametersLayout= new QGridLayout();

    lengthEdgeSlider = new QSlider(Qt::Horizontal, this);
    lengthEdge = new QSpinBox(this);
    lengthEdgeSlider->setRange(5, 50);
    lengthEdge->setRange(5, 50);

    lineParametersLayout->addWidget(new QLabel("Length line", this), 0, 0);
    lineParametersLayout->addWidget(lengthEdge, 0, 1);
    lineParametersLayout->addWidget(lengthEdgeSlider, 0, 2);

    QSlider *widthEdgeSlider = new QSlider(Qt::Horizontal, this);
    widthEdge = new QSpinBox();
    widthEdgeSlider->setRange(1, 15);
    widthEdge->setRange(1, 15);

    lineParametersLayout->addWidget(new QLabel("Width line", this), 1, 0);
    lineParametersLayout->addWidget(widthEdge, 1, 1);
    lineParametersLayout->addWidget(widthEdgeSlider, 1, 2);

    connect(lengthEdge, SIGNAL(valueChanged(int)), lengthEdgeSlider, SLOT(setValue(int)));
    connect(lengthEdgeSlider, SIGNAL(sliderMoved(int)), lengthEdge, SLOT(setValue(int)));
    connect(widthEdge, SIGNAL(valueChanged(int)), widthEdgeSlider, SLOT(setValue(int)));
    connect(widthEdgeSlider, SIGNAL(sliderMoved(int)), widthEdge, SLOT(setValue(int)));
    QGroupBox* lineParameters = new QGroupBox("Line Parameters", this);
    lineParameters->setLayout(lineParametersLayout);
    return lineParameters;
}
QGroupBox* SettingsView::initGameOptions(){
    QGridLayout *gameOptionsLayout= new QGridLayout(this);
    firstImpact = new QLineEdit(this);
    secondImpact = new QLineEdit(this);
    liveBegin = new QLineEdit(this);
    liveEnd = new QLineEdit(this);
    birthBegin = new QLineEdit(this);
    birthEnd = new QLineEdit(this);

    QDoubleValidator *validator = new QDoubleValidator(0,50,1, this);
    validator->setNotation(QDoubleValidator::StandardNotation);

    firstImpact->setValidator(validator);
    secondImpact->setValidator(validator);
    liveBegin->setValidator(validator);
    liveEnd->setValidator(validator);
    birthBegin->setValidator(validator);
    birthEnd->setValidator(validator);

    gameOptionsLayout->addWidget(new QLabel("FIRST_IMPACT", this), 0, 0);
    gameOptionsLayout->addWidget(new QLabel("SECOND_IMPACT", this), 0, 1);
    gameOptionsLayout->addWidget(new QLabel("LIVE_BEGIN", this), 0, 2);
    gameOptionsLayout->addWidget(new QLabel("LIVE_END", this), 0, 3);
    gameOptionsLayout->addWidget(new QLabel("BIRTH_BEGIN", this), 0, 4);
    gameOptionsLayout->addWidget(new QLabel("BIRTH_END", this), 0, 5);
    gameOptionsLayout->addWidget(firstImpact, 1, 0);
    gameOptionsLayout->addWidget(secondImpact, 1, 1);
    gameOptionsLayout->addWidget(liveBegin, 1, 2);
    gameOptionsLayout->addWidget(liveEnd, 1, 3);
    gameOptionsLayout->addWidget(birthBegin, 1, 4);
    gameOptionsLayout->addWidget(birthEnd, 1, 5);

    QGroupBox *gameOptions= new QGroupBox("Game Options", this);
    gameOptions->setLayout(gameOptionsLayout);
    return gameOptions;
}

QDialogButtonBox* SettingsView::initButtons(){
    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setOrientation(Qt::Horizontal);
    buttons->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(checkInput()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    return buttons;
}




