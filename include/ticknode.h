#ifndef TICKNODE_H
#define TICKNODE_H

#include <tick.h>

class TickNode {
public:
  TickNode() {}
  TickNode(const Tick& t, int index) : tick(t), index(index) {}
  Tick getTick() {
    return tick;
  }

  int tickIndex() {
    return index;
  }

  class CompareTickNodes {
    public:
      bool operator () (const TickNode& left, const TickNode& right)
      {
        return Tick::CompareTicks()(left.tick, right.tick);
      }
  };


private:
  Tick tick;
  int index;
};

#endif
