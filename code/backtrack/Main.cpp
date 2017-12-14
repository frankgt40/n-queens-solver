// Author: Cheng Cai
// Email: ccai6@uci.edu

//#include "n-queens-ex.h"
#include <fstream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include "backtrack-bitmask.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;
typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;
using namespace std;
#include <iostream>
#include <iterator>

int main(int ac, char* av[]) {
  int maxN = 17; // number of queens
  bool printSolutions = false;
  try {
    po::options_description desc("Opions of n-queens solver");
    desc.add_options()
      ("help", "print all the available options")
      ("maxN", po::value<int>(), "maximum number of queens (default is 17)")
      ("print", "print the solutions (default is without printing solutions)")
      ;

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 0;
    }
    if (vm.count("print")) {
      printSolutions = true;
    }
    if (vm.count("maxN")) {
      maxN = vm["maxN"].as<int>();
      std::cout << "maximum number of queens: " << maxN << "." << std::endl;
    }  else {
      std::cout << "You have to give the number of queens!" << std::endl;
      return -1;
    }
  } catch (std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return -1;
  } catch (...) {
    std::cerr << "Unknown exception!" << std::endl;
    return -1;
  }

  ofstream evalFile;
  evalFile.open("evalCSP.txt");
  Solution s;
  for (int i = 4; i <= maxN; i++) {
    Time t1(boost::posix_time::microsec_clock::local_time());
    std::vector<std::vector<std::string> > matrix = s.solveNQueens(i);
    Time t2(boost::posix_time::microsec_clock::local_time());
    TimeDuration dt = t2 - t1;
    //number of elapsed miliseconds
    long msec = dt.total_nanoseconds();
    //print elapsed seconds (with millisecond precision)
    evalFile << i <<  ", " <<  msec << endl;
    if (printSolutions) {
      for (std::vector<std::vector<std::string> >::iterator itr = matrix.begin(); itr != matrix.end(); itr++) {
	std::cout << std::endl  << "One solution: " << std::endl;
	for (std::vector<std::string>::iterator itr2 = itr->begin(); itr2 != itr->end(); itr2++) {
	  std::cout << *itr2 << "\n";
	}
      }
      std::cout << std::endl << "Number of solutions: " << matrix.size() << std::endl;
    }

  }

  evalFile.close();

  return 0;
}
