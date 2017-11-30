#ifndef LIGHT_H
#define LIGHT_H

#include <QVector4D>

class Light
{
public :
    Light();

    QVector4D position = QVector4D (15, 15, 15, 0);
    QVector4D ambient  = QVector4D (1, 1, 1, 1);
    QVector4D diffuse  = QVector4D (1, 1, 1, 1);
    QVector4D specular = QVector4D (1, 1, 1, 1);
};

#endif // LIGHT_H
