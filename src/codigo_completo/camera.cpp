#include "camera.h"

Camera::Camera ()
{
    projectionMatrix.setToIdentity ();
    computeViewMatrix();
}
void Camera :: computeViewMatrix()
{
    eye = GetEyeVector();

    viewMatrix.setToIdentity ();
    viewMatrix.lookAt(eye , center , up);
}
void Camera::resizeViewport (int width , int height )
{
    qDebug("w: %d, h: %d", width, height);
    projectionMatrix.setToIdentity ();
    float aspectRatio = static_cast <float>(width) / static_cast <float>(height);
    projectionMatrix.perspective(60.0 , aspectRatio , nearField, farField);
}

void Camera::MoveCameraLeft(){
    theta -= qDegreesToRadians(10.0f);
    computeViewMatrix();
}

void Camera::MoveCameraRight(){
    theta += qDegreesToRadians(10.0f);
    computeViewMatrix();
}

void Camera::MoveCameraUp(){
    phi += qDegreesToRadians(10.0f);
    computeViewMatrix();
}

void Camera::MoveCameraDown(){
    phi -= qDegreesToRadians(10.0f);
    computeViewMatrix();
}

void Camera::IncreaseDistance(){
    radius+=0.1f;
    computeViewMatrix();
}

void Camera::DecreaseDistance(){
    radius-=0.1f;
    computeViewMatrix();
}

QVector3D Camera::GetEyeVector(){
    //float X = radius*sin(phi)*cos(theta);
    //float Y = radius*sin(phi)*sin(theta);
    //float Z = -radius*cos(phi);

    float X = radius*sin(phi)*sin(theta);
    float Y = radius*cos(phi);
    float Z = radius*sin(phi)*cos(theta);
    qDebug("(%f. %f, %f)", X, Y, Z);
    return QVector3D (X, Y, Z);
}
