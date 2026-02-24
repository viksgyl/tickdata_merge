#include <tick.h>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace std::chrono;

bool Tick::loadTick(const string& tick, const string& symbol){
  vector<string> result;
  boost::split(result, tick, boost::is_any_of(","));
  this->symbol = symbol;
  int index = 0;
  if (symbol.empty())
    this->symbol= result[index++];

  bool retVal = false;
  if (result.size() >=5)
  {
    readTicktime(result[index++]);
    price     = stod(result[index++]);
    size      = stol(result[index++], nullptr, 10);
    exchange  = boost::algorithm::trim_left_copy(result[index++]);
    type      = boost::algorithm::trim_left_copy(result[index++]);
    retVal    = true;
  }
  return retVal;
}

void Tick::writeHeader(ostringstream& os)
{
  os << "Symbol, Timestamp, Price, Size, Exchange, Type" << endl;
}

void Tick::writeTick(ostringstream& os) {
  os << symbol << ", ";
  writeTicktime(os);
  os << ", " << setprecision(2) << price << ", " << size << ", " << exchange << ", " << type << endl;
}

void Tick::writeTicktime(ostringstream& os) {
  auto seconds = double(tickTime.time_since_epoch().count()) * 
  high_resolution_clock::period::num / high_resolution_clock::period::den;
  auto const millis = modf(seconds, &seconds); 
  time_t time(seconds);
  auto const tm = localtime(&time);
  os << put_time(tm, "%Y-%m-%d %H:%M:") << setw(6)
  << setfill('0') << fixed << setprecision(3) << millis;
}

void Tick::readTicktime(const string& strTime) {
  stringstream ss{strTime};
  tm tm{};
  ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
  tickTime =  system_clock::from_time_t(mktime(&tm));
  double ms = 0.0;
  if (ss.peek() != '.' || !(ss >> ms))
    throw invalid_argument("decimal");
  size_t msec = ms * high_resolution_clock::period::den / high_resolution_clock::period::num;
  tickTime += high_resolution_clock::duration(msec);
}
