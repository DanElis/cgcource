#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    model = new Model(defaultModelSettings);
    view = new View(this, &model, defaultViewSettings);
    controller = new Controller(&model,&view,this);
    this->setMinimumSize(800,600);



    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *horizontalLayout = new QHBoxLayout();

    verticalLayout->setContentsMargins(0,0,0,0);
    horizontalLayout->setContentsMargins(0,0,0,0);
    verticalLayout->setSpacing(0);
    horizontalLayout->setSpacing(0);

    QScrollBar *verticalScrollBar = new QScrollBar(centralWidget);
    QScrollBar *horizontalScrollBar = new QScrollBar(centralWidget);
    verticalScrollBar->setOrientation(Qt::Vertical);
    horizontalScrollBar->setOrientation(Qt::Horizontal);
    verticalScrollBar->setRange(1, 10);
    horizontalScrollBar->setRange(1, 10);

    horizontalLayout->addWidget(view);
    horizontalLayout->addWidget(verticalScrollBar);
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addWidget(horizontalScrollBar);

    QAction *actionNew = new QAction(this);
    QAction *actionLoad = new QAction(this);
    QAction *actionSave = new QAction(this);
    QAction *actionExit = new QAction(this);
    QAction *actionNextState = new QAction(this);
    actionRun = new QAction(this);
    QAction *actionClear = new QAction(this);
    actionReplace = new QAction(this);
    actionXOR = new QAction(this);
    actionImpact = new QAction(this);
    QAction *actionSettings = new QAction(this);
    QAction *actionAbout = new QAction(this);

    actionNew->setIcon(QIcon(":/images/New.png"));
    actionLoad->setIcon(QIcon(":/images/Open.png"));
    actionReplace->setIcon(QIcon(":/images/Replace_mode"));
    actionSave->setIcon(QIcon(":/images/Save.png"));
    actionNextState->setIcon(QIcon(":/images/Step.png"));
    actionRun->setIcon(QIcon(":/images/Start.png"));
    actionClear->setIcon(QIcon(":/images/Clear.png"));
    actionXOR->setIcon(QIcon(":/images/XOR_mode.png"));
    actionImpact->setIcon(QIcon(":/images/Impact.png"));
    actionSettings->setIcon(QIcon(":/images/Settings.png"));
    actionAbout->setIcon(QIcon(":/images/About.png"));


    actionRun->setCheckable(true);
    actionReplace->setCheckable(true);
    actionReplace->setChecked(true);
    actionXOR->setCheckable(true);
    actionImpact->setCheckable(true);

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *menuFile = new QMenu(menuBar);
    QMenu *menuEdit = new QMenu(menuBar);
    QMenu *menuView = new QMenu(menuBar);
    QMenu *menuSimulation = new QMenu(menuBar);
    QMenu *menuHelp = new QMenu(menuBar);
    setMenuBar(menuBar);

    QToolBar *mainToolBar = new QToolBar("Show toolbar", this);
    addToolBar(Qt::TopToolBarArea, mainToolBar);
    insertToolBarBreak(mainToolBar);
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    menuBar->addAction(menuFile->menuAction());
    menuBar->addAction(menuEdit->menuAction());
    menuBar->addAction(menuView->menuAction());
    menuBar->addAction(menuSimulation->menuAction());
    menuBar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionNew);
    menuFile->addAction(actionLoad);
    menuFile->addAction(actionSave);
    menuFile->addSeparator();
    menuFile->addAction(actionExit);
    menuEdit->addAction(actionReplace);
    menuEdit->addAction(actionXOR);
    menuEdit->addSeparator();
    menuEdit->addAction(actionSettings);
    menuView->addAction(actionImpact);
    menuSimulation->addAction(actionNextState);
    menuSimulation->addAction(actionRun);
    menuSimulation->addSeparator();
    menuSimulation->addAction(actionClear);
    menuHelp->addAction(actionAbout);

    mainToolBar->addAction(actionNew);
    mainToolBar->addAction(actionSave);
    mainToolBar->addAction(actionLoad);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionImpact);
    mainToolBar->addAction(actionSettings);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionReplace);
    mainToolBar->addAction(actionXOR);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionClear);
    mainToolBar->addAction(actionNextState);
    mainToolBar->addAction(actionRun);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionAbout);

    actionNew->setText("NewDocument");
    actionLoad->setText("LoadFile");
    actionSave->setText("SaveFile");
    actionExit->setText("Exit");
    actionNextState->setText("NextState");
    actionRun->setText("Run");
    actionClear->setText("Clear");
    actionReplace->setText("Replace");
    actionXOR->setText("XOR");
    actionImpact->setText("Impact");
    actionSettings->setText("Settings");
    actionAbout->setText("Info");

    actionNew->setToolTip("Создать новую модель");
    actionLoad->setToolTip("Загрузить модель");
    actionSave->setToolTip("Сохранить модель");
    actionNextState->setToolTip("Следующий шаг");
    actionRun->setToolTip("Запустить итеративный процесс");
    actionClear->setToolTip("Очистить поле");
    actionReplace->setToolTip("В этом режиме любая клетка при клике на нее становится \"живой\" ");
    actionXOR->setToolTip("В этом режиме клик меняет состояние клетки на противоположное");
    actionImpact->setToolTip("Отобразить значения воздействия");
    actionSettings->setToolTip("Открыть настройки");
    actionAbout->setToolTip("Информация о авторе");

    menuFile->setTitle("File");
    menuEdit->setTitle("Edit");
    menuView->setTitle("View");
    menuSimulation->setTitle("Simulation");
    menuHelp->setTitle("Help");

    connect(actionNew, SIGNAL(triggered(bool)), this, SLOT(createNew()));
    connect(actionSave, SIGNAL(triggered(bool)), this, SLOT(save()));
    connect(actionLoad, SIGNAL(triggered(bool)), this, SLOT(open()));
    connect(actionAbout, SIGNAL(triggered(bool)), controller, SLOT(showInfo()));
    connect(actionClear, SIGNAL(triggered(bool)), controller, SLOT(clear()));
    connect(actionNextState, SIGNAL(triggered(bool)),controller, SLOT(nextState()));
    connect(actionRun, SIGNAL(toggled(bool)),controller, SLOT(changeTimer(bool)));
    connect(actionSettings, SIGNAL(triggered(bool)), this, SLOT(openSettings()));

    connect(actionXOR, SIGNAL(toggled(bool)), view, SLOT(setXORMode(bool)));
    connect(actionImpact, SIGNAL(toggled(bool)), this, SLOT(setImpact(bool)));

    connect(actionReplace, SIGNAL(triggered(bool)), actionXOR, SLOT(toggle()));
    connect(actionXOR, SIGNAL(triggered(bool)), actionReplace, SLOT(toggle()));

    connect(actionExit, SIGNAL(triggered(bool)), this, SLOT(close()));

    connect(view, SIGNAL(changeSizeImageX(int,int)), horizontalScrollBar, SLOT(setRange(int,int)));
    connect(view, SIGNAL(changeSizeImageY(int,int)), verticalScrollBar, SLOT(setRange(int,int)));

    connect(horizontalScrollBar, SIGNAL(sliderMoved(int)), view, SLOT(setShiftScrollX(int)));
    connect(verticalScrollBar, SIGNAL(sliderMoved(int)), view, SLOT(setShiftScrollY(int)));
    connect(view,SIGNAL(setVisiableVerticalScroll(bool)),verticalScrollBar,SLOT(setVisible(bool)) );
    connect(view,SIGNAL(setVisiableHorizontalScroll(bool)),horizontalScrollBar,SLOT(setVisible(bool)) );
    view->changeSizeScrolls();
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete model;
    delete view;
    delete controller;
}


void MainWindow::resizeEvent(QResizeEvent *event){
    int width = event->size().width();
    int height = event->size().height();
    view->changeSizeScreen(width,height);
}
void MainWindow::changeEvent(QEvent* e){
    if (e->type() == QEvent::WindowStateChange){
        view->changeSizeScreen(this->width(),this->height());
    }
}
void MainWindow::closeEvent(QCloseEvent *e)
{
    if(model->isFileSaved())
        e->accept();
    if (QMessageBox::Cancel == showSaveDialog()) {
        e->ignore();
        return;
    }
    e->accept();
}
void MainWindow::openSettings(){
    Settings modelSettings = model->getSettings();

    ViewSettings viewSettings = view->getSettings();
    SettingsView settings(&viewSettings, &modelSettings, this);
    settings.exec();
    if(view->getSettings().modeXOR != viewSettings.modeXOR)
        actionXOR->trigger();
    if(viewSettings.lengthEdge < 10 && actionImpact->isChecked()){
        actionImpact->trigger();
    }
    model->setSettings(modelSettings);
    view->setSettings(viewSettings);
}

void MainWindow::createNew(){
    if (QMessageBox::Cancel == showSaveDialog()) {
        return;
    }
    model->setSettings(defaultModelSettings);
    view->setSettings(defaultViewSettings);
    model->clear();
    actionRun->setChecked(false);
    actionReplace->setChecked(true);
    model->clear();
    view->update();
}



int MainWindow::showSaveDialog() {
    QMessageBox msg_box(this);
    msg_box.setText("The document has been modified.");
    msg_box.setInformativeText("Do you want to save your changes?");
    msg_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msg_box.setDefaultButton(QMessageBox::Save);
    int ret = msg_box.exec();
    if (ret == QMessageBox::Save) {
        save();
    }
    return ret;
}

void MainWindow::save(){
    QString filename = QFileDialog::getSaveFileName(this,tr("Save File"),
                                                    "../Life/FIT_16208_ELISEEV_LIFE_Data/");
    if (filename.length() == 0) {
        return;
    }
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly)) {
        QErrorMessage error_message;
        error_message.setModal(true);
        error_message.showMessage(QString("Cannot open file"));
        error_message.exec();
        return;
    }
    QTextStream file(&f);
    model->saveFile(file, view->getSettings().widthEdge,view->getSettings().lengthEdge);
}
void MainWindow::open(){
    int lengthEdge;
    int widthEdge;
    int m;
    int n;
    if(filename.length() == 0){
        filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "../Life/FIT_16208_ELISEEV_LIFE_Data/");// launch occurs from the directory
                                                                     // /g16208_Eliseev/build-Life-Desktop-Debug
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
    str >> m;
    str >> n;
    line = file.readLine();
    QTextStream str2(&line);
    str2 >> widthEdge;
    line = file.readLine();
    QTextStream str3(&line);
    str3 >> lengthEdge;
    line = file.readLine();
    QTextStream str4(&line);
    int count;
    str4 >> count;
    model->setSize(n, m);
    int x, y;
    for (int i = 0; i < count; ++i) {
        line = file.readLine();
        QTextStream str5(&line);
        str5 >> x >> y;
        model->setCell(x,y,true);
    }
    view->setLineParameters(lengthEdge, widthEdge);
    view->update();
    filename = list.join('/');
}

void MainWindow::setImpact(bool state){
    if(view->getSettings().lengthEdge < 10 && state){
        QMessageBox msg_box(this);
        msg_box.setText("Warning");
        msg_box.setInformativeText("Impossible to set mode because cell size is small");
        msg_box.setStandardButtons(QMessageBox::Ok);
        msg_box.setDefaultButton(QMessageBox::Ok);
        msg_box.exec();
    }
    else{
        view->setImpact(state);
    }
}
