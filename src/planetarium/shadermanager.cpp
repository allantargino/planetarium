#include "shadermanager.h"

ShaderManager::ShaderManager(QOpenGLWidget*_glWidget){
    glWidget = _glWidget;

    glWidget->makeCurrent();
}

ShaderManager::~ShaderManager(){
    destroyShaders();
}

void ShaderManager::initializeGL(){
    initializeOpenGLFunctions();
}

void ShaderManager::createShaders ()
{
    numShaders = 1;

    QString vertexShaderFile[]  = {":/shaders/shaders/vtexture.glsl"};

//    QString vertexShaderFile[]  = {":/shaders/shaders/vshader1.glsl",
//                                   ":/shaders/shaders/vflat.glsl",
//                                   ":/shaders/shaders/vgouraud.glsl",
//                                   ":/shaders/shaders/vphong.glsl",
//                                   ":/shaders/shaders/vnormal.glsl",
//                                   ":/shaders/shaders/vtexture.glsl",
//                                   ":/shaders/shaders/vtex2.glsl",
//                                   ":/shaders/shaders/vnormalmap.glsl",
//                                   ":/shaders/shaders/vcubemap.glsl"};

    QString fragmentShaderFile[] = {":/shaders/shaders/ftexture.glsl"};

//    QString fragmentShaderFile[] = {":/shaders/shaders/fshader1.glsl",
//                                    ":/shaders/shaders/fflat.glsl",
//                                    ":/shaders/shaders/fgouraud.glsl",
//                                    ":/shaders/shaders/fphong.glsl",
//                                    ":/shaders/shaders/fnormal.glsl",
//                                    ":/shaders/shaders/ftexture.glsl",
//                                    ":/shaders/shaders/ftex2.glsl",
//                                    ":/shaders/shaders/fnormalmap.glsl",
//                                    ":/shaders/shaders/fcubemap.glsl"};

    destroyShaders();

    shaderProgram.clear();

    for (int i = 0; i < numShaders; ++i)
    {
        QFile vs(vertexShaderFile[i]);
        QFile fs(fragmentShaderFile[i]);
        QFile gs(":/shaders/shaders/geometry.glsl");

        vs.open(QFile::ReadOnly | QFile::Text);
        fs.open(QFile::ReadOnly | QFile::Text);
        gs.open(QFile::ReadOnly | QFile::Text);

        QTextStream streamVs(&vs), streamFs(&fs), streamGs(&gs);

        QString qtStringVs = streamVs.readAll();
        QString qtStringFs = streamFs.readAll();
        QString qtStringGs = streamGs.readAll();

        std::string stdStringVs = qtStringVs.toStdString();
        std::string stdStringFs = qtStringFs.toStdString();
        std::string stdStringGs = qtStringGs.toStdString();

        // Create an empty vertex shader handle
        GLuint vertexShader = 0;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        const GLchar *source = stdStringVs.c_str();

        glShaderSource(vertexShader, 1, &source, 0);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
            qDebug("%s", &infoLog[0]);

            glDeleteShader(vertexShader);
            return;
        }

        // Create an empty geometry shader handle
        GLuint geometryShader = 0;
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        // Send the geometry shader source code to GL
        source = stdStringGs.c_str();
        glShaderSource(geometryShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(geometryShader);

        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(geometryShader, maxLength, &maxLength, &infoLog[0]);
            qDebug("%s", &infoLog[0]);

            glDeleteShader(geometryShader);
            glDeleteShader(vertexShader);
            return;
        }


        // Create an empty fragment shader handle
        GLuint fragmentShader = 0;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = stdStringFs.c_str();//(const GLchar *)stringFs.toStdString().c_str();
        glShaderSource(fragmentShader, 1, &source, 0);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
            qDebug("%s", &infoLog[0]);

            glDeleteShader(fragmentShader);
            glDeleteShader(vertexShader);
            return;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        GLuint shaderProgramID = 0;
        shaderProgramID = glCreateProgram();
        shaderProgram.push_back(shaderProgramID);

        // Attach our shaders to our program
        glAttachShader(shaderProgramID, vertexShader);
        glAttachShader(shaderProgramID, geometryShader);
        glAttachShader(shaderProgramID, fragmentShader);

        // Link our program
        glLinkProgram(shaderProgramID);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(shaderProgramID, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(shaderProgramID, maxLength, &maxLength, &infoLog[0]);
            qDebug("%s", &infoLog[0]);

            glDeleteProgram(shaderProgramID);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return;
        }

        glDetachShader(shaderProgramID, vertexShader);
        glDetachShader(shaderProgramID, geometryShader);
        glDetachShader(shaderProgramID, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(geometryShader);
        glDeleteShader(fragmentShader);

        vs.close();
        gs.close();
        fs.close();
    }
}

void ShaderManager::destroyShaders ()
{
    for (GLuint shaderProgramID : shaderProgram)
        glDeleteProgram(shaderProgramID);
    shaderProgram.clear();
}

GLuint ShaderManager::getShader(int index){
    if(index>=0 || index<=(numShaders-1))
        return shaderProgram[index];
    else
        return shaderProgram[0];
}
