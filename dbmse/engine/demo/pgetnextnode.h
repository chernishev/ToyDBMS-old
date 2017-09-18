// Toy DBMS, v0.4
// George Chernishev (c) 2016-2017, chernishev<at>google mail
// A task framework for undergraduate students at Saint-Petersburg Academic University, DBMS development course
// More details regarding the course can be found here: www.math.spbu.ru/user/chernishev/
// CHANGELOG:
// 0.4: no chance for efficiency competition, so, this year I reoriented task towards ideas:
//      1) tried to remove C code, now we are using lots of std:: stuff
//      2) implemented support for multiple attributes in the DBMS
//      3) code clean-up and restructurization
// 0.3: added:
//      1) support for restricting physical join node size
//      2) support for deduplication node, LUniqueNode
//      3) print methods for Predicate and BaseTable
//      updated:
//      1) new format for data files: third line is the sort status now
//      2) added projection code
//      3) contract contains print methods for physical and logical nodes
// 0.2: first public release

#ifndef PGETNEXTNODE_H
#define PGETNEXTNODE_H
#include <vector>
#include "../interface/interface.h"

class PGetNextNode : public PResultNode{
  public:
    PGetNextNode();
    PGetNextNode(PResultNode* left, PResultNode* right, LAbstractNode* p);
    // internal way to transfer data
    virtual std::vector<std::vector<Value>> GetNext();
    // getting access to data
    virtual void Initialize();
    // get number of attributes
    virtual int GetAttrNum();
  protected:

  private:

};

#endif // PGETNEXTNODE_H
