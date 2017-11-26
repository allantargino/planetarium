#ifndef MODEL_H
#define MODEL_H

#include <QtOpenGL>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QTextStream>
#include <QFile>

#include <fstream>
#include <limits>
#include <iostream>
#include <memory>

#include "light.h"
#include "trackball.h"
#include "material.h"

class Model : public QOpenGLExtraFunctions
{
public:
    Model(QOpenGLWidget *_glWidget);
    ~Model();

    std::unique_ptr<QVector4D []> vertices;
    std::unique_ptr<QVector3D []> normals;
    std::unique_ptr<unsigned int[]> indices;
    std::unique_ptr<QVector2D []> texCoords;
    std::unique_ptr<QVector4D []> tangents;

    QOpenGLWidget *glWidget;

    unsigned int numVertices;
    unsigned int numFaces;

    void createNormals();
    void createTexCoords();
    void createTangents();

    GLuint vao = 0;

    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint vboNormals = 0;
    GLuint vboTexCoords = 0;
    GLuint vboTangents = 0;
    GLuint textureID = 0;

    void createVBOs();
    void createShaders();

    void destroyVBOs();
    void destroyShaders();

    void drawModel();

    QMatrix4x4 modelMatrix;
    QVector3D midPoint;
    double invDiag;
    double zoom = 0;

    Material material;
    TrackBall trackBall;

    int shaderIndex;
    int numShaders;

    std::vector<GLuint> shaderProgram;

    void readOFFFile(const QString &fileName);
    void loadTexture(const QImage &image);

    QVector3D position = QVector3D(0.0, 0.0, 0.0);
    float scale = 1.0;
    float angle = 0.0;
};

#endif // MODEL_H
