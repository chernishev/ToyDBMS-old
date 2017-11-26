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

#include <typeinfo>
#include <iostream>
#include <vector>
#include <tuple>
#include "../interface/interface.h"
#include "../interface/basics.h"
#include "pselectnode.h"
#include "pjoinnode.h"
#include "pcrossproductnode.h"
#include "pprojectnode.h"

// Here be rewriter and optimizer
PResultNode* QueryFactory(LAbstractNode* node){
  if (auto* selectNode = dynamic_cast<LSelectNode*>(node)){
    std::vector<Predicate> p = selectNode->predicates;
    return new PSelectNode(selectNode, p);
  }

  if (auto* joinNode = dynamic_cast<LJoinNode*>(node)) {
    auto* leftPNode = dynamic_cast<PGetNextNode*>(QueryFactory(joinNode->GetLeft()));
    auto* leftRNode = dynamic_cast<PGetNextNode*>(QueryFactory(joinNode->GetRight()));

    return new PJoinNode(leftPNode, leftRNode, node);
  }

  if (auto* l_cross_product_node = dynamic_cast<LCrossProductNode*>(node)) {
    auto* rres = dynamic_cast<PGetNextNode*>(QueryFactory(node->GetRight()));
    auto* lres = dynamic_cast<PGetNextNode*>(QueryFactory(node->GetLeft()));

    return new PCrossProductNode(lres, rres, l_cross_product_node);
  }

  if (auto l_project_node = dynamic_cast<LProjectNode*>(node)) {
    auto next = dynamic_cast<PGetNextNode*>(QueryFactory(l_project_node->GetLeft()));
    return new PProjectNode(next, l_project_node);
  }

  return nullptr;

}

void ExecuteQuery(PResultNode* query){
  std::tuple<ErrCode, std::vector<Value>> res;
  res = query->GetRecord();
  ErrCode ec = std::get<0>(res);
  std::vector<Value> vals = std::get<1>(res);
  while(ec == EC_OK){
    for (int i = 0; i < query->GetAttrNum(); i++){
      if(vals[i].vtype == VT_INT)
        std::cout << vals[i].vint << " ";
      else if(vals[i].vtype == VT_STRING)
        std::cout << vals[i].vstr << " ";
    }
    printf("\n");
    res = query->GetRecord();
    ec = std::get<0>(res);
    vals = std::get<1>(res);
  }

}

int main(){
  {
    std::cout << "Starting demo" << std::endl;
    std::cout << "Query1: plain select" << std::endl;
    BaseTable bt1 = BaseTable("table1");
    std::cout << bt1;
    LAbstractNode* n1 = new LSelectNode(bt1, {});
    PResultNode* q1 = QueryFactory(n1);
    q1->Print(0);
    ExecuteQuery(q1);
    delete n1;
    delete q1;
  }

  {
    std::cout << std::endl << "Query2: simple equi-join" << std::endl;
    BaseTable bt1 = BaseTable("table1");
    BaseTable bt2 = BaseTable("table2");
    std::cout << bt1;
    std::cout << bt2;
    std::vector<Predicate> predicates = {
        Predicate(PT_EQUALS, VT_STRING, 1, 0, "cero")
    };
    LAbstractNode* n1 = new LSelectNode(bt1, predicates);
    LAbstractNode* n2 = new LSelectNode(bt2, {});
//    LJoinNode* n3 = new LJoinNode(n1, n2, "table1.id", "table2.id2", 666);
    auto* n3 = new LCrossProductNode(n1, n2);
    PResultNode* q1 = QueryFactory(n3);
    std::cout << std::endl;
    q1->Print(0);
    ExecuteQuery(q1);
    delete n3;
    delete q1;
  }

  {
    std::cout << std::endl;
    BaseTable bt1 = BaseTable("table1");
    BaseTable bt2 = BaseTable("table2");
    std::cout << bt1;
    std::cout << bt2;
    std::vector<Predicate> predicates = {
        Predicate(PT_EQUALS, VT_STRING, 1, 0, "cero")
    };
    LAbstractNode* n1 = new LSelectNode(bt1, predicates);
    LAbstractNode* n2 = new LSelectNode(bt2, {});

    LProjectNode* p1 = new LProjectNode(n1, {"table1.description", "table1.frequency"});
    PResultNode* q1 = QueryFactory(p1);
    q1->Print(0);
    ExecuteQuery(q1);
  }

}
