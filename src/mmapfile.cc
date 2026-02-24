#include <mmapfile.h>
#include <tick.h>
#include <sys/stat.h> 
#include <sys/mman.h>
#include <fcntl.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <regex>

using namespace std;

int Segment::getFileSize(const std::string& filename) 
{
  int size = -1;
  try 
  {
    int fd = open (filename.c_str(), O_RDONLY, S_IRUSR| S_IWUSR);
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
      cout << "Error getting file size : " << filename << endl;
    }
    size = sb.st_size;
  }
  catch (ios_base::failure &e) {
    cerr << "Exception opening/reading file : " << e.what() << endl;
  }
  catch (...) {
    cerr << "Exception reading file" << endl;
  }  
  return size;
}

bool Reader::readSegment(int filePosition, string& segment, int &segmentLength)
{
  bool retVal = false;
  try {
    int fd = open (fileName.c_str(), O_RDONLY, S_IRUSR| S_IWUSR);
    int sz = BLOCK;
    if ((sz + filePosition) > fileSize)
      sz = fileSize - filePosition;
    char* data = (char*) mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, filePosition);
    segment = string(data, sz);
    munmap(data, sz);
    segmentLength = sz;
    close(fd);
    retVal = true;
  }
  catch (ios_base::failure &e) {
    cerr << "Exception opening/reading file : " << e.what() << endl;
  }
  catch (...) {
    cerr << "Exception " << endl;
  }  
  return retVal;
}

string Reader::symbol() 
{
  regex symbolExpr {".*/([A-Z]+).*"};
  smatch sm;
  if (regex_match(fileName, sm, symbolExpr)){
    productSymbol = sm[1];
  }
  return productSymbol;
}

Segment::Data Reader::operator () (int filePosition, const string& partialReadRecord)
{
  int segmentLength = 0;
  string segment;
  Data data;
  if (readSegment(filePosition, segment, segmentLength)) 
  {
    int index               = lastPosition(segment, segmentLength);
    data.partialReadRecord  = segment.substr(index + 1);
    data.offset             = filePosition + segmentLength;
    data.segment            =  fillFromSegment(segment, partialReadRecord);
    data.success            = true;
  }
  return data;
}

queue<Tick> Reader::fillFromSegment(const string& segment, const string& partialReadRecord)
{    
  stringstream sstr(segment);
  string prefix = "Timestamp,";
  int indx = 0;
  queue<Tick> dataq;
  string tickStr;
  while (getline(sstr, tickStr, delimeter))
  {
    if (indx == 0 && tickStr.substr(0, prefix.size()) == prefix)
      continue;
    if (indx == 0)  tickStr = partialReadRecord + tickStr;
    indx++;
    Tick t;
    if (t.loadTick(tickStr, productSymbol))
      dataq.push(t);
  }
#ifdef DEBUG
  cout << "Read records : " << dataq.size() << " " << indx << endl;
#endif
  return dataq;
}
int Reader::lastPosition(const string& str, int len)
{
  int idx = len - 1;
  while (str[idx--] != delimeter);
  return idx+1;
}

Segment::Data Writer::operator () (const string& str, int filePosition)
{
  int offset = 0;
  Data writeData;

  try 
  {
    int fd = open (fileName.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    int sz = BLOCK;
    int length = str.length();
#ifdef DEBUG
    cout << str;
#endif
    posix_fallocate(fd, filePosition, length);
    int alignment = filePosition % 4096;
    if (sz > length)
      sz = length;
    while (sz) {
      char* data = (char*) mmap(NULL, sz, PROT_READ| PROT_WRITE, MAP_SHARED, fd, filePosition + offset - alignment);
      if (data == MAP_FAILED)
        exit(1);
      memcpy(data+alignment, str.c_str()+offset, sz-alignment);
      msync(data, sz, MS_SYNC);
      munmap(data, sz);
      offset += sz-alignment;
      alignment = 0;
      if (length > offset) {
        sz = length - offset;
      }
      else
        sz = 0;
    }
    close(fd);
    writeData.offset   = filePosition + offset;
    writeData.success  = true;
  }
  catch (ios_base::failure &e) {
    cerr << "Exception opening/reading file : " << e.what() << endl;
    writeData.success = false;
  }
  catch (...) {
    cerr << "Exception " << endl;
    writeData.success = false;
  }  
  return writeData;
}
