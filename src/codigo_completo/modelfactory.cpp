#include "modelfactory.h"

ModelFactory::ModelFactory(QOpenGLWidget* _glWidget, ShaderManager* _shaders)
{
    glWidget = _glWidget;
    shaders = _shaders;
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
    int shaderIndex = 0;
    //Physical Info
    float radius = 500.0f;
    float distanceSun = 0.0f;
    //Orbit
    float orbitSpeed = 0.0;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetEarth(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\earth.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 100.0f;
    float distanceSun = 700.0f;
    //Orbit
    float orbitSpeed = 0.8;
    //Translation

    auto earth = GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed);
    earth->orbitObject = sun.get();
    //earth->orbitDistance = 1.0;

    earth->translationSpeed = 10;

    return earth;
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetMoon(std::shared_ptr<AstronomicalObject> earth){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\moon.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 50.0f;
    float distanceEarth = 125.0f;
    //Orbit
    float orbitSpeed = 4.0;

    auto moon = GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceEarth, orbitSpeed);
    moon->orbitObject = earth.get();
    //moon->orbitDistance = 0.2;

    return moon;
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetModel(const QString offModelFileName, const QString textureFileName, int shaderIndex, float radius, float distanceOrbitObject, float orbitSpeed){

    float maxScale = 1000;
    float radiusNormalized = radius/maxScale;
    float distanceOrbitObjectNormalized = distanceOrbitObject/maxScale;

    auto model = std::make_shared<AstronomicalObject>(glWidget);

    model->scale = radiusNormalized;
    model->position = QVector3D(distanceOrbitObjectNormalized, 0.0, 0.0);
    model->orbitDistance = distanceOrbitObjectNormalized;

    //TODO: Random?
    model->angle = 0.0f;

    model->shaderProgram = shaders->getShader(shaderIndex);
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
