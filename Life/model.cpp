#include "model.h"

Model::Model(const Settings &settings):
    currentField(0),
    previousField(1),
    modelSettings(settings)
{
    saved = true;
    fields[currentField].resize(settings.n*settings.m);
    fields[previousField].resize(settings.n*settings.m);
}
Model::~Model(){}

void Model::setSettings(const Settings &newSettings){
    saved = false;
    int oldM = modelSettings.m;
    int oldN = modelSettings.n;
    modelSettings = newSettings;
    if(oldM != newSettings.m || oldN != newSettings.n){
        changeSizeField(oldN, oldM);
    }

    recalculateImpacts();

}
bool Model::isAlive(int x, int y)
{
    return fields[currentField][y*modelSettings.m + x].alive;
}

void Model::setCell(int x, int y, bool state){
    Cell &cell = fields[currentField][y*modelSettings.m + x];
        if (cell.alive != state){
            cell.alive = state;
            addImpact(x,y);
        }
}
void Model::nextStep(){
    saved = false;
    currentField = 1 - currentField;
    previousField = 1 - previousField;
    int n = modelSettings.n;
    int m = modelSettings.m;
    float impact;
    memset(fields[currentField].data(), 0, fields[0].size() * sizeof(Cell));
    for(int i = 0; i < n; i++){
        int width = i%2 ? (m - 1) : m;
        for(int j = 0; j < width; j++){
            impact = fields[previousField][i*modelSettings.m+j].impact;
            if((fields[previousField][i*modelSettings.m+j].alive && impact >= modelSettings.liveBegin && impact <= modelSettings.liveEnd)
                    || (impact >= modelSettings.birthBegin && impact <= modelSettings.birthEnd))
                setCell(j,i,true);
            else {
                setCell(j,i,false);
            }
        }
    }
}
void Model::clear(){
    saved = false;
    memset(fields[0].data(), 0, fields[0].size() * sizeof(Cell));
    memset(fields[1].data(), 0, fields[1].size() * sizeof(Cell));

}
float Model::calculateImpact(int x, int y){
    float impact = 0;
    float firstImpact = modelSettings.fstImpact;
    float secondImpact = modelSettings.sndImpact;
    int n = modelSettings.n;
    int m = modelSettings.m;
    bool isOddLine = y%2 == 0;
    for(int i = 0; i < 6; i++){
        QPoint pos = QPoint(x,y) + fstShift[i];
        int shift = shiftOddLine(isOddLine,pos.y());
        if (pos.x() >= 0 && pos.y() >= 0 &&
                    pos.x() < n + shift && pos.y() < m)
            impact += firstImpact * fields[currentField][pos.y()*m+pos.x() - shift].alive;
    }
    for(int i = 0; i < 6; i++){
        QPoint pos = QPoint(x,y) + sndShift[i];
        int shift = shiftOddLine(isOddLine,pos.y());
        if (pos.x() >= 0 && pos.y() >= 0 &&
                    pos.x() < n && pos.y() < m)
            impact += secondImpact * fields[currentField][pos.y()*m+pos.x() - shift].alive;
    }
    return impact;
}
void Model::recalculateImpacts(){
    saved = false;
    int m = modelSettings.m;
    int n = modelSettings.n;
    for(int i = 0; i < n; i++){
        int curM = i%2 ? (m-1) : m;
        for(int j = 0; j < curM; j++){
            fields[currentField][i*m + j].impact = calculateImpact(j,i);
        }
    }
}

void Model::addImpact(int x, int y){
    saved = false;
    int m = modelSettings.m;
    int n = modelSettings.n;
    bool isOddLine = y%2 == 0;
    for(int i = 0; i < 6; i++){
        QPoint pos = QPoint(x,y) + fstShift[i] ;
        int shift = shiftOddLine(isOddLine,pos.y());
        if (pos.x() >= 0 + shift && pos.y() >= 0 &&
                    pos.x() < m && pos.y() < n)
            if(isAlive(x,y)){
                fields[currentField][pos.y()*m+pos.x()-shift].impact += modelSettings.fstImpact;
            }
            else {
                fields[currentField][pos.y()*m+pos.x()-shift].impact -= modelSettings.fstImpact;
            }
    }

    for(int i = 0; i < 6; i++){
        QPoint pos = QPoint(x,y) + sndShift[i];
        int shift = shiftOddLine(isOddLine,pos.y());
        if (pos.x() >= 0 + shift && pos.y() >= 0 &&
                    pos.x() < modelSettings.n && pos.y() < modelSettings.m)
            if(isAlive(x,y)){
                fields[currentField][pos.y()*m+pos.x()-shift].impact += modelSettings.sndImpact;
            }
            else {
                fields[currentField][pos.y()*m+pos.x()-shift].impact -= modelSettings.sndImpact;
            }
    }

}
Settings Model::getSettings(){
    return modelSettings;
}

float Model::getImpact(int x,int y){
    return fields[currentField][y*modelSettings.m+x].impact;
}

int Model::shiftOddLine(bool isOddLine,int y){
    return isOddLine*(y%2);
}

void Model::changeSizeField(int oldN, int oldM){
    saved = false;
    currentField = 1 - currentField;
    previousField = 1 - previousField;
    fields[currentField].resize(modelSettings.n*modelSettings.m);
    int m = oldM;
    int n = oldN;
    memset(fields[currentField].data(), 0, fields[currentField].size() * sizeof(Cell));
    for(int i = 0; i < n; i++){
        int curM = i%2 ? (m-1) : m;
        for(int j = 0; j < curM; j++){
            if(fields[previousField][i*m + j].alive && i < modelSettings.m && j < modelSettings.n){
                setCell(j,i,true);
            }
        }
    }
    fields[previousField].resize(modelSettings.n*modelSettings.m);
}

void Model::setSize(int n, int m){
    saved = false;
    modelSettings.m = m;
    modelSettings.n = n;
    fields[currentField] = std::vector<Cell>(modelSettings.m * modelSettings.n);
    fields[previousField] = std::vector<Cell>(modelSettings.m * modelSettings.n);
}

void Model::saveFile(QTextStream &file, int widthEdge, int lengthEdge){
    saved = true;
    int n = modelSettings.n;
    int m = modelSettings.m;
    file << m << " ";
    file << n << '\n';
    file << widthEdge << '\n';
    file << lengthEdge << '\n';
    int count = 0;
    for(int i = 0; i < n; i++){
        int curM = i%2 ? (m-1) : m;
        for(int j = 0; j < curM; j++){
            if(isAlive(j,i)){
                count++;
            }
        }
    }
    file << count << '\n';

    for(int i = 0; i < n; i++){
        int curM = i%2 ? (m-1) : m;
        for(int j = 0; j < curM; j++){
            if(isAlive(j,i)){
                file << j << " " << i << '\n';
            }
        }
    }
    file << '\r';

}
bool Model::isFileSaved(){
    return saved;
}








