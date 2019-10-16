#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "parameters.h"
#include "parameters2.h"
#include "about.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "default.cpp"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTextStream in(&default_data);
    loadFromStream(in);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    ui->canvas->zeroAngles();

    auto &splines = ui->canvas->splines;
    auto &figures = ui->canvas->figures;

    splines.clear();
    figures.clear();
    splines.push_back(BSpline());

    ui->canvas->updateFigures(&ui->canvas->settings, &figures);
}

void MainWindow::on_actionOpen_triggered()
{
    QDir().mkdir("./FIT_16208_ELISEEV_Wireframe_Data/");
    auto name = QFileDialog::getOpenFileName(this, QString(), "./FIT_16208_ELISEEV_Wireframe_Data/", "Text files (*.txt)");

    if (name == "")
        return;

    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, windowTitle(), "Failed to open file:\n" + name);
        return;
    }

    QTextStream in(&file);
    if (!loadFromStream(in)) {
        QMessageBox::critical(this, windowTitle(), "Wrong format of file!");
        return;
    }

    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    QDir().mkdir("./FIT_16208_ELISEEV_Wireframe_Data/");
    auto name = QFileDialog::getSaveFileName(this, QString(), "./FIT_16208_ELISEEV_Wireframe_Data/unnamed-scene.txt", "Text files(.txt)");

    if (name == "")
        return;

    QFile file(name);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, windowTitle(), "Failed to save to file:\n" + name);
        return;
    }

    QTextStream out(&file);
    saveToStream(out);

    file.close();
}

void MainWindow::on_actionZero_angles_triggered()
{
    ui->canvas->zeroAngles();
}

void MainWindow::on_actionSpline_parameters_triggered()
{
    Parameters parameters(ui->canvas->splines, ui->canvas->settings, this);
    connect(&parameters, SIGNAL(parametersDone(void *, void *)), ui->canvas, SLOT(updateSplines(void *, void *)));
    parameters.exec();
    disconnect(&parameters, SIGNAL(parametersDone(void *, void *)), ui->canvas, SLOT(updateSplines(void *, void *)));
}

void MainWindow::on_actionFigure_parameters_triggered()
{
    if (ui->canvas->figures.isEmpty())
        return;

    Parameters2 parameters(ui->canvas->figures, ui->canvas->settings, this);
    connect(&parameters, SIGNAL(parametersDone(void *, void *)), ui->canvas, SLOT(updateFigures(void *, void *)));
    parameters.exec();
    disconnect(&parameters, SIGNAL(parametersDone(void *, void *)), ui->canvas, SLOT(updateFigures(void *, void *)));
}

void MainWindow::on_actionAbout_author_triggered()
{
    AboutDialog about(this);
    about.exec();
}

inline QString readCommentedLine(QTextStream &in)
{
    auto line = in.readLine();
    auto splited = line.split("//");

    return splited[0].simplified();
}

inline bool readUInt(QTextStream &in, uint &i)
{
    auto line = readCommentedLine(in);
    bool ok;
    i = line.toUInt(&ok);
    if (!ok) return false;

    return true;
}

inline bool readUIntPair(QTextStream &in, uint &f, uint &s)
{
    auto line = readCommentedLine(in);
    auto fs = line.split(' ');
    if (fs.size() != 2) return false;

    bool ok1, ok2;
    f = fs[0].toUInt(&ok1);
    s = fs[1].toUInt(&ok2);
    if (!ok1 || !ok2) return false;

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

#define UINT(v,f) uint v; {bool ok; v = f.toUInt(&ok); if (!ok) return false; }
#define DOUBLE(v,f) double v; {bool ok; v = f.toDouble(&ok); if (!ok) return false; }

inline bool readMatrix(QTextStream &in, float *data) {
    return readFloatTriple(in, data[0], data[1], data[2])
        && readFloatTriple(in, data[4], data[5], data[6])
        && readFloatTriple(in, data[8], data[9], data[10]);
}

bool MainWindow::loadFromStream(QTextStream &in)
{
    auto nmkabcd_line = readCommentedLine(in);
    auto nmkabcd = nmkabcd_line.split(' ');
    if (nmkabcd.size() != 7) return false;
    UINT(n,nmkabcd[0]);
    UINT(m,nmkabcd[1]);
    UINT(k,nmkabcd[2]);
    DOUBLE(a,nmkabcd[3]);
    DOUBLE(b,nmkabcd[4]);
    DOUBLE(c,nmkabcd[5]);
    DOUBLE(d,nmkabcd[6]);

    auto znzfswsh_line = readCommentedLine(in);
    auto znzfswsh = znzfswsh_line.split(' ');
    if (znzfswsh.size() != 4) return false;
    DOUBLE(zn,znzfswsh[0]);
    DOUBLE(zf,znzfswsh[1]);
    DOUBLE(sw,znzfswsh[2]);
    DOUBLE(sh,znzfswsh[3]);

    float Mrot_data[16] = { 0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 1 };
    if (!readMatrix(in, Mrot_data)) return false;

    uint bcR, bcG, bcB;
    if (!readUIntTriple(in, bcR, bcG, bcB)) return false;
    bcR = std::min(bcR, 255u); bcG = std::min(bcG, 255u); bcB = std::min(bcB, 255u);

    uint N;
    if (!readUInt(in, N)) return false;


    QList<BSpline> splines;
    QVector<Figure> figures;

    for (int i = 0; i < N; ++i) {
        uint R, G, B;
        if (!readUIntTriple(in, R, G, B)) return false;
        R = std::min(R, 255u); G = std::min(G, 255u); B = std::min(B, 255u);

        BSpline spline;
        Figure figure;

        float Cx, Cy, Cz;
        if (!readFloatTriple(in, Cx, Cy, Cz)) return false;

        float R_data[16] = { 0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 0,
                             0, 0, 0, 1 };
        if (!readMatrix(in, R_data)) return false;

        figure.setC(Cx, Cy, Cz);
        figure.setR(R_data);
        spline.setColors(R, G, B);
        spline.changeLimits(a, b);
        spline.setSizes(300, 300);


        uint K;
        if (!readUInt(in, K)) return false;

        for (int j = 0; j < K; ++j) {
            float x, y;
            if (!readFloatPair(in, x, y)) return false;

            spline.addLast(x, y);
        }

        splines.push_back(spline);
        figures.push_back(figure);
    }


    Settings new_settings(n, m, k, a, b, c, d, zn, zf, sw, sh, 0xFF000000 | (bcR << 16) | (bcG << 8) | (bcB));

    ui->canvas->settings = new_settings;
    ui->canvas->Mrot = Matrix4x4(Mrot_data);
    ui->canvas->splines = splines;
    ui->canvas->figures = figures;

    ui->canvas->updateSplines(&ui->canvas->settings, &ui->canvas->splines);

    return true;
}

bool MainWindow::saveToStream(QTextStream &out)
{
    auto &settings = ui->canvas->settings;
    auto &splines = ui->canvas->splines;
    auto &figures = ui->canvas->figures;

    out << settings.n << ' ' << settings.m << ' ' << settings.k << ' ';
    out << settings.a << ' ' << settings.b << ' ' << settings.c << ' ' << settings.d << endl;
    out << settings.zn << ' ' << settings.zf << ' ' << settings.sw << ' ' << settings.sh << endl;

    float *Mrot_data = ui->canvas->Mrot.data();
    out << Mrot_data[0] << ' ' << Mrot_data[4] << ' ' << Mrot_data[8] << endl;
    out << Mrot_data[1] << ' ' << Mrot_data[5] << ' ' << Mrot_data[9] << endl;
    out << Mrot_data[2] << ' ' << Mrot_data[6] << ' ' << Mrot_data[10] << endl;

    uint bcR = ((settings.background_color & 0x00FF0000) >> 16);
    uint bcG = ((settings.background_color & 0x0000FF00) >> 8);
    uint bcB = ((settings.background_color & 0x000000FF));
    out << bcR << ' ' << bcG << ' ' << bcB << endl;

    out << figures.size() << endl;

    for (auto &figure : figures) {
        out << ((uint) figure.spline().r()) << ' ';
        out << ((uint) figure.spline().g()) << ' ';
        out << ((uint) figure.spline().b()) << endl;

        float x, y, z;
        figure.getC(x, y, z);
        out << x << ' ' << y << ' ' << z << endl;

        const float *R_data = figure.getR().data();
        out << R_data[0] << ' ' << R_data[4] << ' ' << R_data[8] << endl;
        out << R_data[1] << ' ' << R_data[5] << ' ' << R_data[9] << endl;
        out << R_data[2] << ' ' << R_data[6] << ' ' << R_data[10] << endl;


        auto &skeleton = figure.spline().skeleton();
        out << skeleton.size() << endl;

        for (auto &dot : skeleton)
            out << dot.first << ' ' << dot.second << endl;
    }

    return true;
}
