#include "astronomicalobject.h"

AstronomicalObject::AstronomicalObject(QOpenGLWidget* _glWidget):Model(_glWidget){
}

void AstronomicalObject::Orbit(float timeElapsed){
    if(orbitSpeed > 0){
        orbitAngleTheta+= orbitSpeed * timeElapsed;
        orbitAnglePhi = qDegreesToRadians(90.0);

        float x = orbitDistance*sin(orbitAnglePhi)*sin(orbitAngleTheta);
        float y = orbitDistance*cos(orbitAnglePhi);
        float z = orbitDistance*sin(orbitAnglePhi)*cos(orbitAngleTheta);

        auto delta  = QVector3D(x,y,z);
        position  = orbitObject->position + delta;

        //position = GetNextOrbit(position, orbitSpeed, timeElapsed);
    }
}
