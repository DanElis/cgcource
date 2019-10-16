#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(int n = 10, int m = 10, int k = 5,
                      float a = 0.0f, float b = 1.0f,
                      float c = 0.0f, float d = 6.28f,
                      float zn = 5, float zf = 20,
                      float sw = 3, float sh = 3,
                      uint bc = 0xFFFFFFFF,
                      QObject *parent = nullptr);

    Settings& operator=(const Settings &other);

    int n, m, k;
    float a, b, c, d;
    float zn, zf, sw, sh;
    uint background_color;
};

#endif // SETTINGS_H
