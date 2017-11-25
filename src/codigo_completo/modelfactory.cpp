#include "modelfactory.h"

ModelFactory::ModelFactory(QOpenGLWidget* _glWidget)
{
    glWidget = _glWidget;
}

ModelFactory::~ModelFactory()
{
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetSun(){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\sun.jpg";
    //Shader
    int shaderIndex = 5;
    //Position
    QVector3D position = QVector3D(0.0, 0.0, 0.0);
    float scale = 1.5;
    float angle = 0.0;
    //Orbit
    float orbitSpeed = 0.0;

    return GetModel(offModelFileName, textureFileName, shaderIndex, position, scale, angle, orbitSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetEarth(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\earth.jpg";
    //Shader
    int shaderIndex = 5;
    //Position
    QVector3D position = QVector3D(1.0, 0.0, 0.0);
    float scale = 0.5;
    float angle = 0.0;
    //Orbit
    float orbitSpeed = 1.0;

    auto earth = GetModel(offModelFileName, textureFileName, shaderIndex, position, scale, angle, orbitSpeed);
    earth->orbitObject = sun.get();

    return earth;
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetModel(const QString offModelFileName, const QString textureFileName, int shaderIndex, const QVector3D position, float scale, float angle, float orbitSpeed){
    auto model = std::make_shared<AstronomicalObject>(glWidget);

    model->position = position;
    model->scale = scale;
    model->angle = angle;

    model->shaderIndex = shaderIndex;
    model->readOFFFile(offModelFileName);

    model->trackBall.resizeViewport(glWidget->width(), glWidget->height());

    QImage image = GetTexture(textureFileName);
    model->loadTexture(image);

    model->orbitSpeed = orbitSpeed;

    return model;
}

QImage ModelFactory::GetTexture(const QString textureFileName){
    QImage image;
    image.load(textureFileName);
    image = image.convertToFormat(QImage::Format_RGBA8888);
    return image;
}
