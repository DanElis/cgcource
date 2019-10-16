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

signals:
    void settingsChanged(void *);

public slots:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void renderingProgress(int progress);
    void renderingDone();

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_rendering_config_triggered();
    void on_actionSave_rendering_config_triggered();
    void on_actionInit_triggered();
    void on_actionWireframe_triggered();
    void on_actionRender_triggered();
    void on_actionSettings_triggered();
    void on_actionAbout_author_triggered();


private:
    Ui::MainWindow *ui;

    bool loadFromStream(QTextStream &in, QTextStream *in2 = nullptr);
    bool loadRenderSettingsFromStream(QTextStream &in, Settings &settings);
    bool saveRenderSettingsToStream(QTextStream &out, const Settings &settings);
};

#endif // MAINWINDOW_H
