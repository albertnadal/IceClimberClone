#include "scene_object_factory.h"
#include <map>
#include <string>

template <class TB>
SceneObjectFactory<TB>::SceneObjectFactory()
{
	getMap();
}

template <class TB>
TB* SceneObjectFactory<TB>::createInstance(std::string const& s)
{
	typename map_type::iterator it = getMap()->find(s);
	if (it == getMap()->end())
	{
		return 0;
	}
	return it->second();
}

template <class TB>
typename SceneObjectFactory<TB>::map_type* SceneObjectFactory<TB>::getMap()  //intentionally never deleted
{
	if (!sceneObjectMap)
	{
		sceneObjectMap = new map_type;
	}
	return sceneObjectMap;
}
