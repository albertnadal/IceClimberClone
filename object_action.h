#ifndef OBJECT_ACTION_H
#define OBJECT_ACTION_H

#include <vector>
#include <types.h>
#include <object_frame.h>

class ObjectAction
{
        int id;
        std::vector<ObjectFrame*> frames;
public:
        ObjectAction(int id);
        ~ObjectAction();
        void AddFrame(ObjectFrame* frame);
        void Print();
};
#endif
