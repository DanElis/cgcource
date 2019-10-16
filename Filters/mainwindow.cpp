#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QWidget* centralWidget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(centralWidget);
    ui->setupUi(this);
    setCentralWidget(centralWidget);
    LoadingZone *zoneA = new LoadingZone("zone A",this);
    //layout->addWidget(zoneA, 0, 0, Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionBlack_and_white_triggered()
{

}
