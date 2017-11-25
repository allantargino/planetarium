#include "model.h"

Model::Model(QOpenGLWidget *_glWidget)
{   
    glWidget = _glWidget;
    glWidget->makeCurrent();

    initializeOpenGLFunctions();

    shaderIndex = 0;
    numShaders = 0;
}

Model::~Model()
{
    destroyVBOs();
    destroyShaders();
}

void Model::destroyVBOs()
{
    GL_CHECK(glDeleteBuffers(1, &vboVertices));
    GL_CHECK(glDeleteBuffers(1, &vboIndices));
    GL_CHECK(glDeleteBuffers(1, &vboNormals));
    GL_CHECK(glDeleteBuffers(1, &vboTexCoords));
    GL_CHECK(glDeleteBuffers(1, &vboTangents));

    GL_CHECK(glDeleteVertexArrays(1, &vao));

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

    GL_CHECK(glGenVertexArrays(1, &vao));
    GL_CHECK(glBindVertexArray(vao));

    GL_CHECK(glGenBuffers(1, &vboVertices));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboVertices));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), vertices.get(), GL_STATIC_DRAW));
    GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CHECK(glEnableVertexAttribArray(0));
    vertices.reset();

    GL_CHECK(glGenBuffers(1, &vboNormals));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboNormals));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector3D), normals.get(), GL_STATIC_DRAW));
    GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CHECK(glEnableVertexAttribArray(1));
    normals.reset();

    GL_CHECK(glGenBuffers(1, &vboTexCoords));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector2D), texCoords.get(), GL_STATIC_DRAW));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboTexCoords));
    GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CHECK(glEnableVertexAttribArray(2));
    texCoords.reset();

    GL_CHECK(glGenBuffers(1, &vboTangents));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vboTangents));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(QVector4D), tangents.get(), GL_STATIC_DRAW));
    GL_CHECK(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr));
    GL_CHECK(glEnableVertexAttribArray(3));
    tangents.reset();

    GL_CHECK(glGenBuffers(1, &vboIndices));
    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, numFaces * 3 * sizeof(unsigned int), indices.get(), GL_STATIC_DRAW));
    indices.reset();
}


void Model::destroyShaders ()
{
    for (GLuint shaderProgramID : shaderProgram)
    {
        GL_CHECK(glDeleteProgram(shaderProgramID));
    }
    shaderProgram.clear();
}

void Model::createShaders ()
{
    numShaders = 9;

    QString vertexShaderFile[]  = {":/shaders/shaders/vshader1.glsl",
                                   ":/shaders/shaders/vflat.glsl",
                                   ":/shaders/shaders/vgouraud.glsl",
                                   ":/shaders/shaders/vphong.glsl",
                                   ":/shaders/shaders/vnormal.glsl",
                                   ":/shaders/shaders/vtexture.glsl",
                                   ":/shaders/shaders/vtex2.glsl",
                                   ":/shaders/shaders/vnormalmap.glsl",
                                   ":/shaders/shaders/vcubemap.glsl"};

    QString fragmentShaderFile[] = {":/shaders/shaders/fshader1.glsl",
                                    ":/shaders/shaders/fflat.glsl",
                                    ":/shaders/shaders/fgouraud.glsl",
                                    ":/shaders/shaders/fphong.glsl",
                                    ":/shaders/shaders/fnormal.glsl",
                                    ":/shaders/shaders/ftexture.glsl",
                                    ":/shaders/shaders/ftex2.glsl",
                                    ":/shaders/shaders/fnormalmap.glsl",
                                    ":/shaders/shaders/fcubemap.glsl"};

    destroyShaders();

    shaderProgram.clear();

    for (int i = 0; i < numShaders; ++i)
    {
        QFile vs(vertexShaderFile[i]);
        QFile fs(fragmentShaderFile[i]);

        vs.open(QFile::ReadOnly | QFile::Text);
        fs.open(QFile::ReadOnly | QFile::Text);

        QTextStream streamVs(&vs), streamFs(&fs);

        QString qtStringVs = streamVs.readAll();
        QString qtStringFs = streamFs.readAll();

        std::string stdStringVs = qtStringVs.toStdString();
        std::string stdStringFs = qtStringFs.toStdString();

        // Create an empty vertex shader handle
        GLuint vertexShader = 0;
        GL_CHECK(vertexShader = glCreateShader(GL_VERTEX_SHADER));

        // Send the vertex shader source code to GL
        const GLchar *source = stdStringVs.c_str();

        GL_CHECK(glShaderSource(vertexShader, 1, &source, 0));

        // Compile the vertex shader
        GL_CHECK(glCompileShader(vertexShader));

        GLint isCompiled = 0;
        GL_CHECK(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled));
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            GL_CHECK(glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength));
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            GL_CHECK(glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]));
            qDebug("%s", &infoLog[0]);

            GL_CHECK(glDeleteShader(vertexShader));
            return;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = 0;
        GL_CHECK(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = stdStringFs.c_str();//(const GLchar *)stringFs.toStdString().c_str();
        GL_CHECK(glShaderSource(fragmentShader, 1, &source, 0));

        // Compile the fragment shader
        GL_CHECK(glCompileShader(fragmentShader));

        GL_CHECK(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled));
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            GL_CHECK(glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength));

            std::vector<GLchar> infoLog(maxLength);
            GL_CHECK(glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]));
            qDebug("%s", &infoLog[0]);

            GL_CHECK(glDeleteShader(fragmentShader));
            GL_CHECK(glDeleteShader(vertexShader));
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        GLuint shaderProgramID = 0;
        GL_CHECK(shaderProgramID = glCreateProgram());
        shaderProgram.push_back(shaderProgramID);

        // Attach our shaders to our program
        GL_CHECK(glAttachShader(shaderProgramID, vertexShader));
        GL_CHECK(glAttachShader(shaderProgramID, fragmentShader));

        // Link our program
        GL_CHECK(glLinkProgram(shaderProgramID));

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        GL_CHECK(glGetProgramiv(shaderProgramID, GL_LINK_STATUS, (int *)&isLinked));
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            GL_CHECK(glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength));

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            GL_CHECK(glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &infoLog[0]));
            qDebug("%s", &infoLog[0]);

            GL_CHECK(glDeleteProgram(shaderProgramID));
            GL_CHECK(glDeleteShader(vertexShader));
            GL_CHECK(glDeleteShader(fragmentShader));
            return;
        }

        GL_CHECK(glDetachShader(shaderProgramID, vertexShader));
        GL_CHECK(glDetachShader(shaderProgramID, fragmentShader));

        GL_CHECK(glDeleteShader(vertexShader));
        GL_CHECK(glDeleteShader(fragmentShader));

        vs.close();
        fs.close();
    }
}

void Model::drawModel()
{
    modelMatrix.setToIdentity();
    modelMatrix.translate(0, 0, zoom);
    modelMatrix.rotate(trackBall.getRotation());
    modelMatrix.scale(invDiag, invDiag, invDiag);
    modelMatrix.translate(-midPoint);

    GLuint locModel = 0;
    GLuint locNormalMatrix = 0;
    GLuint locShininess = 0;
    GL_CHECK(locModel = glGetUniformLocation(shaderProgram[shaderIndex], "model"));
    GL_CHECK(locNormalMatrix = glGetUniformLocation(shaderProgram[shaderIndex], "normalMatrix"));
    GL_CHECK(locShininess = glGetUniformLocation(shaderProgram[shaderIndex], "shininess"));

    GL_CHECK(glBindVertexArray(vao));
   // GL_CHECK(glUseProgram(shaderProgram[shaderIndex]));

    GL_CHECK(glUniformMatrix4fv(locModel, 1, GL_FALSE, modelMatrix.data()));
    GL_CHECK(glUniformMatrix3fv(locNormalMatrix, 1, GL_FALSE, modelMatrix.normalMatrix().data()));
    GL_CHECK(glUniform1f(locShininess, static_cast<GLfloat>(material.shininess)));

    if (textureID)
    {
        GLuint locColorTexture = 0;
        GL_CHECK(locColorTexture = glGetUniformLocation(shaderProgram[shaderIndex], "colorTexture"));
        glUniform1i(locColorTexture, 0);

        GL_CHECK(glActiveTexture(GL_TEXTURE0));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureID));
    }

    if (textureLayerID)
    {
        GLuint locColorTextureLayer = 1;
        GL_CHECK(locColorTextureLayer = glGetUniformLocation(shaderProgram[shaderIndex], "colorTextureLayer"));
        glUniform1i(locColorTextureLayer, 1);

        GL_CHECK(glActiveTexture(GL_TEXTURE1));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureLayerID));
    }

    if (textureCubeMapID)
    {
        GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMapID));
    }

    GL_CHECK(glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0));
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
    createShaders();
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
        GL_CHECK(glDeleteTextures(1, &textureID));
    }

    GL_CHECK(glGenTextures(1, &textureID));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureID));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits()));

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
}

void Model::loadTextureLayer(const QImage &image)
{   
    if (textureLayerID)
    {
        GL_CHECK(glDeleteTextures(1, &textureLayerID));
    }

    GL_CHECK(glGenTextures(1, &textureLayerID));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureLayerID));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits()));

    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
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

void Model::loadCubeMapTexture(QString folderName)
{
    QImage negx = QImage(QString(folderName).append("/negx.jpg")).convertToFormat(QImage::Format_RGBA8888 );
    QImage negy = QImage(QString(folderName).append("/negy.jpg")).convertToFormat(QImage::Format_RGBA8888 );
    QImage negz = QImage(QString(folderName).append("/negz.jpg")).convertToFormat(QImage::Format_RGBA8888 );
    QImage posx = QImage(QString(folderName).append("/posx.jpg")).convertToFormat(QImage::Format_RGBA8888 );
    QImage posy = QImage(QString(folderName).append("/posy.jpg")).convertToFormat(QImage::Format_RGBA8888 );
    QImage posz = QImage(QString(folderName).append("/posz.jpg")).convertToFormat(QImage::Format_RGBA8888 );

    negx = negx.convertToFormat(QImage::Format_RGBA8888);
    negy = negy.convertToFormat(QImage::Format_RGBA8888);
    negz = negz.convertToFormat(QImage::Format_RGBA8888);
    posx = posx.convertToFormat(QImage::Format_RGBA8888);
    posy = posy.convertToFormat(QImage::Format_RGBA8888);
    posz = posz.convertToFormat(QImage::Format_RGBA8888);

    if (textureCubeMapID)
    {
        GL_CHECK(glDeleteTextures(1, &textureCubeMapID));
    }

    GL_CHECK(glGenTextures(1, &textureCubeMapID));
    GL_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMapID));

    GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, negx.width(), negx.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negx.bits()));
    GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, negy.width(), negy.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negy.bits()));
    GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, negz.width(), negz.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, negz.bits()));
    GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, posx.width(), posx.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, posx.bits()));
    GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, posy.width(), posy.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, posy.bits()));
    GL_CHECK(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, posz.width(), posz.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, posz.bits()));

    GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CHECK(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
}

