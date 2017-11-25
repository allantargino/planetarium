#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "astronomicalobject.h"

class ModelFactory
{
public:
    ModelFactory(QOpenGLWidget *_glWidget);
    ~ModelFactory();

    std::shared_ptr<AstronomicalObject> GetSun();
    std::shared_ptr<AstronomicalObject> GetEarth(std::shared_ptr<AstronomicalObject> sun);
    std::shared_ptr<AstronomicalObject> GetMoon(std::shared_ptr<AstronomicalObject> earth);

protected:
     QOpenGLWidget* glWidget;

     std::shared_ptr<AstronomicalObject> GetModel(const QString offModelFileName, const QString textureFileName, int shaderIndex, const QVector3D position, float scale, float angle, float orbitSpeed);
     QImage GetTexture(const QString textureFileName);
};

#endif // MODELFACTORY_H
