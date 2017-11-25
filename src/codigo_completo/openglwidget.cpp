#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget * parent) : QOpenGLWidget(parent)
{
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
   if(!model) return;
   this->model->zoom += 0.001 * event->delta();
}

void OpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug("OpenGL version: %s", glGetString(GL_VERSION));
    qDebug("GLSL %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);

    connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer.start(0);
}

void OpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!model)
        return;

    int shaderProgramID = model->shaderProgram[model->shaderIndex];

    QVector4D ambientProduct = light.ambient * model->material.ambient;
    QVector4D diffuseProduct = light.diffuse * model->material.diffuse;
    QVector4D specularProduct = light.specular * model->material.specular;

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
    glUniform1f(locShininess, model->material.shininess);

    model->drawModel();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    camera.resizeViewport(width, height);

    if (model)
        model->trackBall.resizeViewport(width, height);
    update();
}


void OpenGLWidget::animate()
{
    update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!model)
        return;

    model->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (!model)
        return;

    if (event->button() & Qt::LeftButton)
        model->trackBall.mousePress(event->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!model)
        return;

    if (event->button() & Qt::LeftButton)
        model->trackBall.mouseRelease(event->localPos());
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
    //Offmodel
    QString fileName = ".\\offmodels\\sphere.off";
    int shaderIndex = 5;
    if (!fileName.isEmpty())
    {
        if (model)
            shaderIndex = model->shaderIndex;

        model = std::make_shared<Model>(this);
        model->shaderIndex = shaderIndex;
        model->readOFFFile(fileName);

        model->trackBall.resizeViewport(width(), height());
    }

    //Texture
    QString textureFileName = ".\\textures\\sun.jpg";
    if (!textureFileName.isEmpty() && model)
    {
        QImage image;
        image.load(textureFileName);
        image = image.convertToFormat(QImage::Format_RGBA8888);

        model->loadTexture(image);
    }

    update();
}
