#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <map>
#include <optional>
#include <AABB/AABB.h>
#include <entity.h>
#include <entity_data_manager.h>
#include <entities/player.h>
#include <entities/brick.h>
#include <entities/side_wall.h>
#include <entities/cloud.h>
#include <entities/topi.h>

class EntityFactory
{
private:
  EntityFactory(EntityDataManager*, aabb::Tree<IEntity*>*);
  EntityFactory &operator=(const EntityFactory &);
  void RegisterEntitys();
  typedef map<EntityIdentificator, CreateEntityFn> FactoryMap;
  FactoryMap m_FactoryMap;
  EntityDataManager *textureManager = nullptr;
  aabb::Tree<IEntity*> *spacePartitionObjectsTree = nullptr;
public:
	~EntityFactory();
	static EntityFactory *Get(EntityDataManager*, aabb::Tree<IEntity*>*);
	void Register(const EntityIdentificator, CreateEntityFn);
	std::optional<IEntity*> CreateEntity(const EntityIdentificator);
};

#endif
