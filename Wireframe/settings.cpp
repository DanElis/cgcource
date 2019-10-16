#include "settings.h"


Settings::Settings(int n, int m, int k, float a, float b, float c, float d, float zn, float zf, float sw, float sh, uint bc, QObject *parent) : QObject(parent)
{
    this->n = n;
    this->m = m;
    this->k = k;

    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;

    this->zn = zn;
    this->zf = zf;
    this->sw = sw;
    this->sh = sh;

    this->background_color = bc;
}

Settings &Settings::operator=(const Settings &other)
{
    n = other.n;
    m = other.m;
    k = other.k;

    a = other.a;
    b = other.b;
    c = other.c;
    d = other.d;

    zn = other.zn;
    zf = other.zf;
    sw = other.sw;
    sh = other.sh;

    background_color = other.background_color;

    return *this;
}
