#ifndef LIGHT_H
#define LIGHT_H

#include <QObject>

#include <matrices.h>


class Light : public QObject
{
    Q_OBJECT
public:
    Light(QObject *parent = nullptr);
    explicit Light(Vector3 position, float R, float G, float B, QObject *parent = nullptr);
    Light(const Light &other);
    Light(Light &&other);
    Light& operator=(const Light &other);
    Light& operator=(Light &&other);


    Vector3 position;
    float R;
    float G;
    float B;
};

#endif // LIGHT_H
