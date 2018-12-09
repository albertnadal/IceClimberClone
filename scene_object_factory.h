#ifndef SCENE_OBJECT_FACTORY_H
#define SCENE_OBJECT_FACTORY_H

#include <map>
#include <string>

template <typename TB>
class SceneObjectFactory
{
public:
  typedef std::map<std::string, TB*(*)()> map_type;
	static map_type* sceneObjectMap;

	SceneObjectFactory();
	static TB* createInstance(std::string const& s);
protected:
	static map_type* getMap();
};

//SceneObjectFactory<TB>::map_type* sceneObjectMap;

#endif
