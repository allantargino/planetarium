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
    QString fileName = ".\\offmodels\\sphere.off";
    //Shader
    int shaderIndex = 5;
    //Texture
    QString textureFileName = ".\\textures\\sun.jpg";

    auto model = std::make_shared<AstronomicalObject>(glWidget);

    model->shaderIndex = shaderIndex;
    model->readOFFFile(fileName);

    model->trackBall.resizeViewport(glWidget->width(), glWidget->height());

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
