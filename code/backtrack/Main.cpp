// Author: Cheng Cai
// Email: ccai6@uci.edu

//#include "n-queens-ex.h"
#include "backtrack-bitmask.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>

int main(int ac, char* av[]) {
  int n = 0; // number of queens
  bool printSolutions = false;
  try {
    po::options_description desc("Opions of n-queens solver");
    desc.add_options()
      ("help", "print all the available options")
      ("n", po::value<int>(), "number of queens")
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
    if (vm.count("n")) {
      n = vm["n"].as<int>();
      std::cout << "Number of queens: " << n << "." << std::endl;
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
  Solution s;
  
  std::vector<std::vector<std::string> > matrix = s.solveNQueens(n);
  if (printSolutions) {
    for (std::vector<std::vector<std::string> >::iterator itr = matrix.begin(); itr != matrix.end(); itr++) {
      std::cout << std::endl  << "One solution: " << std::endl;
      for (std::vector<std::string>::iterator itr2 = itr->begin(); itr2 != itr->end(); itr2++) {
	std::cout << *itr2 << "\n";
      }
    }
  }
  std::cout << std::endl << "Number of solutions: " << matrix.size() << std::endl;
  return 0;
}
