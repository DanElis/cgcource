#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include "model.h"

#include <QtCore/QVariant>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <stack>
#include "view.h"
#include "model.h"
#include "controller.h"
#include "settingsview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void changeEvent(QEvent* e) override;
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *event) override ;
public slots:
    void openSettings();
    void save();
    void setImpact(bool state);
    void open();
    void createNew();
private:
    int showSaveDialog();
    Settings modelSettings;
    Model *model;
    View *view;
    QAction *actionXOR;
    QAction *actionRun;
    QAction *actionImpact;
    QAction *actionReplace;
    QString filename;
    Controller *controller;
    Settings defaultModelSettings = {
            20,
            20,
            2.0,
            3.3,
            2.3,
            2.9,
            1.0,
            0.3
        };
    const ViewSettings defaultViewSettings = {1,20,0};
};

#endif // MAINWINDOW_H
