#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "about.h"
#include "parameters.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>

#include "light.h"
#include "primitives/primitives.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(settingsChanged(void*)), ui->canvas, SLOT(settingsChanged(void*)));
    connect(ui->canvas, SIGNAL(renderingProgress(int)), this, SLOT(renderingProgress(int)));
    connect(ui->canvas, SIGNAL(renderingDone()), this, SLOT(renderingDone()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Right:
        ui->canvas->goRight();
        break;
    case Qt::Key_Left:
        ui->canvas->goLeft();
        break;
    case Qt::Key_Up:
        ui->canvas->goUp();
        break;
    case Qt::Key_Down:
        ui->canvas->goDown();
        break;
    }
}

void MainWindow::renderingProgress(int progress)
{
    ui->statusBar->showMessage(QString("Rendering: %0%").arg(progress));
}

void MainWindow::renderingDone()
{
    ui->actionNew->setEnabled(true);
    ui->actionOpen->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionLoad_rendering_config->setEnabled(true);
    ui->actionSave_rendering_config->setEnabled(true);
    ui->actionInit->setEnabled(true);
    ui->actionWireframe->setEnabled(true);
    ui->actionRender->setEnabled(true);

    ui->statusBar->showMessage("Rendering done");
}

void MainWindow::on_actionNew_triggered()
{
    Settings empty;
    emit settingsChanged(&empty);
}

void MainWindow::on_actionOpen_triggered()
{
    QDir().mkdir("./FIT_16208_ELISEEV_Raytracing_Data/");
    auto name = QFileDialog::getOpenFileName(this, QString(), "./FIT_16208_ELISEEV_Raytracing_Data/", "Scenes (*.scene)");

    if (name == "")
        return;

    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, windowTitle(), "Failed to open file:\n" + name);
        return;
    }
    QTextStream in(&file);


    bool result;

    name.chop(5);
    QFile file2(name.append("render"));
    if (file2.open(QIODevice::ReadOnly)) {
        QTextStream in2(&file2);
        result = loadFromStream(in, &in2);
        file2.close();
    } else
        result = loadFromStream(in);

    file.close();

    if (!result) {
        QMessageBox::critical(this, windowTitle(), "Wrong format of file!");
        return;
    }

    ui->canvas->wireframeMode();
}

void MainWindow::on_actionSave_triggered()
{
    QDir().mkdir("./FIT_16208_ELISEEV_Raytracing_Data/");
    auto name = QFileDialog::getSaveFileName(this, QString(), "./FIT_16208_ELISEEV_Raytracing_Data/", "24-bit BMP file (*.bmp);;PNG file (*.png)");

    if (name == "")
        return;

    if (!ui->canvas->saveImage(name))
        QMessageBox::critical(this, windowTitle(), "Failed to save image:\n" + name);
}

void MainWindow::on_actionLoad_rendering_config_triggered()
{
    QDir().mkdir("./FIT_16208_ELISEEV_Raytracing_Data/");
    auto name = QFileDialog::getOpenFileName(this, QString(), "./FIT_16208_ELISEEV_Raytracing_Data/", "Render config (*.render)");

    if (name == "")
        return;
    if (!name.endsWith(".render"))
        name += ".render";

    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, windowTitle(), "Failed to open file:\n" + name);
        return;
    }
    QTextStream in(&file);


    if (!loadRenderSettingsFromStream(in, ui->canvas->settings))
        QMessageBox::critical(this, windowTitle(), "Wrong format of file!");

    file.close();

    ui->canvas->wireframeMode();
}

void MainWindow::on_actionSave_rendering_config_triggered()
{
    QDir().mkdir("./FIT_16208_ELISEEV_Raytracing_Data/");
    auto name = QFileDialog::getSaveFileName(this, QString(), "./FIT_16208_ELISEEV_Raytracing_Data/unnamed-scene.render", "Render config (.render)");

    if (name == "")
        return;

    QFile file(name);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, windowTitle(), "Failed to save to file:\n" + name);
        return;
    }

    QTextStream out(&file);
    if (!saveRenderSettingsToStream(out, ui->canvas->settings))
        QMessageBox::critical(this, windowTitle(), "Failed to save to file:\n" + name);

    file.close();
}

void MainWindow::on_actionInit_triggered()
{
    ui->canvas->init();
}

void MainWindow::on_actionWireframe_triggered()
{
    ui->canvas->wireframeMode();
}

void MainWindow::on_actionRender_triggered()
{
    if (ui->canvas->settings.deny)
        return;

    ui->actionNew->setEnabled(false);
    ui->actionOpen->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionLoad_rendering_config->setEnabled(false);
    ui->actionSave_rendering_config->setEnabled(false);
    ui->actionInit->setEnabled(false);
    ui->actionWireframe->setEnabled(false);
    ui->actionRender->setEnabled(false);

    ui->canvas->renderScene();
}

void MainWindow::on_actionSettings_triggered()
{
    Parameters parameters(ui->canvas->settings, this);
    connect(&parameters, SIGNAL(parametersDone(void*)), ui->canvas, SLOT(settingsChanged(void*)));
    parameters.exec();
    disconnect(&parameters, SIGNAL(parametersDone(void*)), ui->canvas, SLOT(settingsChanged(void*)));
}

void MainWindow::on_actionAbout_author_triggered()
{
    About about(this);
    about.exec();
}



// Parsing
inline QString readCommentedLine(QTextStream &in)
{
    while (!in.atEnd()) {
        auto line = in.readLine().simplified();
        if (line.isEmpty() || line.startsWith("//"))
            continue;

        return line.split("//")[0].simplified();
    }
    return "";
}

inline bool readUInt(QTextStream &in, uint &i)
{
    auto line = readCommentedLine(in);
    bool ok;
    i = line.toUInt(&ok);
    if (!ok) return false;

    return true;
}

inline bool readUIntTriple(QTextStream &in, uint &f, uint &s, uint &t)
{
    auto line = readCommentedLine(in);
    auto fs = line.split(' ');
    if (fs.size() != 3) return false;

    bool ok1, ok2, ok3;
    f = fs[0].toUInt(&ok1);
    s = fs[1].toUInt(&ok2);
    t = fs[2].toUInt(&ok3);
    if (!ok1 || !ok2 || !ok3) return false;

    return true;
}

inline bool readFloat(QTextStream &in, float &i)
{
    auto line = readCommentedLine(in);
    bool ok;
    i = line.toFloat(&ok);
    if (!ok) return false;

    return true;
}

inline bool readFloatPair(QTextStream &in, float &f, float &s)
{
    auto line = readCommentedLine(in);
    auto fs = line.split(' ');
    if (fs.size() != 2) return false;

    bool ok1, ok2;
    f = fs[0].toFloat(&ok1);
    s = fs[1].toFloat(&ok2);
    if (!ok1 || !ok2) return false;

    return true;
}

inline bool readFloatTriple(QTextStream &in, float &f, float &s, float &t)
{
    auto line = readCommentedLine(in);
    auto fs = line.split(' ');
    if (fs.size() != 3) return false;

    bool ok1, ok2, ok3;
    f = fs[0].toFloat(&ok1);
    s = fs[1].toFloat(&ok2);
    t = fs[2].toFloat(&ok3);
    if (!ok1 || !ok2 || !ok3) return false;

    return true;
}

inline bool readFloat7(QTextStream &in, float &v1, float &v2, float &v3, float &v4, float &v5, float &v6, float &v7)
{
    auto line = readCommentedLine(in);
    auto fs = line.split(' ');
    if (fs.size() != 7) return false;

    bool ok1, ok2, ok3, ok4, ok5, ok6, ok7;
    v1 = fs[0].toFloat(&ok1);
    v2 = fs[1].toFloat(&ok2);
    v3 = fs[2].toFloat(&ok3);
    v4 = fs[3].toFloat(&ok4);
    v5 = fs[4].toFloat(&ok5);
    v6 = fs[5].toFloat(&ok6);
    v7 = fs[6].toFloat(&ok7);
    if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5 || !ok6 || !ok7) return false;

    return true;
}

#define UINT(v,f) uint v; {bool ok; v = f.toUInt(&ok); if (!ok) return false; }
#define FLOAT(v,f) float v; {bool ok; v = f.toFloat(&ok); if (!ok) return false; }
#define POINT(v) float X##v, Y##v, Z##v; if (!readFloatTriple(in, X##v, Y##v, Z##v)) return false;
#define POINT2(v) float X##v, Y##v, Z##v; if (!readFloatTriple(in2, X##v, Y##v, Z##v)) return false;
#define VECTOR(v) Vector3(X##v, Y##v, Z##v)
#define CHARACTERISTICS if (!readFloat7(in, KDr, KDg, KDb, KSr, KSg, KSb, power)) return false;
#define SET_CHARACTERISTICS figure->setLightParameters(KDr, KDg, KDb, KSr, KSg, KSb, power);

bool MainWindow::loadFromStream(QTextStream &in, QTextStream *in2)
{
    uint Ar, Ag, Ab;
    if (!readUIntTriple(in, Ar, Ag, Ab)) return false;

    uint N;
    if (!readUInt(in, N)) return false;


    QVector<Light> lights(N);
    for (int i = 0; i < N; ++i) {
        auto light_line = readCommentedLine(in);
        auto light = light_line.split(' ');
        if (light.size() != 6) return false;
        FLOAT(X,light[0]);
        FLOAT(Y,light[1]);
        FLOAT(Z,light[2]);
        UINT(R,light[3]);
        UINT(G,light[4]);
        UINT(B,light[5]);

        lights[i] = Light(VECTOR(), R / 255.0f, G / 255.0f, B / 255.0f);
    }


    QList< QSharedPointer<Figure> > figures;

    while (!in.atEnd()) {
        auto desc = readCommentedLine(in).toUpper();

        float KDr, KDg, KDb, KSr, KSg, KSb, power;

        if (desc == "SPHERE") {
            float r;
            POINT();
            if (!readFloat(in, r)) return false;
            CHARACTERISTICS;

            auto *figure = new Sphere(VECTOR(), r);
            SET_CHARACTERISTICS;
            figures.push_back(QSharedPointer<Figure>(figure));
        }
        else if (desc == "BOX") {
            POINT(l);
            POINT(h);
            CHARACTERISTICS;

            auto figure = new Box(VECTOR(l), VECTOR(h));
            SET_CHARACTERISTICS;
            figures.push_back(QSharedPointer<Figure>(figure));
        }
        else if (desc == "TRIANGLE") {
            POINT(0);
            POINT(1);
            POINT(2);
            CHARACTERISTICS;

            auto figure = new Triangle(VECTOR(0), VECTOR(1), VECTOR(2));
            SET_CHARACTERISTICS;
            figures.push_back(QSharedPointer<Figure>(figure));
        }
        else if (desc == "QUADRANGLE") {
            POINT(0);
            POINT(1);
            POINT(2);
            POINT(3);
            CHARACTERISTICS;

            auto figure = new Quadrangle(VECTOR(0), VECTOR(1), VECTOR(2), VECTOR(3));
            SET_CHARACTERISTICS;
            figures.push_back(QSharedPointer<Figure>(figure));
        }
        else if (desc != "") return false;
    }


    Settings new_settings(Ar / 255.0f, Ag / 255.0f, Ab / 255.0f, lights, figures);

    if (in2 != nullptr) {
        new_settings.should_init = false;

        if (!loadRenderSettingsFromStream(*in2, new_settings)) return false;
    } else
        emit settingsChanged(&new_settings);

    return true;
}

bool MainWindow::loadRenderSettingsFromStream(QTextStream &in, Settings &settings)
{
    uint Br, Bg, Bb;
    if (!readUIntTriple(in, Br, Bg, Bb)) return false;

    float gamma;
    if (!readFloat(in, gamma)) return false;

    uint depth;
    if (!readUInt(in, depth)) return false;

    auto quality_str = readCommentedLine(in).toLower();
    Settings::Quality quality;
    if (quality_str == "rough")
        quality = Settings::Rough;
    else if (quality_str == "normal")
        quality = Settings::Normal;
    else if (quality_str == "fine")
        quality = Settings::Fine;
    else return false;

    POINT(eye);
    POINT(view);
    POINT(up);

    float zn, zf, sw, sh;
    if (!readFloatPair(in, zn, zf)) return false;
    if (!readFloatPair(in, sw, sh)) return false;


    settings.Br = Br / 255.0f;
    settings.Bg = Bg / 255.0f;
    settings.Bb = Bb / 255.0f;
    settings.gamma = gamma;
    settings.depth = depth;
    settings.quality = quality;
    settings.eye = VECTOR(eye);
    settings.view = VECTOR(view);
    settings.up = VECTOR(up);
    settings.zn = zn;
    settings.zf = zf;
    settings.sw = sw;
    settings.sh = sh;

    emit settingsChanged(&settings);

    return true;
}

bool MainWindow::saveRenderSettingsToStream(QTextStream &out, const Settings &settings)
{
    out << ((uint) (255.0f * settings.Br)) << ' ';
    out << ((uint) (255.0f * settings.Bg)) << ' ';
    out << ((uint) (255.0f * settings.Bb)) << endl;

    out << settings.gamma << endl;
    out << settings.depth << endl;

    switch (settings.quality) {
    case Settings::Rough:
        out << "rough" << endl;
        break;
    case Settings::Normal:
        out << "normal" << endl;
        break;
    case Settings::Fine:
        out << "fine" << endl;
        break;
    }

    out << settings.eye.x() << ' ' << settings.eye.y() << ' ' << settings.eye.z() << endl;
    out << settings.view.x() << ' ' << settings.view.y() << ' ' << settings.view.z() << endl;
    out << settings.up.x() << ' ' << settings.up.y() << ' ' << settings.up.z() << endl;

    out << settings.zn << ' ' << settings.zf << endl;
    out << settings.sw << ' ' << settings.sh << endl;

    return true;
}
