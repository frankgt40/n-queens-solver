#include <boost/program_options.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
namespace po = boost::program_options;
typedef boost::posix_time::ptime Time;
typedef boost::posix_time::time_duration TimeDuration;
using namespace std;
class Header;

class Node {
protected:
  Node* up;
  Node* down;
  Node* left;
  Node* right;
public:
  Node(): up(this), down(this), left(this), right(this) {
  }
  Node* u() {
    return up;
  }
  Node* d() {
    return down;
  }
  Node* r() {
    return right;
  }
  Node* l() {
    return left;
  }
  void setL(Node* l) {
    left = l;
  }
  void setR(Node* r) {
    right = r;
  }
  void setU(Node* u) {
    up = u;
  }
  void setD(Node* d) {
    down = d;
  }
  virtual bool isRoot() {
    return false;
  }
  virtual bool isHeader() {
    return false;
  }
  virtual Header* getC() {
    return NULL;
  }

};

class Header : public Node {
private:
  int _size;
  string description;
public :
  bool optional;
  int id;
  bool isRank;
  Header() : _size(0), description(""), id(-1), isRank(false) {
  }
  int size() {
    return this->_size;
  }
  string dscpt() {
    return description;
  }
  void setSize(int s) {
    if (_size == 0) {
      this->r()->setL(this->l());
      this->l()->setR(this->r());
    }
    _size = s;
  }
  void setDescription(string s) {
    description = s;
  }
  int increase() {
    _size++;
    return _size;
  }
  int decrease() {
    _size--;
    return _size;
  }
  bool isRoot() {
    return false;
  }
  bool isHeader() {
    return true;
  }
  
};

class Element : public Node {
private:
  Header* column;
public:
  Element(Header* h=NULL): column(h) {
  }
  Header* getC() {
    return column;
  }
  void setC(Header* c) {
    column = c;
  }
  bool isRoot() {
    return false;
  }
  bool isHeader() {
    return false;
  }
  
};

class Root : public Header {
public:
  Root() {
    
  }
  bool isRoot() {
    return true;
  }
  bool isHeader() {
    return false;
  }

};

class Matrix {
private:
  Root* root;
  vector<int*>* rsl;
  vector<Node*>* O;
  int n;
public:
  Matrix() {}
  Root* getRoot() {
    return root;
  }
  void genForNQueens(int n) {
    rsl = new vector<int*>();
    O = new vector<Node*>(n, NULL);
    this->n = n;
    
    root = new Root();
    Node* curr = NULL;
    curr = root;
    //// First, generating Headers
    // for n ranks
    Header* header;
    for (int i = 1; i <= n; i++) {
      header = new Header();
      header->setDescription("rank-"+to_string(i));
      header->optional = false;
      header->id = i;
      header->isRank = true;
      curr->setR(header);
      header->setL(curr);
      curr = header;
    }
    // for n files
    for (int i = 1; i <= n; i++) {
      header = new Header();
      header->setDescription("file-"+to_string(i));
      header->optional = false;
      header->id = i;
      header->isRank = false;
      curr->setR(header);
      header->setL(curr);
      curr = header;
    }
    // for represent diagonals
    for (int i =  1; i <= 2*n-1; i++) {
      header = new Header();
      header->setDescription("pDiag-"+to_string(i));
      header->optional = true;      
      curr->setR(header);
      header->setL(curr);
      curr = header;
    }
    // for reverse diagonals
    for (int i =  1; i <= 2*n-1; i++) {
      header = new Header();
      header->setDescription("rDiag-"+to_string(i));
      header->optional = true;            
      curr->setR(header);
      header->setL(curr);
      curr = header;
    }
    curr->setR(root);
    root->setL(curr);

    //// Generate the rows, there are N^2 rows
    // Every position on the N-Queen board will have four elements: rank, file, present diagonal, and reverse diagonal
    Element* rank;
    Element* file;
    Element* pDiag;
    Element* rDiag;
    for (int i = 1; i <= n; i++) {      // ranks
      for (int j = 1; j <= n; j++) {	// files
	Header* c = getHeader("rank-"+to_string(i)); 
	// for rank
	rank = new Element();
	rank->setC(c);
		
	rank->setU(c->u());
	rank->setD(c);
	rank->u()->setD(rank);
	rank->d()->setU(rank);
	c->increase();
	// for file
	c = getHeader("file-"+to_string(j));
	file = new Element();
	file->setC(c);
	
	file->setU(c->u());
	file->setD(c);
	file->u()->setD(file);
	file->d()->setU(file);
	c->increase();
	// for pDiagonal
	c = getHeader("pDiag-"+to_string(i+j-1));
	pDiag = new Element();
	pDiag->setC(c);

	pDiag->setU(c->u());
	pDiag->setD(c);
	pDiag->u()->setD(pDiag);
	pDiag->d()->setU(pDiag);
	c->increase();
	// for rDiagonal
	c = getHeader("rDiag-"+to_string(n-i+j));
	rDiag = new Element();
	rDiag->setC(c);

	rDiag->setU(c->u());
	rDiag->setD(c);
	rDiag->u()->setD(rDiag);
	rDiag->d()->setU(rDiag);
	c->increase();

	// connect them to form a row
	rank->setR(file);
	rank->setL(rDiag);
	file->setR(pDiag);
	file->setL(rank);
	pDiag->setR(rDiag);
	pDiag->setL(file);
	rDiag->setR(rank);
	rDiag->setL(pDiag);
      }
    }
    manuOptionalColumns();
  }
  void manuOptionalColumns() {
    int i = 0;
    Header* lastOne = NULL;
    Header* optional = NULL;
    for (Header* h = (Header*)root->r(); h != root;  i++) {
      if (i == 2*n-1) {
	//	cout << endl << "Last one: " << h->dscpt() << endl;
	lastOne = h;
      }
      if (i > 2*n-1) {
	optional = h;
      }
      h = (Header*)h->r();
      if (i > 2*n-1) {
	optional->setR(optional);
	optional->setL(optional);
      }
    }
    root->setL(lastOne);
    lastOne->setR(root);
  }
  
  void testPrint() {
    Node* currH = (Node*)root->r();
    while(currH != root) {
      // print the column
      Header* h = (Header*)currH;
      std::cout << h->dscpt() << " [size=" << h->size()<<"] : ";
      // print the elements of this column
      for (Node* elm = h->d(); elm != h; elm = elm->d()) {
	Element* e = (Element*) elm;
	std::cout << "1,";
      }
      std::cout << std::endl;
      
      // next column
      currH = currH->r();
    }
  }
  void testPrint(int i, int size) {
    for (int j = 1; j < i; j++) {
      std::cout << 0;
    }
    std::cout << 1;
    for (int j = i+1; j <= size; j++) {
      std::cout << 0;
    }
  }
  Header* getHeader(string description) {
    if (root == NULL) {
      return NULL;
    }
    Node* rsl = NULL;
    Node* curr = NULL;
    curr = root->r();
    while (!curr->isRoot()) {
      Header* h = (Header*)curr;
      if (h->dscpt()==description) {
	rsl = curr;
      }
      curr = curr->r();
    }
    return (Header*)rsl;
  }
  inline void cover(Header* c) {
    c->r()->setL(c->l()); c->l()->setR(c->r());
    for (Node* i = c->d(); i != c; i = i->d()) {
      for (Node* j = i->r(); i != j; j = j->r()) {
	j->d()->setU(j->u()); j->u()->setD(j->d());
	j->getC()->decrease();
      }
    }
  }
  inline void uncover(Header* c) {
    for (Node* i = c->u(); i != c; i = i->u()) {
      for (Node* j = i->l(); j != i; j = j->l()) {
	j->d()->setU(j); j->u()->setD(j);
	j->getC()->increase();
      }
    }
    c->r()->setL(c); c->l()->setR(c);
  }

  void testPrintHeader() {
    for (Header* h = (Header*)root->r(); h != root; h = (Header*)h->r()) {
      cout << h->dscpt() << "->";
    }
    cout << endl;
  }
  Header* chooseColumn() {
    // based on smallest first heuristic
    Node* h = NULL;
    Header* rsl = (Header*)((Node*)root->r());
    int minSize = ((Header*)((Node*)root->r()))->size();
    for (h = (Node*)root->r(); h != root && ((Header*)h)->dscpt().find("Diag-") != string::npos; h = h->r()) {
      int size = ((Header*)h)->size();
      if (size < minSize && size != 0) {
	minSize = size;
	rsl = (Header*)h;
      } else if (minSize == 0) {
	minSize = size;
      }
    }
    if (minSize == 0) return NULL;
    return rsl;
  }

  void search(int k) {
    if (root->r() == root) {
      // print current solution, and return;
      int* tmp = new int[n];
      for (vector<Node*>::iterator itr = O->begin();
       	   itr != O->end(); itr++) {
	int rank = ((Element*)(*itr))->getC()->id;
	int file = ((Element*)(*itr))->r()->getC()->id;
	tmp[rank-1] = file;
      }
      rsl->push_back(tmp);
    } else {
      // choose a column: use heuristic
      Header* c = chooseColumn();
      if (c == NULL)
      	return;
	
      /////////////////Modifications//////////////////
      cover(c);
      for (Node* r = c->d(); r != c; r = r->d()) {
	O->at(k) = r;
	//Node* ok = r;
	for (Node* j = r->r(); j != r; j = j->r()) {
	  Header* h = ((Element*)j)->getC();
	  cover(h);
	}
	search(k+1);
	r = O->at(k);
	//r = ok;
	c = ((Element*)r)->getC();
	for (Node* j = r->l(); j != r; j = j->l()) {
	  Header* h = ((Element*)j)->getC();
	  uncover(h);
	}
      }
      uncover(c);
      /////////////////Modifications//////////////////      
    }
  }
  void erase() {
    delete root;
  }
  vector<int*>* getRsl() {
    return rsl;
  }
};



int main(int ac, char* av[]) {
  int maxN = 27; // number of queens
  bool printSolutions = false;
  bool eval = false;
  try {
    po::options_description desc("Opions of n-queens solver");
    desc.add_options()
      ("help", "print all the available options")
      ("maxN", po::value<int>(), "maximum number of queens (default is 27)")
      ("print", "print the solutions (default is false)")
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
      std::cout << "[Warning] No maxium number of queens specified. Use 27." << std::endl;
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
    ofstream soluFile;
    ofstream soluNumFile;
    evalFile.open("eval.txt");
    if (printSolutions) soluFile.open("solutions.txt");
    soluNumFile.open("solutionNumber.txt");
    for (int i = 4; i <= maxN; i++) {
      Time t1(boost::posix_time::microsec_clock::local_time());
      Matrix m;
      m.genForNQueens(i);
      m.search(0);
      Time t2(boost::posix_time::microsec_clock::local_time());
      TimeDuration dt = t2 - t1;

      //number of elapsed miliseconds
      long msec = dt.total_nanoseconds();
      
      vector<int*>* rsl = m.getRsl();
      //print elapsed seconds (with millisecond precision)
      evalFile << i <<  ", " <<  msec << endl;
      int soluSize = rsl->size();
      soluNumFile << i << ", "<< soluSize << endl;
      if (printSolutions) {
	soluFile << "N=" << i << ":" << endl;
	for (vector<int*>::iterator itr = rsl->begin(); itr != rsl->end(); itr++) {
	  soluFile << "One solution: ";
	  for (int j = 0; j < i;  j++) {
	    soluFile << "(" << j+1 <<"," << (*itr)[j] << "), ";
	  }
	  soluFile << endl;
	}
      }
      m.erase();
      delete rsl;
    }
    evalFile.close();
    if (printSolutions) soluFile.close();
    soluNumFile.close();
      
  }
  cout << endl << "Finished!" << endl;
  return 0;
}
