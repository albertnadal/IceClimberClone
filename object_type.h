#pragma once
#include <vector>
#include <object_action.h>

class ObjectType
{
        int id;
        std::vector<ObjectAction*> actions;
public:
        ObjectType(int id);
        ~ObjectType();
        void AddAction(ObjectAction* action);
        void Print();
};
