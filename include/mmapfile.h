#ifndef FILE_H
#define FILE_H
#include <sstream>
#include <queue>

class Tick;

class Segment {
public:
  struct Data {
    bool success = false;
    int offset = 0;
    std::queue<Tick> segment;
    std::string partialReadRecord;
  };

  Segment(const std::string& filename, int filesize) : fileName(filename), fileSize(filesize) {}
  static int getFileSize(const std::string& filename);

protected:
  std::string fileName;
  std::string productSymbol;
  int fileSize = 0;
  const int PAGE_SIZE = 4096;

  const int BLOCK = 1024 * PAGE_SIZE; // read 4mb blocks
  const char delimeter = '\n';
};

class Reader : Segment {
public:
  Reader(const std::string& filename, int filesize) : Segment (filename, filesize) { symbol(); }
  Data operator () (int position, const std::string& partialReadRecord = "");
private:
  std::string symbol ();
  int lastPosition(const std::string& data, int dataLen);
  bool readSegment(int filePosition, std::string& segment, int &segmentLength);
  std::queue<Tick> fillFromSegment(const std::string& segment, const std::string& partialReadRecord);
};

class Writer : Segment {
public:
  Writer(const std::string& filename, int filesize) : Segment (filename, filesize) {}
  Data operator () (const std::string& str, int filePosition = 0);
};

#endif
