#pragma once
#include <vector>
#include <types.h>
#include <object_frame.h>

class ObjectAction
{
        int id;
        std::vector<ObjectFrame> frames;
public:
        ObjectAction(int id);
        void AddFrame(ObjectFrame frame);
};
