#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>

#include "settings.h"

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
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionZero_angles_triggered();
    void on_actionSpline_parameters_triggered();
    void on_actionFigure_parameters_triggered();
    void on_actionAbout_author_triggered();

private:
    Ui::MainWindow *ui;

    bool loadFromStream(QTextStream &in);
    bool saveToStream(QTextStream &out);
};

#endif // MAINWINDOW_H
