#include <tickfile.h>
#include <string>
#include <regex>

using namespace std; 

void File::initialize(const std::string& filename) {
  fileName = filename;
  fileSize = Segment::getFileSize(filename);
}

void File::switchQ() {
  unique_lock<shared_mutex> lock(mutex);
  if (segment.valid()) {
    Segment::Data data = segment.get();
    if (data.success) {
      *backupQ = data.segment;
      filePosition = data.offset;
      partialReadRecord = data.partialReadRecord;
    }
  }

  if (activeQ == &queue1) {
    activeQ = &queue2;
    backupQ = &queue1;
  }
  else if (activeQ == &queue2) {
    activeQ = &queue1;
    backupQ = &queue2;
  }

  if (filePosition < fileSize)
    segment = async(Reader(fileName, fileSize), filePosition, partialReadRecord);
}

bool TickReader::isReady(){
  switchQ();
  return true;
}

bool TickReader::hasNext() {
  if (!activeQ->empty() || filePosition < fileSize)
    return true;
  return false;    
}

Tick TickReader::getNext() {
  if (activeQ->empty())
  {
    switchQ();
  }
  Tick tick = activeQ->front();
  activeQ->pop();
  return tick;
}

void TickReader::initialize(const string& filename) 
{
  unique_lock<shared_mutex> lock(mutex);
  File::initialize(filename);
  if (activeQ == nullptr) activeQ = &queue1;
  if (backupQ == nullptr) backupQ = &queue2;
  if (filePosition < fileSize)
    segment = async(Reader(filename, fileSize), filePosition, partialReadRecord);
}


void TickWriter::initialize(const string& filename) 
{
  unique_lock<shared_mutex> lock(mutex);
  fileName = filename;
  fileSize = -1;
  if (activeQ == nullptr) activeQ = &queue1;
  if (backupQ == nullptr) backupQ = &queue2;
}

void TickWriter::push(const Tick& tick)
{
  /* we can switch the queue once the file has say 100000 records and dump to file
     Leaving it now for simplicity */
  activeQ->push(tick);
} 

void TickWriter::write() 
{
  ostringstream os;
  Tick::writeHeader(os);
  switchQ();
  while (!backupQ->empty())
  {
    Tick tick = backupQ->front();
    backupQ->pop();
    tick.writeTick(os);
  }
  segment = async(Writer(fileName, fileSize), os.str(), filePosition);
}
