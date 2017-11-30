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

    int size = objects.size();
    for (int i = 0; i < size; ++i) {
        objects[i]->drawModel();
    }

    if(trajectory->count>0){
        GLuint locProjection2 = glGetUniformLocation(trajectory->shaderProgram, "projection");
        GLuint locView2 = glGetUniformLocation(trajectory->shaderProgram, "view");

        glUseProgram(trajectory->shaderProgram);

        glUniformMatrix4fv(locProjection2, 1, GL_FALSE, camera.projectionMatrix.data());
        glUniformMatrix4fv(locView2, 1, GL_FALSE, camera.viewMatrix.data());

        trajectory->drawModel();

    }
}
void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    camera.resizeViewport(width, height);

    update();
}


void OpenGLWidget::animate()
{
    float elapsedTime = time.restart() / 1000.0f;

    int size = objects.size();
    for (int i = 0; i < size; ++i){
         objects[i]->Rotate(elapsedTime);
         objects[i]->Translate(elapsedTime);
    }

    update();
}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        camera.MoveCameraLeft();
        break;
    case Qt::Key_Right:
        camera.MoveCameraRight();
        break;
    case Qt::Key_Up:
        camera.MoveCameraUp();
        break;
    case Qt::Key_Down:
        camera.MoveCameraDown();
        break;
    case Qt::Key_1:
        camera.IncreaseDistance();
        break;
    case Qt::Key_2:
        camera.DecreaseDistance();
        break;
    case Qt::Key_Escape:
        qApp->quit();
        break;
    default:
        break;
    }
    update();
}
void OpenGLWidget::start(){
    shaders->initializeGL();
    shaders->createShaders();

    auto sun = factory->GetSun();
    auto mercury = factory->GetMercury(sun);
    auto venus = factory->GetVenus(sun);
    auto earth = factory->GetEarth(sun);
    auto moon = factory->GetMoon(earth);
    auto mars = factory->GetMars(sun);
    auto jupiter = factory->GetJupiter(sun);
    auto saturn = factory->GetSaturn(sun);
    auto ring = factory->GetSaturnRing(saturn);
    auto uranus = factory->GetUranus(sun);
    auto neptune = factory->GetNeptune(sun);

    objects.push_back(sun);
    objects.push_back(mercury);
    objects.push_back(venus);
    objects.push_back(earth);
    objects.push_back(moon);
    objects.push_back(mars);
    objects.push_back(jupiter);
    objects.push_back(saturn);
    objects.push_back(ring);
    objects.push_back(uranus);
    objects.push_back(neptune);

    QVector3D start = QVector3D(0,0,0);
    trajectory->addTrajectory(start, QVector3D(0.0,0.0,1.0), mercury->orbitDistance * 2);
    trajectory->addTrajectory(start, QVector3D(0.0,1.0,0.0), venus->orbitDistance * 2);
    trajectory->addTrajectory(start, QVector3D(1.0,0.0,0.0), earth->orbitDistance * 2);
    trajectory->addTrajectory(start, QVector3D(0.0,0.7,0.3), mars->orbitDistance * 2);
    trajectory->addTrajectory(start, QVector3D(0.5,1.0,0.5), jupiter->orbitDistance * 2);
    trajectory->addTrajectory(start, QVector3D(1.0,1.0,0.0), saturn->orbitDistance * 2);
    trajectory->addTrajectory(start, QVector3D(1.0,0.0,1.0), uranus->orbitDistance * 2);
    trajectory->addTrajectory(start, QVector3D(0.3,1.0,0.7), neptune->orbitDistance * 2);
    trajectory->startTrajectory();

    update();
}
