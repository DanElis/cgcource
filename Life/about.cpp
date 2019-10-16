#include "about.h"

#include <QGridLayout>
#include <QtWidgets>

About::About(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("About FIT_16208_ELISEEV_INIT");
    QGridLayout *layout = new QGridLayout(this);

    QLabel *label = new QLabel("Conway's Game of Life\n\n\
Created by Eliseev Daniil\n\
NSU, 2019 ", this);

    label->setWordWrap(true);

    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    buttons->setStandardButtons(QDialogButtonBox::Ok);
    QObject::connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    QLabel *photo = new QLabel(this);
    QPixmap pixmap = QPixmap(":/images/eliseev.jpg").scaledToHeight(180);
    photo->setPixmap(pixmap);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->addWidget(label, 0, 1);
    layout->addWidget(buttons, 1, 1);
    layout->addWidget(photo,0,0);
}
