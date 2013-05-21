// Copyright 2013 Eugen Sawin <esawin@me73.com>
#ifndef SRC_IO_HTML_H_
#define SRC_IO_HTML_H_

#include <string>
#include <sstream>

namespace flow {
namespace io {

std::string StripHtml(const std::string& html) {
  std::stringstream ss;
  const size_t end = html.size();
  size_t pos = html.find("body");
  pos = html.find(">", pos);
  int tag_level = 1;
  while (pos < end && tag_level) {
    if (html[pos] == '<') {
      if (html[pos + 1] == '/') {
        --tag_level;
      } else {
        ++tag_level;
        if (html.substr(pos, 7) == "<script") {
          pos = html.find("</script", pos + 7);
          --tag_level;
        }
      }
      pos = html.find(">", pos) + 1;
    } else if (html[pos] == '>') {
      ++pos;
    } else {
      const size_t end = html.find_first_of("<>", pos);
      ss << html.substr(pos, end - pos);
      pos = end;
    }
  }
  return ss.str();
}

}  // namespace io
}  // namespace flow
#endif  // SRC_IO_HTML_H_
