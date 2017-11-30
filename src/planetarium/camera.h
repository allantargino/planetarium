#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>

class Camera
{
public :
    Camera();
    QVector3D eye;
    QVector3D center = QVector3D(0.0, 0.0, 0.0);
    QVector3D up = QVector3D(0.0, 1.0, 0.0);

    QMatrix4x4 projectionMatrix;
    QMatrix4x4 viewMatrix;

    void computeViewMatrix();
    void resizeViewport(int width, int height);

    float nearField = 0.1f;
    float farField = 20.0f;

    float radius = 2.5f;
    float theta = 0.0f;
    float phi = qDegreesToRadians(75.0f);

    void MoveCameraLeft();
    void MoveCameraRight();
    void MoveCameraUp();
    void MoveCameraDown();
    void IncreaseDistance();
    void DecreaseDistance();

    QVector3D GetEyeVector();
};

#endif // CAMERA_H
