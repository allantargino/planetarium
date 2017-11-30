#include "model.h"

Model::Model(QOpenGLWidget *_glWidget)
{   
    glWidget = _glWidget;
    glWidget->makeCurrent();

    initializeOpenGLFunctions();
}

Model::~Model()
{
    destroyVBOs();
}

void Model::destroyVBOs()
{
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboIndices);
    glDeleteBuffers(1, &vboNormals);
    glDeleteBuffers(1, &vboTexCoords);
    glDeleteBuffers(1, &vboTangents);

    glDeleteVertexArrays(1, &vao);

    vboVertices = 0;
    vboIndices = 0;
    vboNormals = 0;
    vboTexCoords = 0;
    vboTangents = 0;
    vao = 0;
}

void Model::createVBOs()
{
    glWidget->makeCurrent();

    destroyVBOs();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    vertices.reset();

    glGenBuffers(1, &vboNormals);
    glBindBuffer(GL_ARRAY_BUFFER, vboNormals);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector3D), normals.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
    normals.reset();

    glGenBuffers(1, &vboTexCoords);
    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector2D), texCoords.get(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);
    texCoords.reset();

    glGenBuffers(1, &vboTangents);
    glBindBuffer(GL_ARRAY_BUFFER, vboTangents);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), tangents.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(3);
    tangents.reset();

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices.get(), GL_STATIC_DRAW);
    indices.reset();
}

void Model::drawModel()
{
    float fixedAngle = -90.0f;

    modelMatrix.setToIdentity();
    modelMatrix.translate(position);
    modelMatrix.rotate(angle, 0.0, 1.0, 0.0);
    modelMatrix.rotate(fixedAngle, 1.0, 0.0, 0.0);
    modelMatrix.scale(invDiag * scale, invDiag * scale, invDiag*scale);
    modelMatrix.translate(-midPoint);

    GLuint locModel = 0;
    GLuint locNormalMatrix = 0;
    GLuint locShininess = 0;
    locModel = glGetUniformLocation(shaderProgram, "model");
    locNormalMatrix = glGetUniformLocation(shaderProgram, "normalMatrix");
    locShininess = glGetUniformLocation(shaderProgram, "shininess");

    glBindVertexArray(vao);
   // GL_CHECK(glUseProgram(shaderProgram[shaderIndex]));

    glUniformMatrix4fv(locModel, 1, GL_FALSE, modelMatrix.data());
    glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, modelMatrix.normalMatrix().data());
    glUniform1f(locShininess, static_cast<GLfloat>(material.shininess));

    if (textureID)
    {
        GLuint locColorTexture = 0;
        locColorTexture = glGetUniformLocation(shaderProgram, "colorTexture");
        glUniform1i(locColorTexture, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);
}

void Model::readOFFFile(QString const &fileName)
{
    std::ifstream stream;
    stream.open(fileName.toUtf8(),std::ifstream::in);
    if (!stream.is_open())
    {
        qWarning("Cannot open file.");
        return;
    }

    std::string line;
    stream >> line;
    stream >> numVertices >> numFaces >> line;

    // http://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
    vertices = std::make_unique<QVector4D[]>(numVertices);
    indices = std::make_unique<unsigned int[]>(numFaces * 3);

    if (numVertices > 0)
    {
        float minLim = std::numeric_limits<float>::lowest();
        float maxLim = std::numeric_limits<float>::max();
        QVector4D max(minLim, minLim, minLim, 1.0);
        QVector4D min(maxLim, maxLim, maxLim, 1.0);
        for (unsigned int i = 0; i < numVertices; ++i)
        {
            float x, y, z;
            stream >> x >> y >> z;
            max.setX(std::max(max.x(), x));
            max.setY(std::max(max.y(), y));
            max.setZ(std::max(max.z(), z));
            min.setX(std::min(min.x(), x));
            min.setY(std::min(min.y(), y));
            min.setZ(std::min(min.z(), z));
            vertices[i] = QVector4D(x, y, z, 1.0);
        }

        midPoint = QVector3D((min + max) * 0.5);
        invDiag = 1 / (max - min).length();
    }

    for (unsigned int i = 0; i < numFaces; ++i)
    {
        unsigned int a, b, c;
        stream >> line >> a >> b >> c;
        indices[i * 3 + 0] = a;
        indices[i * 3 + 1] = b;
        indices[i * 3 + 2] = c;
    }
    stream.close();

    createNormals();
    createTexCoords();
    createTangents();
    createVBOs();
}

void Model::createNormals()
{
    normals = std::make_unique<QVector3D[]>(numVertices);

    for (unsigned int i = 0; i < numFaces; ++i)
    {
        QVector3D a = QVector3D(vertices[indices[i * 3 + 0]]);
        QVector3D b = QVector3D(vertices[indices[i * 3 + 1]]);
        QVector3D c = QVector3D(vertices[indices[i * 3 + 2]]);
        QVector3D faceNormal = QVector3D::crossProduct((b - a), (c - b));

        // Accumulates face normals on the vertices
        normals[indices[i * 3 + 0]] += faceNormal;
        normals[indices[i * 3 + 1]] += faceNormal;
        normals[indices[i * 3 + 2]] += faceNormal;
    }

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        normals[i].normalize();
    }
}

void Model::createTexCoords()
{
    texCoords = std::make_unique<QVector2D[]>(numVertices);

    // Compute minimum and maximum values
    auto minz = std::numeric_limits<float>::max();
    auto maxz = std::numeric_limits<float>::lowest();

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        minz = std::min(vertices[i].z(), minz);
        maxz = std::max(vertices[i].z(), maxz);
    }

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        auto s = (std::atan2(vertices[i].y(), vertices[i].x()) + M_PI) / (2 * M_PI);
        auto t = 1.0f - (vertices[i].z() - minz) / (maxz - minz);
        texCoords[i] = QVector2D(s, t);
    }
}

void Model::loadTexture(const QImage &image)
{
    if (textureID)
    {
        glDeleteTextures(1, &textureID);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Model::createTangents()
{
    tangents = std::make_unique<QVector4D[]>(numVertices);

    std::unique_ptr<QVector3D[]> bitangents;
    bitangents = std::make_unique<QVector3D[]>(numVertices);

    for (unsigned int i = 0; i < numFaces ; ++i)
    {
        unsigned int i1 = indices[i * 3 + 0];
        unsigned int i2 = indices[i * 3 + 1];
        unsigned int i3 = indices[i * 3 + 2];

        QVector3D E = vertices[i1].toVector3D();
        QVector3D F = vertices[i2].toVector3D();
        QVector3D G = vertices[i3].toVector3D();

        QVector2D stE = texCoords[i1];
        QVector2D stF = texCoords[i2];
        QVector2D stG = texCoords[i3];

        QVector3D P = F - E;
        QVector3D Q = G - E;

        QVector2D st1 = stF - stE;
        QVector2D st2 = stG - stE;

        QMatrix2x2 M;
        M(0, 0) =  st2.y();
        M(0, 1) = -st1.y();
        M(1, 0) = -st2.x();
        M(1, 1) =  st1.x();
        M *= (1.0 / (st1.x() * st2.y() - st2.x() * st1.y()));

        QVector4D T = QVector4D (M(0, 0) * P.x() + M(0, 1) * Q.x(),
                                 M(0, 0) * P.y() + M(0, 1) * Q.y(),
                                 M(0, 0) * P.z() + M(0, 1) * Q.z(), 0.0);

        QVector3D B = QVector3D (M(1, 0) * P.x() + M(1, 1) * Q.x(),
                                 M(1, 0) * P.y() + M(1, 1) * Q.y(),
                                 M(1, 0) * P.z() + M(1, 1) * Q.z());
        tangents[i1] += T;
        tangents[i2] += T;
        tangents[i3] += T;

        bitangents[i1] += B;
        bitangents[i2] += B;
        bitangents[i3] += B;
    }

    for (unsigned int i = 0; i < numVertices; ++i)
    {
        const QVector3D &n = normals[i];
        const QVector4D &t = tangents[i];
        tangents[i] = (t - n * QVector3D::dotProduct(n, t.toVector3D())).normalized();
        QVector3D b = QVector3D::crossProduct(n, t.toVector3D());
        double hand = QVector3D::dotProduct(b, bitangents[i]);
        tangents[i].setW((hand < 0.0) ? -1.0 : 1.0);
    }
}
