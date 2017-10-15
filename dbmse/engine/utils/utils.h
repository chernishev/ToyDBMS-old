#pragma once

namespace utils {

    template<typename T>
    void append_to_back(std::vector<T> &to, const std::vector<T> &from) {
      to.insert(std::end(to), std::begin(from), std::end(from));
    }

}