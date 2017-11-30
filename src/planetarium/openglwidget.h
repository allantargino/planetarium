#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

#include "light.h"
#include "model.h"
#include "camera.h"
#include "astronomicalobject.h"
#include "modelfactory.h"
#include "shadermanager.h"
#include "trajectory.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *event);

    //Animation
    QTimer timer;
    QTime time;

public slots:
    void animate();
    void start();
    void exit();
    void toggleTrajectory();
    void toggleRotationTranslation();
    void increaseAnimationSpeed();
    void decreaseAnimationSpeed();

protected:
    std::unique_ptr<ModelFactory> factory;
    std::unique_ptr<ShaderManager> shaders;

    std::vector<std::shared_ptr<AstronomicalObject>> objects;
    std::unique_ptr<Trajectory> trajectory;

    float animationSpeed = 1.0;

    Light light;
    Camera camera;

    bool rotationTranslationEnable = true;
    bool trajectoryEnabled = false;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

};

#endif // OPENGLWIDGET_H
