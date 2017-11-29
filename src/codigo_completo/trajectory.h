#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

#include <memory>

class Trajectory : public QOpenGLExtraFunctions
{
public :
    Trajectory(QOpenGLWidget* _glWidget);
    ~Trajectory();

    QOpenGLWidget* glWidget;

    GLuint shaderProgram;

    void createVBOs();
    void createShaders();
    void destroyVBOs();
    void destroyShaders();

    void startTrajectory();
    void drawModel();

    GLuint vao = 0;
    GLuint vbo[3];

};


#endif // TRAJECTORY_H
