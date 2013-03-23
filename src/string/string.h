// Copyright 2013 Eugen Sawin <esawin@me73.com>
#ifndef SRC_STRING_STRING_H_
#define SRC_STRING_STRING_H_

#include <cassert>
#include <string>

namespace flow {
namespace string {

// Replaces all occurences of old_str with new_str in the given string.
void Replace(const std::string& old_str, const std::string& new_str,
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

}  // namespace string
}  // namespace flow
#endif  // SRC_STRING_STRING_H_
