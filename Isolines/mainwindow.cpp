#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    colors{QColor(255,0,0),QColor(0,255,0),QColor(0,0,255),QColor(125,125,0),QColor(125,0,125),QColor(0,125,125)},
    plot(new Plot(QColor(255,255,255),colors,{-5,5,-5,5,100,100},true,this)),
    legend(new Legend(colors,plot,this))
{
    plot->setMouseTracking(true);
    legend->setMaximumHeight(60);
    QScrollArea * scroll_area = new QScrollArea(this);
    QWidget * central_widget = new QWidget(this);
    QVBoxLayout * central_layout = new QVBoxLayout(central_widget);
    resize(800, 600);
    central_layout->addWidget(plot);
    central_layout->addWidget(legend);
    scroll_area->setWidget(central_widget);
    scroll_area->setWidgetResizable(true);
    scroll_area->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    setCentralWidget(scroll_area);
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menuFile = new QMenu(menuBar);
    QMenu *menuSettings = new QMenu(menuBar);
    QMenu *menuHelp = new QMenu(menuBar);
    setMenuBar(menuBar);

    QAction *actionSettings = new QAction(this);
    QAction *actionNew= new QAction(this);
    //QAction *actionSave= new QAction(this);
    QAction *actionLoad= new QAction(this);
    QAction *actionAbout= new QAction(this);
    QAction *actionModeColorMap = new QAction(this);
    QAction *actionModeInterpolate= new QAction(this);
    QAction *actionModeIsolines = new QAction(this);
    QAction *actionModeGrid = new QAction(this);

    actionModeGrid->setCheckable(true);
    actionModeColorMap->setCheckable(true);
    actionModeColorMap->setChecked(true);
    actionModeInterpolate->setCheckable(true);
    actionModeIsolines->setCheckable(true);



    actionNew->setText("NewDocument");
    actionLoad->setText("LoadFile");
   // actionSave->setText("SaveFile");
    actionSettings->setText("Settings");
    actionAbout->setText("Info");
    actionModeGrid->setText("Grid");
    actionModeColorMap->setText("Color map");
    actionModeInterpolate->setText("Interpolation");
    actionModeIsolines->setText("Isolines");

    QToolBar *mainToolBar = new QToolBar("Show toolbar", this);
    addToolBar(Qt::TopToolBarArea, mainToolBar);
    insertToolBarBreak(mainToolBar);



    menuFile->addAction(actionNew);
    menuFile->addAction(actionLoad);

    menuSettings->addAction(actionSettings);
    menuSettings->addAction(actionModeGrid);
    menuSettings->addAction(actionModeColorMap);
    menuSettings->addAction(actionModeInterpolate);
    menuSettings->addAction(actionModeIsolines);


    menuHelp->addAction(actionAbout);

    mainToolBar->addAction(actionNew);
    //mainToolBar->addAction(actionSave);
    mainToolBar->addAction(actionLoad);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionModeGrid);
    mainToolBar->addAction(actionModeColorMap);
    mainToolBar->addAction(actionModeInterpolate);
    mainToolBar->addAction(actionModeIsolines);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionSettings);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionAbout);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuSettings->menuAction());
    menuBar->addAction(menuHelp->menuAction());

    menuFile->setTitle("File");
    menuSettings->setTitle("Settings");
    menuHelp->setTitle("Help");

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    connect(actionModeGrid, SIGNAL(triggered(bool)),plot,SLOT(setModeGrid(bool)));
    connect(actionModeColorMap, SIGNAL(triggered(bool)),plot,SLOT(setModeColorMap(bool)));
    connect(actionModeIsolines, SIGNAL(triggered(bool)),plot,SLOT(setModeIsolines(bool)));
    connect(actionModeInterpolate, SIGNAL(triggered(bool)),plot,SLOT(setModeInterpolate(bool)));

    connect(actionModeInterpolate, SIGNAL(triggered(bool)),legend,SLOT(update()));
    connect(actionModeColorMap, SIGNAL(triggered(bool)),legend,SLOT(update()));

    connect(actionAbout, SIGNAL(triggered(bool)), this, SLOT(showInfo()));
    connect(actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettings()));
    connect(actionLoad, SIGNAL(triggered(bool)),this, SLOT(open()));
    connect(actionModeColorMap, SIGNAL(triggered(bool)), actionModeInterpolate, SLOT(toggle()));
    connect(actionModeInterpolate, SIGNAL(triggered(bool)), actionModeColorMap, SLOT(toggle()));

    connect(plot,SIGNAL(changeStatusBar(double,double,double)),this,SLOT(updateStatusBar(double,double,double)));

}

MainWindow::~MainWindow()
{
}
void MainWindow::openSettings(){

    ParametrsIsolines param = plot->getParametrs();
    Settings settings(this, &param);
    settings.exec();
    plot->setParametrs(param);
}
void MainWindow::showInfo(){
    About about(this);
    about.exec();
}

void MainWindow::open(){
    int k;
    int m;
    int n;

    if(filename.length() == 0){
        filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "FIT_16208_ELISEEV_ISOLINES_Data/");
    }
    else{
        filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                   filename);
    }
    QStringList list = filename.split('/');
    list.pop_back();
    if (filename.length() == 0) {
       return;
    }
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly)) {
        QErrorMessage error_message;
        error_message.setModal(true);
        error_message.showMessage(QString("Cannot open file"));
        error_message.exec();

        return;
    }
    QTextStream file(&f);

    QString line = file.readLine();
    QTextStream str(&line);
    str >> k;
    str >> m;
    line = file.readLine();
    QTextStream str2(&line);

    str2 >> n;
    int r, g,b;
    colors = std::vector<QColor>(n+1);
    for (int i = 0; i < n+1; ++i) {
        line = file.readLine();
        QTextStream str3(&line);
        str3 >> r >> g >> b;
        colors[i].setRed(r);
        colors[i].setGreen(g);
        colors[i].setBlue(b);
    }
    int riz,giz,biz;
    line = file.readLine();
    QTextStream str4(&line);
    str4 >> riz >> giz >> biz;
    colorLine.setRed(riz);
    colorLine.setGreen(giz);
    colorLine.setBlue(biz);
    filename = list.join('/');
    ParametrsIsolines parametrs = plot->getParametrs();
    parametrs.k = k;
    parametrs.m = m;

    plot->setColors(colors,colorLine);
    plot->setParametrs(parametrs);
}

void MainWindow::updateStatusBar(double x, double y, double value){
    statusBar->showMessage(QString("%1 : (%2; %3), value: %4").arg("Current position").arg(x).arg(y).arg(value));
}



