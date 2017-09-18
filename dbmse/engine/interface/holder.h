#ifndef HOLDER_H
#define HOLDER_H

#include <typeinfo>
#include <string.h>

#include "basics.h"

class Holder {
  public:

    template<typename T>
    Holder (Type type, T val){
      type_ = type;
      impl_ = val;
    }


    Holder (const Holder& h){
      type_ = h.type_;
      impl_ = h.impl_;
    }

    ~Holder () {}

    operator int () const {
      if (type_ != VT_INT) {
        throw std::bad_cast();
      }
      return impl_.int_;
    }

    operator std::string () const {
      if (type_ != VT_STRING) {
        throw std::bad_cast();
      }
      return impl_.string_;
    }


  private:
    union Impl {
      int int_;
      std::string string_;
      Impl() { new(&string_) std::string; }
      Impl(const Impl& i){
        int_ = i.int_;
        string_ = i.string_;
      }
      ~Impl() {}
    } impl_;

    ValueType type_;

};

#endif // HOLDER_H

