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

    //Planet Info
    float radius = 500.0f;
    float distanceSun = 0.0f;
    //Orbit
    float orbitSpeed = 0.0f;
    AstronomicalObject* orbitObject = nullptr;
    //Translation
    float translationSpeed = 0.0f;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetMercury(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\mercury.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 38.2f;
    float distanceSun = 350.0f;
    //Orbit
    float orbitSpeed = 5;
    AstronomicalObject* orbitObject = sun.get();
    //Translation
    float translationSpeed = 10;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetVenus(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\venus.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 94.9f;
    float distanceSun = 450.0f;
    //Orbit
    float orbitSpeed = 4;
    AstronomicalObject* orbitObject = sun.get();
    //Translation
    float translationSpeed = 10;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
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
    float distanceSun = 600.0f;
    //Orbit
    float orbitSpeed = 1;
    AstronomicalObject* orbitObject = sun.get();
    //Translation
    float translationSpeed = 10;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
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
    float distanceEarth = 50.0f;
    //Orbit
    float orbitSpeed = 5;
    AstronomicalObject* orbitObject = earth.get();;
    //Translation
    float translationSpeed = 0.0f;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceEarth, orbitSpeed, orbitObject, translationSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetMars(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\mars.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 53.0f;
    float distanceSun = 700.0f;
    //Orbit
    float orbitSpeed = 1.03f;
    AstronomicalObject* orbitObject = sun.get();
    //Translation
    float translationSpeed = 10;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetJupiter(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\jupiter.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 250.0f;
    float distanceSun = 900.0f;
    //Orbit
    float orbitSpeed = 0.41f;
    AstronomicalObject* orbitObject = sun.get();
    //Translation
    float translationSpeed = 10;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetSaturn(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\saturn.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 200.0f;
    float distanceSun = 1200.0f;
    //Orbit
    float orbitSpeed = 0.44f;
    AstronomicalObject* orbitObject = sun.get();
    //Translation
    float translationSpeed = 10;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetSaturnRing(std::shared_ptr<AstronomicalObject> saturn){
    //Off Model
    QString offModelFileName = ".\\offmodels\\ring.off";
    //Texture
    QString textureFileName = ".\\textures\\rings.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 270.0f;
    float distanceSaturn = 0.0f;
    //Orbit
    float orbitSpeed = 0.44f;
    AstronomicalObject* orbitObject = saturn.get();;
    //Translation
    float translationSpeed = 0.0f;

    auto ring = GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSaturn, orbitSpeed, orbitObject, translationSpeed);
    ring->angle = saturn->angle;

    return ring;
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetUranus(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\uranus.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 150.0f;
    float distanceSun = 1400.0f;
    //Orbit
    float orbitSpeed = 0.72f;
    AstronomicalObject* orbitObject = sun.get();
    //Translation
    float translationSpeed = 10;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
}

std::shared_ptr<AstronomicalObject> ModelFactory::GetNeptune(std::shared_ptr<AstronomicalObject> sun){
    //Off Model
    QString offModelFileName = ".\\offmodels\\sphere.off";
    //Texture
    QString textureFileName = ".\\textures\\neptune.jpg";
    //Shader
    int shaderIndex = 0;
    //Physical Info
    float radius = 125.0f;
    float distanceSun = 1700.0f;
    //Orbit
    float orbitSpeed = 0.72f;
    AstronomicalObject* orbitObject = sun.get();
    //Translation
    float translationSpeed = 10;

    return GetModel(offModelFileName, textureFileName, shaderIndex, radius, distanceSun, orbitSpeed, orbitObject, translationSpeed);
}


std::shared_ptr<AstronomicalObject> ModelFactory::GetModel(const QString offModelFileName, const QString textureFileName, int shaderIndex, float radius, float distanceOrbitObject, float orbitSpeed, AstronomicalObject* orbitObject, float translationSpeed){
    //Normalization
    float maxScale = 1000;
    float radiusNormalized = radius/maxScale;
    float distanceOrbitObjectNormalized = distanceOrbitObject/maxScale;

    //Create astronomical object
    auto model = std::make_shared<AstronomicalObject>(glWidget);

    //Physical info
    model->scale = radiusNormalized;
    model->position = QVector3D(distanceOrbitObjectNormalized, 0.0, 0.0);
    model->angle = Utils::GetRandomNumber(0.0f, 360.0f);

    model->orbitDistance = distanceOrbitObjectNormalized;
    model->orbitSpeed = orbitSpeed;
    model->orbitObject = orbitObject;
    model->orbitAngleTheta = Utils::GetRandomNumber(0.0f, 360.0f);

    model->translationSpeed = translationSpeed;

    //Shader
    model->shaderProgram = shaders->getShader(shaderIndex);
    model->readOFFFile(offModelFileName);

    //Texture
    QImage image = GetTexture(textureFileName);
    model->loadTexture(image);

    return model;
}

QImage ModelFactory::GetTexture(const QString textureFileName){
    QImage image;
    image.load(textureFileName);
    image = image.convertToFormat(QImage::Format_RGBA8888);
    return image;
}
