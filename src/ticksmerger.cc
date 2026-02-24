#include <ticksmerger.h>
#include <sstream>
#include <boost/program_options.hpp>
#include <experimental/filesystem>
#include <iostream>
#include <multiplexer.h>
#include <ticknode.h>

using namespace std;
TicksMerger& TicksMerger::instance() {
  static TicksMerger TicksMerger;
  return TicksMerger;
}

bool TicksMerger::initialize(int argc, char** argv) {
  if (parseCmdLine(argc, argv)){
    files = getFiles(dataPath);
#ifdef DEBUG
    for (auto file: files)
      cout << file << endl;
#endif

    experimental::filesystem::create_directory(outputPath);
  }
  return true;
}

void TicksMerger::start() {
  vector<string> slice;
  for (size_t index = 0; (index < Multiplexer::k && index < files.size()); ++index)
    slice.push_back(files[index]);

  Multiplexer m;
  m.initialize(slice, outputPath + "/MultiplexedFile.txt");
  if (m.isReady())
    m.multiplex();
}

bool TicksMerger::parseCmdLine(int argc, char** argv){
  stringstream ss;
  ss << "Usage : " << argv [0] << " --data <foldername> --output <foldername> --k <k-way search number" << endl;
  namespace po = boost::program_options;
  po::options_description desc ("Allowed Options");
  desc.add_options()
  ("help", ss.str().c_str())
  ("data", po::value(&dataPath), "Data Folder Path")
  ("output", po::value(&outputPath), "Output Folder Path");

  po::positional_options_description positionals;
  positionals.add("data", 1);
  positionals.add("output", 1);
  po::variables_map vm;
  po::store(po::command_line_parser (argc, argv)
    .positional(positionals)
    .options(desc).run(), vm);
  po::notify(vm);

  if (vm.count("help") || !(vm.count ("data") && vm.count("output"))) {
    cerr << desc << endl;
    exit(1);
  }
  return true;
}

vector<string> TicksMerger::getFiles(const string& path) {
  vector<string> files;
  namespace fs = experimental::filesystem;
  for (auto entry : fs::directory_iterator(path))
  {
    if (!fs::is_directory(entry))
    {
#ifdef DEBUG
      cout << entry.path() << endl;
#endif
      files.push_back(entry.path());
    }
  }
  return files;
}
