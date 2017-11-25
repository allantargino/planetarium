#ifndef ASTRONOMICALOBJECT_H
#define ASTRONOMICALOBJECT_H

#include "model.h"

class AstronomicalObject: public Model
{
public :
    AstronomicalObject(QOpenGLWidget* _glWidget);

    //Orbit
    float orbitSpeed = 0;
    float orbitAnglePhi = 0;
    float orbitAngleTheta = 0;
    AstronomicalObject* orbitObject = nullptr;

    void Orbit(float elapsedTime);
};

#endif // ASTRONOMICALOBJECT_H
