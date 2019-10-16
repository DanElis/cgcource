#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <settings.h>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <about.h>
#include <QFile>
#include <QErrorMessage>
#include <QTextStream>
#include <QFileDialog>
#include <plot.h>
#include <legend.h>
#include <QScrollArea>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
private slots:
    void updateStatusBar(double x, double y, double value);
    void showInfo();
    void openSettings();
    void open();
private:
    QColor colorLine;
    std::vector<QColor> colors;
    Plot* plot;
    QString filename;
    Legend* legend;
    QStatusBar* statusBar;

};

#endif // MAINWINDOW_H
