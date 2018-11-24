#pragma once
#include <vector>
#include <object_action.h>

class ObjectType
{
        int id;
        std::vector<ObjectAction> actions;
public:
        ObjectType(int id, std::vector<ObjectAction> actions);
};
