#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "astronomicalobject.h"

class ModelFactory
{
public :
    ModelFactory(QOpenGLWidget *_glWidget);
    ~ModelFactory();

    std::shared_ptr<AstronomicalObject> GetSun();

protected:
     QOpenGLWidget* glWidget;

     QImage GetTexture(const QString textureFileName);
};

#endif // MODELFACTORY_H
