#include <multiplexer.h>
#include <ticknode.h>
#include<queue>
#include <iostream>

using namespace std;
const int Multiplexer::k;

bool Multiplexer::initialize(const vector<std::string>& inputFiles, const string& outputFile)
{
  index = 0;
  for (auto file: inputFiles)
    tickFiles[index++].initialize(file);
  writer.initialize(outputFile);
  return true;
}

bool Multiplexer::isReady() {
  for (auto fIndex = 0; fIndex < index; ++fIndex)
    tickFiles[fIndex].isReady();
  return true;
}

bool Multiplexer::tickNode(int fIndex, TickNode& tn)
{
  bool retVal = false;
  if (tickFiles[fIndex].hasNext()) {
    Tick tick = tickFiles[fIndex].getNext();
    ostringstream ss;
    tick.writeTick(ss);
    tn = TickNode(tick, fIndex);
    retVal = true;
  }
  return retVal;
}

void Multiplexer::multiplex()
{
  priority_queue<TickNode, vector<TickNode>, TickNode::CompareTickNodes> pq;
  TickNode node;
  
  for (auto fIndex = 0; fIndex < index ; ++fIndex) {
    if (!tickNode(fIndex, node))
      continue;
    pq.push(node);
  }

  while(!pq.empty()) {
    TickNode tn = pq.top();
    pq.pop();
    int queueIndex = tn.tickIndex();
    writer.push(tn.getTick());
    if (tickNode(queueIndex, node))
      pq.push(node);
  }

  writer.write();
}
