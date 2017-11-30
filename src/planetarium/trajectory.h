#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

#include <memory>

class Trajectory : public QOpenGLExtraFunctions
{
public :
    Trajectory(QOpenGLWidget* _glWidget, int _trajectories);
    ~Trajectory();

    QOpenGLWidget* glWidget;

    void createVBOs();
    void destroyVBOs();

    void startTrajectory();
    void drawModel();

    void addTrajectory(QVector3D startPoint, QVector3D color, float rad);

    GLuint vao = 0;
    GLuint vbo[3];

    int count = 0;
    int trajectories;
    std::unique_ptr<QVector3D[]> startPoints;
    std::unique_ptr<QVector3D[]> colors;
    std::unique_ptr<float[]> radius;

};


#endif // TRAJECTORY_H
