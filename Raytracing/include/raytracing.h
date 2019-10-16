#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <QObject>
#include <QThreadPool>
#include <QAtomicInt>

#include "settings.h"
#include "matrices.h"


class Raytracing : public QObject
{
    Q_OBJECT
public:
    explicit Raytracing(Settings &settings, int width, int height, QObject *parent = nullptr);

    float* startRaytracing();


signals:
    void renderingProgress(int);


private:
    Settings *settings;
    int width, height, wh, hh;


    QThreadPool thread_pool;
    QAtomicInt tasks_done;


    class RaytracingTask : public QRunnable {
    public:
        RaytracingTask(std::function<void ()> &&F);
        void run() override;
    private:
        std::function<void ()> F;
    };

    inline QRunnable* constructTask(const Vector3 &screen, const Vector3 &i, const Vector3 &j,
                                    int x, int y, float *it, float *it2 = nullptr);



    void trace(int depth, const Vector3 &eye, const Vector3 &dir, float &R, float &G, float &B);
    inline bool checkAllFigures(const Vector3 &eye, const Vector3 &dir,
                                float &x, float &y, float &z,
                                QSharedPointer<Figure> **figure);


    inline void translateToRealCoords(float &x, float &y) const;
};

#endif // RAYTRACING_H
