#include "astronomicalobject.h"

AstronomicalObject::AstronomicalObject(QOpenGLWidget* _glWidget):Model(_glWidget){
}

void AstronomicalObject::Orbit(float timeElapsed){
    if(orbitSpeed > 0){
        orbitAngleTheta+= orbitSpeed * timeElapsed;
        orbitAnglePhi = qDegreesToRadians(90.0);

        float radius = position.distanceToPoint(orbitObject->position);

        float x = radius*sin(orbitAnglePhi)*sin(orbitAngleTheta);
        float y = radius*cos(orbitAnglePhi);
        float z = radius*sin(orbitAnglePhi)*cos(orbitAngleTheta);

        position  = QVector3D(x,y,z);

        //position = GetNextOrbit(position, orbitSpeed, timeElapsed);
    }
}

//QVector3D AstronomicalObject::GetNextOrbit(QVector3D current, float speed, float timeElapsed, float theta, float phi)
//{
////    qDegreesToRadians((currentAngle + angleCorrection)

////    float delta_x = radius*sin(phi)*sin(theta);
////    float delta_y = radius*cos(phi);
////    float delta_z = radius*sin(phi)*cos(theta);

////    float x = current.x() + movimentFactor * cos());
////    float y = current.y() + movimentFactor * sin(qDegreesToRadians((currentAngle + angleCorrection)));
////    float z = current.z();

////    return QVector3D(x, y, z);
//    return QVector3D(0,0,0);
//}
