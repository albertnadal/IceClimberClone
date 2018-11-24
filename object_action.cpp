#include "object_action.h"

ObjectAction::ObjectAction(int _id);
        : id(_id)
{
}

ObjectAction::AddFrame(ObjectFrame frame)
{
  frames.Add(frame);
}
