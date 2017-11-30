#include "astronomicalobject.h"

AstronomicalObject::AstronomicalObject(QOpenGLWidget* _glWidget):Model(_glWidget){
}

void AstronomicalObject::Rotate(float elapsedTime){
    if(orbitSpeed > 0){
        orbitAngleTheta+= orbitSpeed * elapsedTime;
        orbitAnglePhi = qDegreesToRadians(90.0);

        float x = orbitDistance*sin(orbitAnglePhi)*sin(orbitAngleTheta);
        float y = orbitDistance*cos(orbitAnglePhi);
        float z = orbitDistance*sin(orbitAnglePhi)*cos(orbitAngleTheta);

        auto delta  = QVector3D(x,y,z);
        position  = orbitObject->position + delta;
    }
}

void AstronomicalObject::Translate(float elapsedTime){
    if(translationSpeed > 0){
        angle+= translationSpeed * elapsedTime * 10;
    }
}
