#include <string>
#include <iostream>
using namespace std;
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
};

class Header : public Node {
private:
  int size;
  string description;
public :
  Header() : size(0), description("") {
  }
  int sizeg() {
    return this->size;
  }
  string dscpt() {
    return description;
  }
  void setSize(int s) {
    size = s;
  }
  void setDescription(string s) {
    description = s;
  }
  int increase() {
    size++;
    return size;
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
public:
  Matrix() {}
  Root* getRoot() {
    return root;
  }
  void genForNQueens(int n) {
    root = new Root();
    Node* curr = NULL;
    curr = root;
    //// First, generating Headers
    // for n ranks
    Header* header;
    for (int i = 1; i <= n; i++) {
      header = new Header();
      header->setDescription("rank-"+to_string(i));
      curr->setR(header);
      header->setL(curr);
      curr = header;
    }
    // for n files
    for (int i = 1; i <= n; i++) {
      header = new Header();
      header->setDescription("file-"+to_string(i));
      curr->setR(header);
      header->setL(curr);
      curr = header;
    }
    // for represent diagonals
    for (int i =  1; i <= 2*n-1; i++) {
      header = new Header();
      header->setDescription("pDiag-"+to_string(i));
      curr->setR(header);
      header->setL(curr);
      curr = header;
    }
    // for reverse diagonals
    for (int i =  1; i <= 2*n-1; i++) {
      header = new Header();
      header->setDescription("rDiag-"+to_string(i));
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
	// for rank
	rank = new Element();
	rank->setC(getHeader("rank-"+to_string(i)));
	// for file
	file = new Element();
	file->setC(getHeader("fiel-"+to_string(j)));
	// for pDiagonal
	pDiag = new Element();
	pDiag->setC(getHeader("pDiag-"+to_string(i+j-1)));
	// for rDiagonal
	rDiag = new Element();
	rDiag->setC(getHeader("rDiag-"+to_string(n-i+j)));
	// connect them to form a row
	rank->setR(file);
	rank->setL(rDiag);
	file->setR(pDiag);
	file->setL(rank);
	pDiag->setR(rDiag);
	pDiag->setL(file);
	rDiag->setR(rank);
	rDiag->setL(pDiag);
	// debug
	testPrint(i, n);testPrint(j, n);testPrint(i+j-1, 2*n-1); testPrint(n-i+j, 2*n-1);
	std::cout << endl;
      }
    }
    //    std::cout << "Header rank3: " << getHeader("rank-3")->dscpt() << std::endl;
  }
  void testPrint() {
    Matrix m = *this;
    Root* root = m.getRoot();
    Node* currH = root->r();
    while(!currH->isRoot()) {
      // print the column
      Header* h = (Header*)currH;
      std::cout << h->dscpt() << ": ";
      // print the elements of this column
      Node* elm = h->d();
      while (!elm->isHeader()) {
	Element* e = (Element*) elm;
	std::cout << "1,";
	// next element
	elm = elm->d(); 
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
  
};

  
int main() {
  Matrix m;
  m.genForNQueens(4);
  m.testPrint();
}
