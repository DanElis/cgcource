#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <cstring>
#include <QPoint>
#include <QObject>
#include <QWidget>
#include <QTextStream>
struct Settings{
    int n;
    int m;
    float liveBegin;
    float liveEnd;
    float birthBegin;
    float birthEnd;
    float fstImpact;
    float sndImpact;
};

struct Cell {
    float impact;
    bool alive;
};
class Model: public QObject
{
    Q_OBJECT
public:
    explicit Model(const Settings &modelSettings);
    ~Model();
    void setSettings(const Settings &newSettings);

    bool isAlive(int x, int y);
    void setCell(int x, int y, bool state);
    float getImpact(int x, int y);
    Settings getSettings();
    void clear();
    void addImpact(int x, int y);
    void saveFile(QTextStream &file, int widthEdge, int lengthEdge);
    void setSize(int n, int m);
    bool isFileSaved();
public slots:
    void nextStep();
private:
    void changeSizeField(int newN, int newM);
    int shiftOddLine(bool isOddLine,int y);
    void recalculateImpacts();
    float calculateImpact(int x, int y);
    bool saved;
    int currentField;
    int previousField;

    const QPoint fstShift[6] = { QPoint(1, 0), QPoint(1, -1),
                        QPoint(0, -1), QPoint(-1,  0),
                        QPoint(1,  1), QPoint(0,  1)};
    const QPoint sndShift[6] = { QPoint(2, 1), QPoint(2, -1),
                        QPoint(0, -2), QPoint(-1,  -1),
                        QPoint(-1,  1), QPoint(0,  2)};
    Settings modelSettings;

    std::vector<Cell> fields[2];
};

#endif // MODEL_H
