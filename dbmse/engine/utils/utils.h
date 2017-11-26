#pragma once

#include <algorithm>

namespace utils {

    template<typename T>
    void append_to_back(std::vector<T> &to, const std::vector<T> &from) {
      to.insert(std::end(to), std::begin(from), std::end(from));
    }

    template<typename T>
    bool contains(std::vector<T> &in, const T &value) {
      return std::find(std::begin(in), std::end(in), value) != std::end(in);
    }
}