#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{
    shaders = std::make_unique<ShaderManager>(this);
    factory = std::make_unique<ModelFactory>(this, shaders.get());

    trajectory = std::make_unique<Trajectory>(this);
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug("OpenGL version: %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);

    time.start();
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (objects.size()==0)
        return;

    int shaderProgramID = shaders->getShader(0);

    QVector4D ambientProduct = light.ambient * objects[0]->material.ambient;
    QVector4D diffuseProduct = light.diffuse * objects[0]->material.diffuse;
    QVector4D specularProduct = light.specular * objects[0]->material.specular;

    GLuint locProjection = glGetUniformLocation(shaderProgramID, "projection");
    GLuint locView = glGetUniformLocation(shaderProgramID, "view");
    GLuint locLightPosition = glGetUniformLocation(shaderProgramID, "lightPosition");
    GLuint locAmbientProduct = glGetUniformLocation(shaderProgramID, "ambientProduct");
    GLuint locDiffuseProduct = glGetUniformLocation(shaderProgramID, "diffuseProduct");
    GLuint locSpecularProduct = glGetUniformLocation(shaderProgramID, "specularProduct");
    GLuint locShininess = glGetUniformLocation(shaderProgramID, "shininess");

    glUseProgram(shaderProgramID);

    glUniformMatrix4fv(locProjection, 1, GL_FALSE, camera.projectionMatrix.data());
    glUniformMatrix4fv(locView, 1, GL_FALSE, camera.viewMatrix.data());
    glUniform4fv(locLightPosition, 1, &(light.position[0]));
    glUniform4fv(locAmbientProduct, 1, &(ambientProduct[0]));
    glUniform4fv(locDiffuseProduct, 1, &(diffuseProduct[0]));
    glUniform4fv(locSpecularProduct, 1, &(specularProduct[0]));
    glUniform1f(locShininess, objects[0]->material.shininess);

    for (int i = 0; i < objects.size(); ++i) {
        objects[i]->drawModel();
    }

    if(trajectory->count>0)
        trajectory->drawModel();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    camera.resizeViewport(width, height);

    if (objects.size()>0)
        objects[0]->trackBall.resizeViewport(width, height);
    update();
}


void OpenGLWidget::animate()
{
    float elapsedTime = time.restart() / 1000.0f;

    for (int i = 0; i < objects.size(); ++i){
         objects[i]->Rotate(elapsedTime);
         objects[i]->Translate(elapsedTime);
    }

    update();
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
   //if(!model) return;
   //this->model->zoom += 0.001 * event->delta();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!objects[0])
        return;

    objects[0]->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (!objects[0])
        return;

    if (event->button() & Qt::LeftButton)
        objects[0]->trackBall.mousePress(event->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!objects[0])
        return;

    if (event->button() & Qt::LeftButton)
        objects[0]->trackBall.mouseRelease(event->localPos());
}

// Strong focus is required
void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        QApplication::quit();
    }
}

void OpenGLWidget::start(){
    shaders->initializeGL();
    shaders->createShaders();

    auto sun = factory->GetSun();
    auto earth = factory->GetEarth(sun);
    auto moon = factory->GetMoon(earth);

    objects.push_back(sun);
    objects.push_back(earth);
    objects.push_back(moon);

    QVector3D start = QVector3D(0,0,0);
    trajectory->addTrajectory(start, QVector3D(1,0,0), earth->orbitDistance);
    //trajectory->addTrajectory(QVector3D(-0.5f, -0.5f, 0.0f), QVector3D(1,0,0), 0.3);
    //trajectory->addTrajectory(QVector3D(0.5f,  -0.5f, 0.0f), QVector3D(0,1,0), 0.3);
    //trajectory->addTrajectory(QVector3D(0.0f,   0.5f, 0.0f), QVector3D(0,0,1), 0.3);

    trajectory->startTrajectory();

    update();
}
