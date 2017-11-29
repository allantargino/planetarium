#include "trajectory.h"

Trajectory::Trajectory(QOpenGLWidget* _glWidget){
    glWidget = _glWidget;

    qDebug("Starting Trajectory");

}

Trajectory::~Trajectory(){
    destroyVBOs();
    destroyShaders();
}

void Trajectory::startTrajectory(){
    initializeOpenGLFunctions();

    createShaders();
    createVBOs();
}

void Trajectory::drawModel(){
    glUseProgram (shaderProgram);
    glBindVertexArray(vao);

    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    glDrawArrays(GL_POINTS, 0, 3);
}


void Trajectory::createShaders()
{
    glWidget->makeCurrent ();
    destroyShaders ();
    QString vertexShaderFile (":/shaders/shaders/vshader1.glsl");
    QString fragmentShaderFile (":/shaders/shaders/fshader1.glsl");
    QString geometryShaderFile (":/shaders/shaders/geometry_circle.glsl");

    QFile vs( vertexShaderFile );
    QFile fs( fragmentShaderFile );
    QFile gs( geometryShaderFile );

    vs. open ( QFile :: ReadOnly | QFile :: Text );
    fs. open ( QFile :: ReadOnly | QFile :: Text );
    gs. open ( QFile :: ReadOnly | QFile :: Text );

    QTextStream streamVs (& vs), streamFs (& fs), streamGs (& gs);

    QString qtStringVs = streamVs . readAll ();
    QString qtStringFs = streamFs . readAll ();
    QString qtStringGs = streamGs . readAll ();

    std :: string stdStringVs = qtStringVs . toStdString ();
    std :: string stdStringFs = qtStringFs . toStdString ();
    std :: string stdStringGs = qtStringGs . toStdString ();

    // Create an empty vertex shader handle
    GLuint vertexShader = 0;
    vertexShader = glCreateShader( GL_VERTEX_SHADER );

    // Send the vertex shader source code to GL
    const GLchar * source = stdStringVs . c_str ();
    glShaderSource( vertexShader , 1, &source , 0);

    // Compile the vertex shader
    glCompileShader( vertexShader );
    GLint isCompiled = 0;
    glGetShaderiv( vertexShader , GL_COMPILE_STATUS , & isCompiled );
    if (isCompiled==GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv ( vertexShader , GL_INFO_LOG_LENGTH , &
        maxLength );
        // The maxLength includes the NULL character
        std :: vector <GLchar > infoLog ( maxLength );
        glGetShaderInfoLog( vertexShader , maxLength , & maxLength , &
        infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteShader( vertexShader );
        return ;
    }



    // Create an empty geometry shader handle
    GLuint geometryShader = 0;
    geometryShader = glCreateShader ( GL_GEOMETRY_SHADER );
    // Send the fragment shader source code to GL
    // Note that std :: string ’s . c_str is NULL character terminated .
    source = stdStringGs . c_str ();
    glShaderSource ( geometryShader , 1, &source , 0);
    // Compile the fragment shader
    glCompileShader ( geometryShader );
    glGetShaderiv ( geometryShader , GL_COMPILE_STATUS , & isCompiled );
    if (isCompiled==GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv ( geometryShader , GL_INFO_LOG_LENGTH , &
        maxLength );
        std :: vector <GLchar > infoLog ( maxLength );
        glGetShaderInfoLog ( geometryShader , maxLength , & maxLength ,
        & infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteShader ( geometryShader );
        glDeleteShader ( vertexShader );
        return ;
    }




    // Create an empty fragment shader handle
    GLuint fragmentShader = 0;
    fragmentShader = glCreateShader ( GL_FRAGMENT_SHADER );
    // Send the fragment shader source code to GL
    // Note that std :: string ’s . c_str is NULL character terminated .
    source = stdStringFs . c_str ();
    glShaderSource ( fragmentShader , 1, &source , 0);
    // Compile the fragment shader
    glCompileShader ( fragmentShader );
    glGetShaderiv ( fragmentShader , GL_COMPILE_STATUS , & isCompiled );
    if (isCompiled==GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv ( fragmentShader , GL_INFO_LOG_LENGTH , &
        maxLength );
        std :: vector <GLchar > infoLog ( maxLength );
        glGetShaderInfoLog ( fragmentShader , maxLength , & maxLength ,
        & infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteShader ( fragmentShader );
        glDeleteShader ( vertexShader );
        return ;
    }

    shaderProgram = glCreateProgram();
    // Attach our shaders to our program
    glAttachShader( shaderProgram , vertexShader );
    glAttachShader( shaderProgram , geometryShader );
    glAttachShader ( shaderProgram , fragmentShader );
    // Link our program
    glLinkProgram( shaderProgram );
    // Note the different functions here : glGetProgram * instead of glGetShader *.
    GLint isLinked = 0;
    glGetProgramiv( shaderProgram , GL_LINK_STATUS , ( int *)& isLinked );
    if ( isLinked == GL_FALSE )
    {
        GLint maxLength = 0;
        glGetProgramiv ( shaderProgram , GL_INFO_LOG_LENGTH , &
        maxLength );
        // The maxLength includes the NULL character
        std :: vector <GLchar > infoLog ( maxLength );
        glGetProgramInfoLog ( shaderProgram , maxLength , & maxLength ,
        & infoLog [0]) ;
        qDebug ("%s", & infoLog [0]) ;
        glDeleteProgram ( shaderProgram );
        glDeleteShader ( vertexShader );
        glDeleteShader ( fragmentShader );
        return ;
    }

    glDetachShader( shaderProgram , vertexShader );
    glDetachShader( shaderProgram , geometryShader );
    glDetachShader ( shaderProgram , fragmentShader );
    glDeleteShader( vertexShader );
    glDeleteShader ( geometryShader );
    glDeleteShader ( fragmentShader );
    vs.close ();
    gs.close ();
    fs.close ();
}

void Trajectory::destroyShaders(){
    glWidget->makeCurrent ();
    glDeleteProgram(shaderProgram);
}

void Trajectory::createVBOs(){
    glWidget->makeCurrent ();
    destroyVBOs ();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position data
    const GLfloat pointPos[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f,  -0.5f, 0.0f,
        0.0f,   0.5f, 0.0f
    };

    // Color data
    const GLfloat pointCol[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    // Radius data
    const GLfloat pointRad[] = {
        0.3f, 0.5f, 0.3f
    };

    glGenBuffers(3, vbo);

    // VBO for position data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointPos), pointPos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    // VBO for color data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointCol), pointCol, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // VBO for radius data
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointRad), pointRad, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);
}

void Trajectory::destroyVBOs()
{
    glWidget->makeCurrent ();

    glDeleteVertexArrays (1, &vao);

    int sizeVBOs = sizeof(vbo) / sizeof(float);
    for (int i = 0; i < sizeVBOs; ++i)
        vbo[i] = 0;

    vao = 0;
}

