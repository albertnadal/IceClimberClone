#include "object_action.h"

ObjectAction::ObjectAction(int _id)
        : id(_id)
{
}

ObjectAction::~ObjectAction() {
  for (auto *frame : frames)
  {
    delete frame;
  }

  frames.clear();
}

void ObjectAction::Print()
{
  printf("Object action Id: %d\n", id);
  printf("Total action frames: %lu\n", frames.size());
  for (auto *frame : frames)
  {
    frame->Print();
  }
}

void ObjectAction::AddFrame(ObjectFrame *frame)
{
  frames.push_back(frame);
}
