#include "settings.h"


Settings::Settings(QObject *parent)
    : Settings(0, 0, 0, QVector<Light>(), QList< QSharedPointer<Figure> >(), parent)
{
    should_init = false;
    deny = true;
}

Settings::Settings(float Ar, float Ag, float Ab,
                   QVector<Light> lights,
                   QList< QSharedPointer<Figure> > figures,
                   QObject *parent)
    : QObject(parent),
      Ar(Ar), Ag(Ag), Ab(Ab),
      lights(std::move(lights)),
      figures(std::move(figures)),
      should_init(true)
{
}

Settings::Settings(const Settings &other)
{
    Ar = other.Ar;
    Ag = other.Ag;
    Ab = other.Ab;
    lights = other.lights;
    figures = other.figures;
    should_init = other.should_init;
    deny = other.deny;
    Br = other.Br;
    Bg = other.Bg;
    Bb = other.Bb;
    gamma = other.gamma;
    depth = other.depth;
    quality = other.quality;
    eye = other.eye;
    view = other.view;
    up = other.up;
    zn = other.zn;
    zf = other.zf;
    sw = other.sw;
    sh = other.sh;
}

Settings::Settings(Settings &&other)
{
    Ar = other.Ar;
    Ag = other.Ag;
    Ab = other.Ab;
    lights = std::move(other.lights);
    figures = std::move(other.figures);
    should_init = other.should_init;
    deny = other.deny;
    Br = other.Br;
    Bg = other.Bg;
    Bb = other.Bb;
    gamma = other.gamma;
    depth = other.depth;
    quality = other.quality;
    eye = std::move(other.eye);
    view = std::move(other.view);
    up = std::move(other.up);
    zn = other.zn;
    zf = other.zf;
    sw = other.sw;
    sh = other.sh;
}

Settings &Settings::operator=(const Settings &other)
{
    Ar = other.Ar;
    Ag = other.Ag;
    Ab = other.Ab;
    lights = other.lights;
    figures = other.figures;
    should_init = other.should_init;
    deny = other.deny;
    Br = other.Br;
    Bg = other.Bg;
    Bb = other.Bb;
    gamma = other.gamma;
    depth = other.depth;
    quality = other.quality;
    eye = other.eye;
    view = other.view;
    up = other.up;
    zn = other.zn;
    zf = other.zf;
    sw = other.sw;
    sh = other.sh;

    return *this;
}

Settings &Settings::operator=(Settings &&other)
{
    Ar = other.Ar;
    Ag = other.Ag;
    Ab = other.Ab;
    lights = std::move(other.lights);
    figures = std::move(other.figures);
    should_init = other.should_init;
    deny = other.deny;
    Br = other.Br;
    Bg = other.Bg;
    Bb = other.Bb;
    gamma = other.gamma;
    depth = other.depth;
    quality = other.quality;
    eye = std::move(other.eye);
    view = std::move(other.view);
    up = std::move(other.up);
    zn = other.zn;
    zf = other.zf;
    sw = other.sw;
    sh = other.sh;

    return *this;
}
