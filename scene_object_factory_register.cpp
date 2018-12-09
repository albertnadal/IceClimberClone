#include <string>
#include <utility>
#include "scene_object_factory_register.h"

template<typename T, typename TB> TB * createT() { return new T; }

template<typename T, typename TB>
SceneObjectFactoryRegister<T, TB>::SceneObjectFactoryRegister(std::string const& s)
{
  this->getMap()->insert(std::make_pair<std::string, TB*(*)()>(s, &createT<T, TB>));
}
