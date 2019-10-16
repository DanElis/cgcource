#include "raytracing.h"

#include <QDebug>
#include <cmath>


Raytracing::Raytracing(Settings &settings, int width, int height, QObject *parent)
    : QObject(parent),
      settings(&settings),
      width(width), height(height)
{
}



// Raytracing
float* Raytracing::startRaytracing()
{
    int amount = (settings->quality == Settings::Rough ? (width * height / 4) : (width * height));
    tasks_done = 0;

    auto *progress_thread = QThread::create([this, amount]() {
        while (tasks_done != amount) {
            emit renderingProgress((int) (100.0f * tasks_done / amount));
            QThread::currentThread()->msleep(250);
        }
    });
    connect(progress_thread, SIGNAL(finished()), progress_thread, SLOT(deleteLater()));
    progress_thread->start();



    float *data = new float[width * height * 3];

    wh = width / 2;
    hh = height / 2;

    Vector3 k = (settings->view - settings->eye).normalized();
    Vector3 i = Vector3::crossProduct(k, settings->up).normalized();
    Vector3 j = Vector3::crossProduct(i, k);

    Vector3 screen = settings->zn * k;


    switch (settings->quality) {
    case Settings::Rough:
        {
            auto *it = data, *it2 = data + width*3;
            for (int y = 0; y < height; y += 2, it += width*3, it2 += width*3)
                for (int x = 0; x < width; x += 2, it += 6, it2 += 6)
                    thread_pool.start(constructTask(screen, i, j, x, y, it, it2));
        }
        break;

    case Settings::Normal:
    case Settings::Fine:
        {
            auto *it = data;
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x, it += 3)
                    thread_pool.start(constructTask(screen, i, j, x, y, it));
        }
        break;
    }


    thread_pool.waitForDone();
    progress_thread->wait();


    float max = 0;
    auto *it = data;
    for (int i = 0; i < width * height * 3; ++i, ++it)
        max = std::max(max, *it);

    if (std::abs(max) < 10e-9f) max = 1;

    it = data;
    for (int i = 0; i < width * height * 3; ++i, ++it)
        it[0] = std::pow(it[0] / max, settings->gamma);


    return data;
}



#define TRACE_SCREEN_POINT(xx,yy,s) \
    float R##s = 0, G##s = 0, B##s = 0; \
    float x##s = xx, y##s = yy; \
    translateToRealCoords( x##s, y##s ); \
    Vector3 P##s = screen + x##s * i + y##s * j; \
    P##s .normalize(); \
    trace(settings->depth, settings->eye, P##s, R##s, G##s, B##s);


QRunnable *Raytracing::constructTask(const Vector3 &screen, const Vector3 &i, const Vector3 &j,
                                     int x, int y, float *it, float *it2)
{
    QRunnable *task;

    switch (settings->quality) {
    case Settings::Rough:
        task = new RaytracingTask([this, &screen, &i, &j, x, y, it, it2]() {
            TRACE_SCREEN_POINT(x + 1.0f, y + 1.0f, 0);
            it[0] = B0;
            it[1] = G0;
            it[2] = R0;
            it[3] = B0;
            it[4] = G0;
            it[5] = R0;
            it2[0] = B0;
            it2[1] = G0;
            it2[2] = R0;
            it2[3] = B0;
            it2[4] = G0;
            it2[5] = R0;
            ++tasks_done;
        });
        break;

    case Settings::Normal:
        task = new RaytracingTask([this, &screen, &i, &j, x, y, it]() {
            TRACE_SCREEN_POINT(x + 0.5f, y + 0.5f, 0);
            it[0] = B0;
            it[1] = G0;
            it[2] = R0;
            ++tasks_done;
        });
        break;

    case Settings::Fine:
        task = new RaytracingTask([this, &screen, &i, &j, x, y, it]() {
            TRACE_SCREEN_POINT(x + 0.25f, y + 0.25f, 0);
            TRACE_SCREEN_POINT(x + 0.75f, y + 0.25f, 1);
            TRACE_SCREEN_POINT(x + 0.25f, y + 0.75f, 2);
            TRACE_SCREEN_POINT(x + 0.75f, y + 0.75f, 3);
            it[0] = (B0 + B1 + B2 + B3) / 4;
            it[1] = (G0 + G1 + G2 + G3) / 4;
            it[2] = (R0 + R1 + R2 + R3) / 4;
            ++tasks_done;
        });
        break;
    }

    return task;
}



void Raytracing::trace(int depth, const Vector3 &eye, const Vector3 &dir, float &R, float &G, float &B)
{
    if (depth <= 0) {
        R = 0;
        G = 0;
        B = 0;
        return;
    }


    float x, y, z;
    QSharedPointer<Figure> *figure_ptr;

    if (!checkAllFigures(eye, dir, x, y, z, &figure_ptr)) {
        R = settings->Br;
        G = settings->Bg;
        B = settings->Bb;
        return;
    }

    auto &figure = *figure_ptr;
    Vector3 P(x, y, z);


    float Rr = 0, Gr = 0, Br = 0;
    trace(depth - 1, P, figure->reflectRay(dir, P), Rr, Gr, Br);


    auto normal = figure->getNormal(P);

    for (const auto &light : settings->lights) {
        Vector3 light_dir = P - light.position;
        float fatt = 1 / (1 + light_dir.length());
        light_dir.normalize();

        float x, y, z;
        QSharedPointer<Figure> *light_figure_ptr;
        checkAllFigures(light.position, light_dir, x, y, z, &light_figure_ptr);

        if (light_figure_ptr != figure_ptr) continue;


        // k1
        float k1 = Vector3::dotProduct(normal, -light_dir);

        // k2
        Vector3 H = light_dir + dir;
        H.normalize();
        float k2 = std::pow(Vector3::dotProduct(normal, -H), figure->power);


        R += fatt * light.R * (figure->KDr * k1 + figure->KSr * k2);
        G += fatt * light.G * (figure->KDg * k1 + figure->KSg * k2);
        B += fatt * light.B * (figure->KDb * k1 + figure->KSb * k2);
    }


    R += figure->KDr * settings->Ar + figure->KSr * Rr;
    G += figure->KDg * settings->Ag + figure->KSg * Gr;
    B += figure->KDb * settings->Ab + figure->KSb * Br;
}

bool Raytracing::checkAllFigures(const Vector3 &eye, const Vector3 &dir,
                                  float &x, float &y, float &z,
                                  QSharedPointer<Figure> **figure)
{
    bool has_intersect = false;
    float min_len;

    for (auto &F : settings->figures) {
        float xF, yF, zF;

        if (F->checkIntersect(eye, dir, xF, yF, zF)) {
            if (has_intersect) {
                float len = (Vector3(xF, yF, zF) - eye).length();

                if (len < min_len) {
                    min_len = len;
                    x = xF;
                    y = yF;
                    z = zF;
                    (*figure) = &F;
                }
            } else {
                has_intersect = true;
                min_len = (Vector3(xF, yF, zF) - eye).length();
                x = xF;
                y = yF;
                z = zF;
                (*figure) = &F;
            }
        }
    }

    return has_intersect;
}



void Raytracing::translateToRealCoords(float &x, float &y) const
{
    x = (x - wh) / wh * (settings->sw / 2);
    y = -(y - hh) / hh * (settings->sh / 2);
}



Raytracing::RaytracingTask::RaytracingTask(std::function<void ()> &&F) : F(std::move(F)) {}

void Raytracing::RaytracingTask::run()
{
    F();
}
