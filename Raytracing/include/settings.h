#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVector>
#include <QSharedPointer>

#include "matrices.h"

#include "light.h"
#include "figure.h"


class Settings : public QObject
{
    Q_OBJECT
public:
    enum Quality {
        Rough, Normal, Fine
    };

    explicit Settings(QObject *parent = nullptr);
    explicit Settings(float Ar, float Ag, float Ab,
                      QVector<Light> lights,
                      QList< QSharedPointer<Figure> > figures,
                      QObject *parent = nullptr);
    Settings(const Settings &other);
    Settings(Settings &&other);
    Settings& operator=(const Settings &other);
    Settings& operator=(Settings &&other);

    float Ar, Ag, Ab;
    QVector<Light> lights;
    QList< QSharedPointer<Figure> > figures;

    bool deny = false;
    bool should_init;

    float Br = 0, Bg = 0, Bb = 0;
    float gamma = 1;
    int depth = 1;
    Quality quality = Normal;
    Vector3 eye, view, up;
    float zn, zf;
    float sw = 1, sh = 1;
};

#endif // SETTINGS_H
