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

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    OpenGLWidget(QWidget *parent = 0);

    //Mouse Events
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    //Animation
    QTimer timer;
    QTime time;

public slots:
    void animate();
    void start();

protected:
    std::unique_ptr<ModelFactory> factory;

    std::vector<std::shared_ptr<AstronomicalObject>> objects;

    Light light;
    Camera camera;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
};

#endif // OPENGLWIDGET_H
