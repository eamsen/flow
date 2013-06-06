// Copyright 2013 Eugen Sawin <esawin@me73.com>
#ifndef SRC_STRING_STRING_H_
#define SRC_STRING_STRING_H_

#include <cassert>
#include <string>
#include <vector>

namespace flow {
namespace string {

// Replaces all occurences of old_str with new_str in the given string.
inline void Replace(const std::string& old_str, const std::string& new_str,
    std::string* str) {
  assert(str);
  if (old_str.size() == 0) {
    return;
  }
  auto pos = str->find(old_str, 0);
  while (pos != std::string::npos) {
    str->replace(pos, old_str.size(), new_str);
    pos = str->find(old_str, pos + new_str.size());
  }
}

// Splits the given string at any of the given delimeters.
inline std::vector<std::string> Split(const std::string& str,
    const std::string& delims) {
  std::vector<std::string> substrings;
  size_t pos = str.find_first_not_of(delims);
  while (pos != std::string::npos) {
    size_t end = str.find_first_of(delims, pos);
    if (end == std::string::npos) {
      // Last item found.
      substrings.push_back(str.substr(pos));
    } else {
      // Item found.
      substrings.push_back(str.substr(pos, end - pos));
    }
    pos = str.find_first_not_of(delims, end);
  }
  return substrings;
}

}  // namespace string
}  // namespace flow
#endif  // SRC_STRING_STRING_H_
