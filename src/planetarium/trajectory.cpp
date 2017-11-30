#include "trajectory.h"

Trajectory::Trajectory(QOpenGLWidget* _glWidget, int _trajectories){
    glWidget = _glWidget;
    trajectories = _trajectories;

    qDebug("Starting Trajectory");
    startPoints = std::make_unique<QVector3D[]>(trajectories);
    colors = std::make_unique<QVector3D[]>(trajectories);
    radius = std::make_unique<float[]>(trajectories);
}

Trajectory::~Trajectory(){
    destroyVBOs();
}

void Trajectory::startTrajectory(){
    initializeOpenGLFunctions();

    createVBOs();
}

void Trajectory::drawModel(){
    glBindVertexArray(vao);

    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glDrawArrays(GL_POINTS, 0, count);
}

void Trajectory::createVBOs(){
    glWidget->makeCurrent ();
    destroyVBOs ();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, vbo);

    // VBO for position data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(QVector3D), startPoints.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // VBO for color data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(QVector3D), colors.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // VBO for radius data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(float), radius.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);
}

void Trajectory::destroyVBOs()
{
    glWidget->makeCurrent ();

    glDeleteVertexArrays (1, &vao);

    int sizeVBOs = sizeof(vbo) / sizeof(float);
    for (int i = 0; i < sizeVBOs; ++i)
        vbo[i] = 0;

    vao = 0;
}

void Trajectory::addTrajectory(QVector3D startPoint, QVector3D color, float rad){
    startPoints[count] = startPoint;
    colors[count] = color;
    radius[count] = rad;
    count++;
}
