#ifndef TICK_H
#define TICK_H
#include <string>
#include <chrono>
#include <sstream>

class Tick {
  public:
    class CompareTicks {
      public:
        bool operator () (const Tick& left, const Tick& right) {
          if (left.tickTime > right.tickTime)
            return true;
          else if (left.tickTime == right.tickTime && left.symbol > right.symbol)
            return true;
          else if (left.tickTime == right.tickTime && left.symbol == right.symbol
            && left.exchange > right.exchange)
            return true;
          else if (left.tickTime == right.tickTime && left.symbol == right.symbol
            && left.exchange == right.exchange && left.type > right.type)
            return true;
          return false;
        }
    };

    Tick() {}
    bool loadTick(const std::string& tick, const std::string& symbol = "");
    static void writeHeader(std::ostringstream& os);
    void writeTick(std::ostringstream& os);

  private:
    void writeTicktime(std::ostringstream& os);
    void readTicktime(const std::string& strTime);

    std::chrono::time_point<std::chrono::system_clock> tickTime;
    double price = 0.0;
    long size = 0;
    std::string exchange = "";
    std::string type = "";
    std::string symbol = "";
};

#endif
