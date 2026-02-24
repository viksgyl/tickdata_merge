#ifndef TICKFILE_H
#define TICKFILE_H
#include <sstream>
#include <queue>
#include <future> 
#include <tick.h>
#include <mmapfile.h>
#include <shared_mutex>

class File {
public:
  File() : filePosition(0), activeQ(nullptr), backupQ(nullptr) {}
  void initialize(const std::string& filename);
  bool isReady() { return false;}
protected:
  File(const File&) = delete;
  void switchQ();
  std::string fileName;
  int filePosition;
  int fileSize;
  std::string partialReadRecord;
  std::queue<Tick> queue1;
  std::queue<Tick> queue2;
  std::queue<Tick>* activeQ;
  std::queue<Tick>* backupQ;
  std::shared_mutex mutex;
  std::future<Segment::Data> segment;
};

class TickReader : File {
public:
  TickReader() {}
  void initialize(const std::string& filename);
  bool isReady();
  Tick getNext();
  bool hasNext();
private:
  TickReader(const TickReader&) = delete;
};

class TickWriter : File {
public:
  TickWriter() {}
  void initialize(const std::string& filename);
  void push(const Tick& tick);
  void write();
private:
  TickWriter(const TickWriter&) = delete;
};

#endif
