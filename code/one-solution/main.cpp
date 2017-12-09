#include <boost/program_options.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <fstream>
namespace po = boost::program_options;
typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;
using namespace std;
class OneSolutionComputer {
public:
  static vector<int>* compute(int n) {
    vector<int>* rsl = new vector<int>(n, -1);
    int x = 0;int y = 0;
    if (n%2 == 0) {
      // n is even
      if ((n-2)%6!=0) {
	// not 6k+2 form
	for (int i = 1; i <= n/2; i++) {
	  // first coordinate
	  x = i;
	  y = 2*i;
	  rsl->at(x-1) = y;
	  // second coordinate
	  x = n/2+i;
	  y = 2*i-1;
	  rsl->at(x-1) = y;
	}
	return rsl;
      } else if (n%6!=0) {
	// not 6k form
	for (int i = 1; i <= n/2; i++) {
	  // first coordinate
	  x = i;
	  y = 1 + (2*(i-1)+n/2-1)%n;
	  rsl->at(x-1) = y;
	  // second coordinate
	  x = n+1-i;
	  y = n - (2*(i-1)+n/2-1)%n;
	  rsl->at(x-1) = y;
	}
	return rsl;
      } else {
	cout << n << ", Special case! Should never be here" << endl;
	return NULL;
      }
    } else {
      // n is odd
      int n2 = n - 1;
      if ((n2-2)%6!=0) {
	// not 6k+2 form
	for (int i = 1; i <= n2/2; i++) {
	  // first coordinate
	  x = i;
	  y = 2*i;
	  rsl->at(x-1) = y;
	  // second coordinate
	  x = n2/2+i;
	  y = 2*i-1;
	  rsl->at(x-1) = y;
	}
	rsl->at(n2) = n;
	return rsl;
      } else if (n2%6!=0) {
	// not 6k form
	for (int i = 1; i <= n2/2; i++) {
	  // first coordinate
	  x = i;
	  y = 1 + (2*(i-1)+n2/2-1)%n2;
	  rsl->at(x-1) = y;
	  // second coordinate
	  x = n2+1-i;
	  y = n2 - (2*(i-1)+n2/2-1)%n2;
	  rsl->at(x-1) = y;
	}
	rsl->at(n2) = n;
	return rsl;
      }
    }
  }
};



int main(int ac, char* av[]) {
  int maxN = 500000; // number of queens
  bool printSolutions = true;
  bool eval = false;
  try {
    po::options_description desc("Opions of n-queens solver");
    desc.add_options()
      ("help", "print all the available options")
      ("maxN", po::value<int>(), "maximum number of queens (default is 10 Million)")
      ("print", "print the solutions (default is true)")
      ("eval", "evaluate the running time for generating graph, without outputing the solutions (default is false)")
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
      std::cout << "maxium number of queens: " << maxN << "." << std::endl;
    }  else {
      std::cout << "[Warning] No maxium number of queens specified. Use 500 thouthands." << std::endl;
    }
    if (vm.count("eval")) {
      std::cout << "Evaluatiion model." << std::endl;
      eval = true;
    }
  } catch (std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
    return -1;
  } catch (...) {
    std::cerr << "Unknown exception!" << std::endl;
    return -1;
  }

  if ( eval) {
    ofstream evalFile;
    evalFile.open ("eval.txt");
    for (int i = 4; i <= maxN; i++) {
      OneSolutionComputer o;
      
      Time t1(boost::posix_time::microsec_clock::local_time());
      vector<int>* rsl = o.compute(i); // compute a solution
      Time t2(boost::posix_time::microsec_clock::local_time());
      TimeDuration dt = t2 - t1;
      //number of elapsed miliseconds
      long msec = dt.total_nanoseconds();
 
      //print elapsed seconds (with millisecond precision)
      evalFile << i <<  ", " <<  msec << std::endl;

      // sleep for a while to avoid killed by kernel
      if (i % 100000 == 0)
	usleep(1000);
    }
    evalFile.close();
    return 0;
  }

  
  if (printSolutions) {

    return 0;
  }
  return 0;
}
