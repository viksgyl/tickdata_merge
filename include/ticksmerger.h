#ifndef TICKSMERGER_H
#define TICKSMERGER_H
#include <string>
#include <vector>

class TicksMerger
{
public:
  static TicksMerger& instance();
  bool initialize(int argc, char** argv);
  void start();
private:
  TicksMerger() {}
  TicksMerger(const TicksMerger&)               = delete;
  TicksMerger(TicksMerger&&) noexcept           = delete;
  TicksMerger& operator = (const TicksMerger&)  = delete;
  TicksMerger& operator = (TicksMerger&&)       = delete;

  bool parseCmdLine(int argc, char** argv);
  std::vector<std::string> getFiles(const std::string& path);
private:
  std::string outputPath;
  std::string dataPath;
  std::vector<std::string> files;
};

#endif
