#include "view.h"

View::View(QWidget *parent, Model **model, const ViewSettings & viewSettings) :
    QWidget(parent),
    image(SIZE_IMAGE,SIZE_IMAGE,format),
    viewSettings(viewSettings),
    model(*model)
{
    widthScreen = 800;
    heightScreen = 5*600/6;
    sizeImageX = sizeImageY = SIZE_IMAGE;
    showImpact = false;
    shiftScroll = QPoint(0,0);
    setShiftX(viewSettings.lengthEdge);
    setShiftY(viewSettings.lengthEdge);
    createCopyImage();

}
void View::paintEvent(QPaintEvent *){
    memset(image.bits(), 0,image.byteCount());
    drawField();
    QPainter painterImage(this);
    painter = &painterImage;
    painter->drawImage(0,0,image,shiftScroll.x(),shiftScroll.y());
}

void View::drawField(){
    QPainter painter(&image);
    int n = model->getSettings().n;
    int m = model->getSettings().m;
    int widthEdge = viewSettings.widthEdge;
    QPoint centre = QPoint(widthEdge/2,widthEdge/2)+shiftX+2*shiftY;

    for (int i = 0; i < n; i++){
        QPoint tmpCentre = centre;
        int curM = i%2 ? (m-1) : m;
        for(int j = 0; j < curM ; j++){
            drawHexagon(widthEdge, tmpCentre, i , &image, &painter);
            if(model->isAlive(j,i)){
                Utils::spanFill(tmpCentre,GREEN, &image);
            }
            else{
                Utils::spanFill(tmpCentre,WHITE, &image);
            }
            if(showImpact && viewSettings.lengthEdge >= 10){
                float impact = model->getImpact(j,i);
                int intImpact = (int)(impact+EPSILON);
                int fractImpact =(int)10*(impact - intImpact + EPSILON);

                painter.drawText(tmpCentre.x()-8,tmpCentre.y() + 3
                                 ,
                             fractImpact == 0 ? QString::number(intImpact) : QString("%1.%2").arg(intImpact).arg(fractImpact));
            }
            tmpCentre += 2*shiftX;
        }
        centre += 3*shiftY+ sign(i)*shiftX;
    }
}
int View::sign(int i){
    return i%2 ? -1 : 1;
}
void View::drawHexagon(const int widthEdge,const QPoint centre,const int lineNumber, QImage *image, QPainter *painter){
    QPoint pointsHexagon[6] = {
        centre-shiftX-shiftY,
        centre-shiftX+shiftY,
        centre+2*shiftY,
        centre+shiftX+shiftY,
        centre+shiftX-shiftY,
        centre-2*shiftY};
    int start = lineNumber%2 ? 5 : 0;
    if(widthEdge == 1){
        Utils::drawLine(pointsHexagon[start + sign(lineNumber)*0],pointsHexagon[start + sign(lineNumber)*1], qRgb(0,0,0), image);
        Utils::drawLine(pointsHexagon[start + sign(lineNumber)*1],pointsHexagon[start + sign(lineNumber)*2], qRgb(0,0,0), image);
        Utils::drawLine(pointsHexagon[start + sign(lineNumber)*2],pointsHexagon[start + sign(lineNumber)*3], qRgb(0,0,0), image);
        Utils::drawLine(pointsHexagon[start + sign(lineNumber)*3],pointsHexagon[start + sign(lineNumber)*4], qRgb(0,0,0), image);
        Utils::drawLine(pointsHexagon[start + sign(lineNumber)*4],pointsHexagon[start + sign(lineNumber)*5], qRgb(0,0,0), image);
        Utils::drawLine(pointsHexagon[start + sign(lineNumber)*5],pointsHexagon[start + sign(lineNumber)*0], qRgb(0,0,0), image);
    }
    else{
        painter->setPen(QPen(Qt::black, widthEdge, Qt::SolidLine, Qt::RoundCap));
        painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
        painter->drawLine(pointsHexagon[start + sign(lineNumber)*0],pointsHexagon[start + sign(lineNumber)*1]);
        painter->drawLine(pointsHexagon[start + sign(lineNumber)*1],pointsHexagon[start + sign(lineNumber)*2]);
        painter->drawLine(pointsHexagon[start + sign(lineNumber)*2],pointsHexagon[start + sign(lineNumber)*3]);
        painter->drawLine(pointsHexagon[start + sign(lineNumber)*3],pointsHexagon[start + sign(lineNumber)*4]);
        painter->drawLine(pointsHexagon[start + sign(lineNumber)*4],pointsHexagon[start + sign(lineNumber)*5]);
        painter->drawLine(pointsHexagon[start + sign(lineNumber)*5],pointsHexagon[start + sign(lineNumber)*0]);
    }

}
void View::setShiftX(const int lengthEdge){
    shiftX = QPoint(lengthEdge*WIDTH_SHIFT,0);
}

void View::setShiftY(const int lengthEdge){
    shiftY = QPoint(0,lengthEdge/2);
}

void View :: mousePressEvent ( QMouseEvent * event ){

    QPoint pos = getPositionHexagon(event->x()+ shiftScroll.x(),event->y()+ shiftScroll.y());
    if(pos.x() == -1)
        return;
    bool newState = true;
    bool oldState = model->isAlive(pos.x(), pos.y());
    if (viewSettings.modeXOR){
        newState = !oldState;
    }

    model->setCell(pos.x(), pos.y(), newState);
    this->update();
}

void View::setXORMode(bool mode){
    viewSettings.modeXOR = mode;
    this->update();
}
void View::setImpact(bool show){

    showImpact = show;
    this->update();
}

QPoint View::getPositionHexagon(int x,int y){
    if(x >= 0)
        x -= 1;
    QColor pos = cpImage.pixelColor(x, y);
    int red  = pos.red();
    int green = pos.green();
    int blue = pos.blue();
    int position = red*255*255+green*255+blue;
    int i = (position-1) % model->getSettings().m ;
    int j = (position-1)/model->getSettings().m;
    return QPoint(i,j);
}

void View::createCopyImage(){
    cpImage = QImage(image.width(),image.height(),image.format());
    QPainter painter(&cpImage);
    int widthEdge = viewSettings.widthEdge;
    int n = model->getSettings().n;
    int m = model->getSettings().m;
    QPoint centre = QPoint(widthEdge/2,widthEdge/2)+shiftX+2*shiftY;
    for (int i = 0; i < n; i++){
        QPoint tmpCentre = centre;
        int curM = i%2 ? (m-1) : m;
        for(int j = 0; j < curM ; j++){
            drawHexagon(widthEdge, tmpCentre, i , &cpImage, &painter);
            int pos = i*m+(j+1);
            Utils::spanFill(tmpCentre,qRgb(pos/(255*255),pos/255,pos%255), &cpImage);
            tmpCentre += 2*shiftX;
        }
        centre += 3*shiftY+ sign(i)*shiftX;
    }
}



void View::mouseMoveEvent(QMouseEvent *ev)
{
    if(ev->buttons() & Qt::LeftButton){
        QPoint pos = getPositionHexagon(ev->pos().x() + shiftScroll.x(),ev->pos().y()+shiftScroll.y());
        if(pos.x() == -1)
            return;
        if (pos != lastModified){
            lastModified = pos;

            bool newState = true;
            if (viewSettings.modeXOR){
                newState = !model->isAlive(pos.x(), pos.y());
            }

            model->setCell(pos.x(), pos.y(), newState);
            this->update();
        }
    }
}

ViewSettings View::getSettings(){
    return viewSettings;

}

void View::setSettings(ViewSettings settings){
    viewSettings = settings;
    setShiftX(viewSettings.lengthEdge);
    setShiftY(viewSettings.lengthEdge);
    changeSizeScrolls();
    if(image.height() < 2*model->getSettings().n*viewSettings.lengthEdge ||
            image.width() < 2*model->getSettings().m*viewSettings.lengthEdge ){
        changeSizeImage();
    }
    createCopyImage();
    this->update();
}

void View::setShiftScrollX(int x){
    shiftScroll.setX(10*(x - 1));
    update();
}

void View::setShiftScrollY(int y){
    shiftScroll.setY(10*(y - 1));
    update();
}

void View::changeSizeImage(){
    while(image.height() >= 2*model->getSettings().n*viewSettings.lengthEdge &&
          image.width() >= 2*model->getSettings().m*viewSettings.lengthEdge ){
        sizeImageX *= 2;
        sizeImageY *= 2;
        image = QImage(sizeImageX,sizeImageY,format);
    }

}

void View::setLineParameters(int lengthEdge, int widthEdge ){
    viewSettings.widthEdge = widthEdge;
    viewSettings.lengthEdge = lengthEdge;

    setShiftX(viewSettings.lengthEdge);
    setShiftY(viewSettings.lengthEdge);
    changeSizeScrolls();
    if(image.height() < 2*model->getSettings().n*viewSettings.lengthEdge ||
            image.width() < 2*model->getSettings().m*viewSettings.lengthEdge ){
        changeSizeImage();
    }
    setShiftScrollY(1);
    setShiftScrollX(1);
    createCopyImage();
}

void View::changeSizeScrolls(){

    int scrollRangeX = SQRT_3*viewSettings.lengthEdge*model->getSettings().m - widthScreen;
    int scrollRangeY = 3*viewSettings.lengthEdge*model->getSettings().n/2+10 - heightScreen;
    if(scrollRangeY < 0)
        emit setVisiableVerticalScroll(false);
    else {
        emit setVisiableVerticalScroll(true);
        emit changeSizeImageY(1,scrollRangeY/10+2);
    }
    if(scrollRangeX < 0){
        emit setVisiableHorizontalScroll(false);
    }
    else{
        emit setVisiableHorizontalScroll(true);
        emit changeSizeImageX(1,scrollRangeX/10+2);
    }
    if(shiftScroll.x() >= scrollRangeX/10+2 && scrollRangeX >= 0){
        this->setShiftScrollX(scrollRangeX/10+2);
    }
    if(shiftScroll.y() >= scrollRangeY/10+2 && scrollRangeY >= 0){
        this->setShiftScrollY(scrollRangeY/10+2);
    }
    this->update();
}

void View::changeSizeScreen(int width, int height){
    widthScreen = width;
    heightScreen = 5*height/6;
    this->changeSizeScrolls();
}




