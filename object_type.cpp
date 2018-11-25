#include "object_type.h"

ObjectType::ObjectType(int _id)
        : id(_id)
{

}

ObjectType::~ObjectType() {
  for (auto *objectAction : actions)
  {
    delete objectAction;
  }

  actions.clear();
}

void ObjectType::Print()
{
  printf("Object type Id: %d\n", id);
  printf("Total object actions: %lu\n", actions.size());
  for (auto *objectAction : actions)
  {
    objectAction->Print();
  }
}

void ObjectType::AddAction(ObjectAction *action) {
  actions.push_back(action);
}
