#include <tick.h>
#include <ticknode.h>
#include <ticksmerger.h>

int main(int argc, char** argv)
{
  TicksMerger &ticksMerger = TicksMerger::instance();
  ticksMerger.initialize(argc, argv);
  ticksMerger.start();

  return 0;
}
