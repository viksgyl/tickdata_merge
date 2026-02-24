#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H
#include <string>
#include <vector>
#include <tickfile.h>

class TickNode;
class Multiplexer
{
public:
  static const int k = 7;
  Multiplexer() {}
  bool initialize(const std::vector<std::string>& inputFiles, const std::string& outputFile);
  void multiplex();
  bool isReady();
private:
  Multiplexer(const Multiplexer&)               = delete;
  Multiplexer(Multiplexer&&) noexcept           = delete;
  Multiplexer& operator = (const Multiplexer&)  = delete;
  Multiplexer& operator = (Multiplexer&&)       = delete;
  bool tickNode(int fIndex, TickNode& tn);
private:
  TickReader tickFiles[k];
  TickWriter writer;
  int index = 0;
};

#endif
