#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "constant.h"
#include "filterzone.h"
#include "loadingzone.h"
#include <QGridLayout>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionBlack_and_white_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
