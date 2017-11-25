#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include "astronomicalobject.h"

class ModelFactory
{
public :
    ModelFactory();

private:
    Planet LoadSun();
};

#endif // MODELFACTORY_H
