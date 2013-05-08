// Copyright 2013 Eugen Sawin <esawin@me73.com>
#ifndef SRC_IO_STRINGIFY_H_
#define SRC_IO_STRINGIFY_H_

#include <cassert>
#include <string>
#include <sstream>
#include <type_traits>
#include <utility>

namespace flow {
namespace io {

// Converts the given value to the desired output type.
template<typename Output, typename Input>
Output Convert(const Input& value) {
  std::stringstream ss;
  ss << value;
  Output out;
  ss >> out;
  return out;
}

// Delimiters and wrappers used for string conversions invoked by the overloaded
// stream << operator.
// TODO(esawin): Make thread-safe.
struct Stringify {
  static std::string delim;
  static std::string pair_div;
  static std::string wrap_start;
  static std::string wrap_end;
  static std::string elem_wrap_start;
  static std::string elem_wrap_end;
  static std::string string_wrap;
};

std::string Stringify::delim = ", ";
std::string Stringify::pair_div = ": ";
std::string Stringify::wrap_start = "(";
std::string Stringify::wrap_end = ")";
std::string Stringify::elem_wrap_start = "";
std::string Stringify::elem_wrap_end = "";
std::string Stringify::string_wrap = "";

}  // namespace io
}  // namespace flow

// Stream operator overload for container types.
template<template<typename...> class Container, typename... Args>
typename std::enable_if<!std::is_same<Container<Args...>, std::string>::value,
         std::ostream&>::type
operator<<(std::ostream& stream, const Container<Args...>& con);

// Stream operator overload for pair types.
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& stream, const std::pair<T1, T2>& pair) {
  return stream << pair.first << flow::io::Stringify::pair_div << pair.second;
}

template<template<typename...> class Container, typename... Args>
typename std::enable_if<!std::is_same<Container<Args...>, std::string>::value,
         std::ostream&>::type
operator<<(std::ostream& stream, const Container<Args...>& con) {
  auto begin = con.begin();
  const auto end = con.end();
  stream << flow::io::Stringify::wrap_start;
  if (begin != end) {
    stream << *begin;
    while (++begin != end) {
      stream << flow::io::Stringify::delim << *begin;
    }
  }
  return stream << flow::io::Stringify::wrap_end;
}

namespace flow {
namespace io {

// Returns the string representation for the given container based on provided
// delimiter and wrapper.
template<template<typename...> class Container, typename... Args>
std::string Str(const Container<Args...>& con,
    const std::string& delim = flow::io::Stringify::delim,
    const std::string& wrap_start = flow::io::Stringify::wrap_start,
    const std::string& wrap_end = flow::io::Stringify::wrap_end,
    const std::string& pair_div = flow::io::Stringify::pair_div,
    const std::string& elem_wrap_start = flow::io::Stringify::elem_wrap_start,
    const std::string& elem_wrap_end = flow::io::Stringify::elem_wrap_end,
    const std::string& string_wrap = flow::io::Stringify::string_wrap);

// Returns the string representation for the given value, args are ignored.
template<typename T>
std::string Str(const T& value,
    const std::string& delim = flow::io::Stringify::delim,
    const std::string& wrap_start = flow::io::Stringify::wrap_start,
    const std::string& wrap_end = flow::io::Stringify::wrap_end,
    const std::string& pair_div = flow::io::Stringify::pair_div,
    const std::string& elem_wrap_start = flow::io::Stringify::elem_wrap_start,
    const std::string& elem_wrap_end = flow::io::Stringify::elem_wrap_end,
    const std::string& string_wrap = flow::io::Stringify::string_wrap) {
  return Convert<std::string>(value);
}

// Returns the string representation for the given string. Huh.
template<>
std::string Str(const std::string& value, const std::string& delim,
    const std::string& wrap_start, const std::string& wrap_end,
    const std::string& pair_div,
    const std::string& elem_wrap_start, const std::string& elem_wrap_end,
    const std::string& string_wrap) {
  return string_wrap + value + string_wrap;
}

// Returns the string representation for the given pair based on provided
// delimiter, wrapper and pair delimiter.
template<typename T1, typename T2>
std::string Str(const std::pair<T1, T2>& pair,
    const std::string& delim = flow::io::Stringify::delim,
    const std::string& wrap_start = flow::io::Stringify::wrap_start,
    const std::string& wrap_end = flow::io::Stringify::wrap_end,
    const std::string& pair_div = flow::io::Stringify::pair_div,
    const std::string& elem_wrap_start = flow::io::Stringify::elem_wrap_start,
    const std::string& elem_wrap_end = flow::io::Stringify::elem_wrap_end,
    const std::string& string_wrap = flow::io::Stringify::string_wrap) {
  return Str(pair.first, delim, wrap_start, wrap_end, pair_div,
             elem_wrap_start, elem_wrap_end, string_wrap) +
    pair_div +
    Str(pair.second, delim, wrap_start, wrap_end, pair_div,
        elem_wrap_start, elem_wrap_end, string_wrap);
}

template<template<typename...> class Container, typename... Args>
std::string Str(const Container<Args...>& con, const std::string& delim,
    const std::string& wrap_start, const std::string& wrap_end,
    const std::string& pair_div,
    const std::string& elem_wrap_start, const std::string& elem_wrap_end,
    const std::string& string_wrap) {
  auto begin = con.begin();
  const auto end = con.end();
  std::ostringstream ss;
  ss << wrap_start;
  if (begin != end) {
    ss << elem_wrap_start
       << Str(*begin, delim, wrap_start, wrap_end, pair_div,
              elem_wrap_start, elem_wrap_end, string_wrap)
       << elem_wrap_end;
    while (++begin != end) {
      ss << delim << elem_wrap_start
         << Str(*begin, delim, wrap_start, wrap_end, pair_div,
                elem_wrap_start, elem_wrap_end, string_wrap)
         << elem_wrap_end;
    }
  }
  ss << wrap_end;
  return ss.str();
}

}  // namespace io
}  // namespace flow
#endif  // SRC_IO_STRINGIFY_H_
