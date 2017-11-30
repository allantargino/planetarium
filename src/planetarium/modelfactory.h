#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "astronomicalobject.h"
#include "shadermanager.h"
#include "utils.h"

class ModelFactory
{
public:
    ModelFactory(QOpenGLWidget *_glWidget, ShaderManager* _shaders);
    ~ModelFactory();

    std::shared_ptr<AstronomicalObject> GetSun();
    std::shared_ptr<AstronomicalObject> GetMercury(std::shared_ptr<AstronomicalObject> sun);
    std::shared_ptr<AstronomicalObject> GetVenus(std::shared_ptr<AstronomicalObject> sun);
    std::shared_ptr<AstronomicalObject> GetEarth(std::shared_ptr<AstronomicalObject> sun);
    std::shared_ptr<AstronomicalObject> GetMoon(std::shared_ptr<AstronomicalObject> earth);
    std::shared_ptr<AstronomicalObject> GetMars(std::shared_ptr<AstronomicalObject> sun);
    std::shared_ptr<AstronomicalObject> GetJupiter(std::shared_ptr<AstronomicalObject> sun);
    std::shared_ptr<AstronomicalObject> GetSaturn(std::shared_ptr<AstronomicalObject> sun);
    std::shared_ptr<AstronomicalObject> GetSaturnRing(std::shared_ptr<AstronomicalObject> saturn);
    std::shared_ptr<AstronomicalObject> GetUranus(std::shared_ptr<AstronomicalObject> sun);
    std::shared_ptr<AstronomicalObject> GetNeptune(std::shared_ptr<AstronomicalObject> sun);

protected:
     QOpenGLWidget* glWidget;
     ShaderManager* shaders;

     std::shared_ptr<AstronomicalObject> GetModel(const QString offModelFileName, const QString textureFileName, int shaderIndex, float radius, float distanceOrbitObject, float orbitSpeed, AstronomicalObject* orbitObject, float translationSpeed);
     QImage GetTexture(const QString textureFileName);
};

#endif // MODELFACTORY_H
