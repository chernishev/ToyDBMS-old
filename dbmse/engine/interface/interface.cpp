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

#include <algorithm>
#include <tuple>
#include <string.h>
#include "interface.h"

LAbstractNode::LAbstractNode(LAbstractNode* left, LAbstractNode* right){
  this->left = left;
  this->right = right;
}

LAbstractNode::~LAbstractNode(){
}

LAbstractNode* LAbstractNode::GetLeft(){
  return left;
}

LAbstractNode* LAbstractNode::GetRight(){
  return right;
}

LJoinNode::LJoinNode(LAbstractNode* left, LAbstractNode* right,
                     std::string offset1, std::string offset2, int memorylimit):LAbstractNode(left, right){
  this->offset1 = offset1;
  this->offset2 = offset2;
  this->memorylimit = memorylimit;

  // TODO: disgusting, fix this
  std::vector<std::string> match;
  ValueType vt;
  COLUMN_SORT cs;
  for (int i = 0; i < left->fieldNames.size(); i++){
    for (int j = 0; j < right->fieldNames.size(); j++){
      std::vector<std::string> l = left->fieldNames[i];
      std::vector<std::string> r = right->fieldNames[j];

      if(std::find(l.begin(), l.end(), offset1) != l.end()){
        if(std::find(r.begin(), r.end(), offset2) != r.end()){
          match = l;
          match.insert(std::end(match), std::begin(r), std::end(r));
          vt = left->fieldTypes[i];
          cs = left->fieldOrders[i];
        }
      } else
      if(std::find(l.begin(), l.end(), offset2) != l.end()){
        if(std::find(r.begin(), r.end(), offset1) != r.end()){
          match = l;
          match.insert(std::end(match), std::begin(r), std::end(r));
          vt = left->fieldTypes[i];
          cs = left->fieldOrders[i];
        }
      }
    }
  }

  for (int i = 0; i < left->fieldNames.size(); i++){
    std::vector<std::string> l = left->fieldNames[i];
    if(std::find(l.begin(), l.end(), offset1) == l.end())
      if(std::find(l.begin(), l.end(), offset2) == l.end()){
        fieldNames.push_back(l);
        fieldTypes.push_back(left->fieldTypes[i]);
        fieldOrders.push_back(left->fieldOrders[i]);
      }
  }

  for (int i = 0; i < right->fieldNames.size(); i++){
    std::vector<std::string> r = right->fieldNames[i];
    if(std::find(r.begin(), r.end(), offset1) == r.end())
      if(std::find(r.begin(), r.end(), offset2) == r.end()){
        fieldNames.push_back(r);
        fieldTypes.push_back(right->fieldTypes[i]);
        fieldOrders.push_back(right->fieldOrders[i]);

      }

  }
  fieldNames.push_back(match);
  fieldTypes.push_back(vt);
  fieldOrders.push_back(cs);

}

LJoinNode::~LJoinNode(){
  delete left;
  delete right;
}

LProjectNode::LProjectNode(LAbstractNode* child, std::vector<std::string> tokeep):LAbstractNode(child, NULL){
  for (int i = 0; i < left->fieldNames.size(); i++){
    for (int j = 0; j < tokeep.size(); j++){
      std::vector<std::string> source = left->fieldNames[i];
      std::string candidate = tokeep[j];
      if(std::find(source.begin(), source.end(), candidate) != source.end()){
        fieldNames.push_back(source);
        fieldTypes.push_back(left->fieldTypes[i]);
        fieldOrders.push_back(left->fieldOrders[i]);
        continue;
      }
    }
  }
}

LProjectNode::~LProjectNode(){
  delete left;
}

LSelectNode::LSelectNode(BaseTable& table,
                         std::vector<Predicate> predicates): LAbstractNode(NULL, NULL){
  this->table = table;
  this->predicates = predicates;
  iteratorpos = 0;
  for (int i = 0; i < table.nbAttr; i++){
    std::string tmp = table.relpath + "." + table.vnames[i];
    std::vector<std::string> tmp2;
    tmp2.push_back(tmp);
    fieldNames.push_back(tmp2);
  }
  fieldTypes = table.vtypes;
  fieldOrders = table.vorders;
}

BaseTable& LSelectNode::GetBaseTable(){
  return table;
}

std::tuple<int, Predicate> LSelectNode::GetNextPredicate(){
  if(predicates.size() == 0 || iteratorpos >= predicates.size()){
      return std::make_tuple(1, Predicate());
  }
  return std::make_tuple(0, predicates[iteratorpos++]);
}

void LSelectNode::ResetIterator(){
  iteratorpos = 0;
}


LSelectNode::~LSelectNode(){
}

LUniqueNode::LUniqueNode(LAbstractNode* child):LAbstractNode(child, NULL){
}

LUniqueNode::~LUniqueNode(){
}


/* Physical nodes*/

PResultNode::PResultNode(PResultNode* left, PResultNode* right, LAbstractNode* p){
  this->left = left;
  this->right = right;
  this->prototype = p;
  pos = 0;
}

PResultNode::~PResultNode(){
}

std::tuple<ErrCode, std::vector<Value>> PResultNode::GetRecord(){
  std::vector<Value> vals;
  if (pos == data.size()) return std::make_tuple(EC_FINISH, vals);
  for(int i = 0; i < GetAttrNum(); i++){
    vals.push_back(data[pos][i]);
  }
  pos++;
  return std::make_tuple(EC_OK, vals);
}
