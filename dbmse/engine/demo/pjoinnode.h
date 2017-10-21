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

#ifndef PJOINNODE_H
#define PJOINNODE_H

#include <vector>
#include <memory>
#include "../interface/interface.h"
#include "pgetnextnode.h"

class PJoinNode : public PGetNextNode {
  public:
    PJoinNode(std::unique_ptr<PGetNextNode> left, std::unique_ptr<PGetNextNode> right, LAbstractNode* p);
    virtual std::vector<std::vector<Value>> GetNext();
    virtual void Initialize();
    virtual void Print(int indent);
  private:
    int pos;
};

#endif // PJOINNODE_H
