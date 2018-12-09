#ifndef SCENE_OBJECT_FACTORY_REGISTER_H
#define SCENE_OBJECT_FACTORY_REGISTER_H

#include <string>
#include <utility>
#include "scene_object_factory.h"

template<typename T, typename TB> TB * createT();

template<typename T, typename TB>
class SceneObjectFactoryRegister : SceneObjectFactory<TB>
{
    SceneObjectFactoryRegister(std::string const& s);
};

#endif
