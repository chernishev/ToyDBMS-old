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

#ifndef BASICS_H
#define BASICS_H

#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>


const int MAXLEN = 1000;

enum ValueType : short {
  VT_INT,
  VT_STRING
};

enum COLUMN_SORT {
  CS_ASCENDING,
  CS_DESCENDING,
  CS_NO,
  CS_UNKNOWN
};

struct Value {
  ValueType vtype;
  int vint;
  std::string vstr;

  Value(int v) {
    vtype = VT_INT;
    vint = v;
    vstr = "";
  }
  Value(std::string v) {
    vtype = VT_STRING;
    vstr = v;
  }
  Value() {
    vtype = VT_INT;
    vint = 0;
    vstr = "";
  }
  operator int() const {
    return vint;
  }
  operator std::string() const {
    return vstr;
  }
  ~Value() {
  }
};

enum PredicateType {
  PT_EQUALS,
  PT_GREATERTHAN,
};

struct Predicate {
  PredicateType ptype;
  ValueType vtype;
  int attribute;
  int vint;
  std::string vstr;
  Predicate(PredicateType ptype, ValueType vtype, int attribute, int vint, std::string vstr) {
    this->ptype = ptype;
    this->vtype = vtype;
    this->attribute = attribute;
    this->vint = vint;
    this->vstr = vstr;
  }
  Predicate(const Predicate& p) {
    this->ptype = p.ptype;
    this->vtype = p.vtype;
    this->attribute = p.attribute;
    this->vint = p.vint;
    this->vstr = p.vstr;
  }
  Predicate() {}
  ~Predicate() {}
};

inline std::ostream& operator<<(std::ostream& stream, const Predicate& p) {
  if (p.ptype == PT_EQUALS)
    stream << "x == ";
  else
    stream << "x < ";

  if (p.vtype == VT_INT)
    stream << p.vint;
  else
    stream << p.vstr;
  return stream;
}

struct BaseTable {
  std::string relpath;
  int nbAttr;
  std::vector<ValueType> vtypes;
  std::vector<std::string> vnames;
  std::vector<COLUMN_SORT> vorders;
  BaseTable() {}
  BaseTable(std::string p): relpath(p) {
    std::string line, word;
    std::ifstream fin(relpath.c_str());
    if (fin.is_open()) {
      fin >> nbAttr;
      // names
      getline(fin, line);
      getline(fin, line);
      std::istringstream iss(line, std::istringstream::in);
      while (iss >> word) {
        vnames.push_back(word);
      }
      // types
      getline(fin, line);
      std::istringstream iss2(line, std::istringstream::in);
      while (iss2 >> word) {
        if (word == "INT")
          vtypes.push_back(VT_INT);
        else
          vtypes.push_back(VT_STRING);
      }
      // order
      getline(fin, line);
      std::istringstream iss3(line, std::istringstream::in);
      while (iss3 >> word) {
        if (word == "ASCENDING")
          vorders.push_back(CS_ASCENDING);
        else if (word == "DESCENDING")
          vorders.push_back(CS_DESCENDING);
        else if (word == "UNKNOWN")
          vorders.push_back(CS_UNKNOWN);
        else if (word == "UNKNOWN")
          vorders.push_back(CS_NO);
      }

      fin.close();
    } else std::cout << "Unable to open file";
  }
  ~BaseTable() {}
};

inline std::ostream& operator<<(std::ostream& stream, const BaseTable& bt) {
  stream << "located in " << bt.relpath << " having " << bt.nbAttr << " following attributes:" << std::endl;
  for (int i = 0; i < bt.nbAttr; i++) {
    stream << i << ". " << bt.vnames[i] << " ";
    if (bt.vtypes[i] == VT_INT)
      stream << "INT ";
    else
      stream << "STR ";
    if (bt.vorders[i] == CS_ASCENDING)
      stream << "ASCENDING";
    else if (bt.vorders[i] == CS_DESCENDING)
      stream << "DESCENDING";
    else if (bt.vorders[i] == CS_UNKNOWN)
      stream << "UNKNOWN";
    else if (bt.vorders[i] == CS_NO)
      stream << "UNSORTED";
    stream << std::endl;
  }
  return stream;
}


enum ErrCode {
  EC_OK,
  EC_FINISH,
  EC_ERROR
};

#endif // BASICS_H
