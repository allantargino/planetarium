#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class ShaderManager : public QOpenGLExtraFunctions
{
public :
    ShaderManager(QOpenGLWidget *_glWidget);
    ~ShaderManager();

    void createShaders();
    void destroyShaders();

    void initializeGL();

    GLuint getShader(int index);

protected:
    QOpenGLWidget* glWidget;

    int numShaders;
    std::vector<GLuint> shaderProgram;
};

#endif // SHADERMANAGER_H
