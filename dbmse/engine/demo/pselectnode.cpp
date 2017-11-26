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

#include <iostream>
#include <fstream>
#include <string>

#include <string.h>
#include <vector>
#include <sstream>

#include "pselectnode.h"

PSelectNode::PSelectNode(LAbstractNode* p, std::vector<Predicate> predicate)
    : PGetNextNode()
    , table(dynamic_cast<LSelectNode*>(p)->GetBaseTable())
    , predicates(predicate)
    , pos(0) {
  prototype = p;
  data.clear();
  Initialize();
}

template <typename T>
bool apply_comparison_predicate(const PredicateType &type, const T& original, const T& compare_to)
{
  switch (type) {
    case PT_EQUALS:
      if (original == compare_to) {
        return true;
      }
      break;
    case PT_GREATERTHAN:
      if (original >= compare_to) {
        return true;
      }
      break;
  }

  return false;

}

bool apply_int_predicate(const Value &compared_attribute, const Predicate &predicate) {
  return apply_comparison_predicate(predicate.ptype, compared_attribute.vint, predicate.vint);
}

bool apply_string_predicate(const Value &compared_attribute, const Predicate &predicate) {
  return apply_comparison_predicate(predicate.ptype, compared_attribute.vstr, predicate.vstr);
}

bool matches_predicates(const BaseTable& table, const std::vector<Value> &record, const std::vector<Predicate> &predicates) {
  for (auto& predicate: predicates) {
    if (table.vtypes[predicate.attribute] == predicate.vtype) {
      auto& compared_attribute = record[predicate.attribute];
      switch (predicate.vtype) {
        case VT_INT:
          if (!apply_int_predicate(compared_attribute, predicate)) {
            return false;
          }
          break;
        case VT_STRING:
          if (!apply_string_predicate(compared_attribute, predicate)) {
            return false;
          }
          break;
        default:
          throw std::runtime_error("Could not match predicate type!");
      }
    }
  }

  return true;
}

void PSelectNode::Initialize(){
  std::string line, word;
  std::ifstream f(table.relpath);
  if(f.is_open()){
    // skipping first 4 lines
    getline(f, line);
    getline(f, line);
    getline(f, line);
    getline(f, line);

    while (getline(f, line)){
      std::vector<Value> tmp;
      std::istringstream iss(line, std::istringstream::in);
      int i = 0;
      while (iss >> word){
        // Yeah, no predicates :) -- Homework
        Value h;
        if (prototype->fieldTypes[i] == VT_INT) {
          h = Value(std::stoi(word));
        } else {
          h = Value(word);
        }
        tmp.push_back(h);
        i++;
      }

      if (matches_predicates(this->table, tmp, predicates)) {
        data.push_back(tmp);
      }
    }
    f.close();
  } else std::cout << "Unable to open file";
}

std::vector<std::vector<Value>> PSelectNode::GetNext(){
  return data;
}

void PSelectNode::Print(int indent) {
  for (int i = 0; i < indent; i++) {
    std::cout << " ";
  }
  std::cout << "SCAN " << table.relpath << " with predicates ";
  if (!predicates.empty()) {
    std::cout << predicates[0];
  } else {
    std::cout << "NULL" << std::endl;
  }
  if (left != NULL) left->Print(indent + 2);
  if (right != NULL) right->Print(indent + 2);
}

