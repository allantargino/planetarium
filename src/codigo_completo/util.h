#ifndef UTIL_H
#define UTIL_H

#include <QOpenGLExtraFunctions>

inline const char* getOpenGLErrorString(GLenum err)
{
    switch (err)
    {
    case GL_NO_ERROR: return "GL_NO_ERROR";
    case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
//    case GL_CONTEXT_LOST: return "GL_CONTEXT_LOST";
    default: return "UNKNOWN ERROR";
    }
}

#ifdef QT_DEBUG
inline void checkOpenGLError(const char* stmt, const char* function,
                             const char* file, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        qDebug() << "OpenGL error:" << getOpenGLErrorString(err)
                 << hex << err << dec << "at" << stmt
                 << "from" << function << "in file" << file
                 << "line" << line;
    }
}

#define GL_CHECK(stmt) \
do \
{ \
    stmt; \
    checkOpenGLError(#stmt, Q_FUNC_INFO, __FILE__, __LINE__); \
} while (0)
#else

#define GL_CHECK(stmt) stmt

#endif

#endif // UTIL_H
