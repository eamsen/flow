// Copyright 2013 Eugen Sawin <esawin@me73.com>
#ifndef SRC_IO_STRINGIFY_H_
#define SRC_IO_STRINGIFY_H_

#include <cassert>
#include <string>
#include <sstream>
#include <tuple>
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
  static const std::string kDelim;
  static const std::string kPairDiv;
  static const std::string kWrapStart;
  static const std::string kWrapEnd;
  static const std::string kElemWrapStart;
  static const std::string kElemWrapEnd;
  static const std::string kStringWrap;

  static std::string delim;
  static std::string pair_div;
  static std::string wrap_start;
  static std::string wrap_end;
  static std::string elem_wrap_start;
  static std::string elem_wrap_end;
  static std::string string_wrap;
};

const std::string Stringify::kDelim = ", ";
const std::string Stringify::kPairDiv = ": ";
const std::string Stringify::kWrapStart = "(";
const std::string Stringify::kWrapEnd = ")";
const std::string Stringify::kElemWrapStart = "";
const std::string Stringify::kElemWrapEnd = "";
const std::string Stringify::kStringWrap = "\"";

std::string Stringify::delim = Stringify::kDelim;
std::string Stringify::pair_div = Stringify::kPairDiv;
std::string Stringify::wrap_start = Stringify::kWrapStart;
std::string Stringify::wrap_end = Stringify::kWrapEnd;
std::string Stringify::elem_wrap_start = Stringify::kElemWrapStart;
std::string Stringify::elem_wrap_end = Stringify::kElemWrapEnd;
std::string Stringify::string_wrap = Stringify::kStringWrap;

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

// Stream operator overload for tuples.
template<typename... Tp>
std::ostream& operator<<(std::ostream& stream, const std::tuple<Tp...>& t) {
  return tuple_stream(t, stream, flow::io::Stringify::delim,
      flow::io::Stringify::wrap_start, flow::io::Stringify::wrap_end,
      flow::io::Stringify::pair_div,
      flow::io::Stringify::elem_wrap_start, flow::io::Stringify::elem_wrap_end,
      flow::io::Stringify::string_wrap);
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
    const std::string& delim = flow::io::Stringify::kDelim,
    const std::string& wrap_start = flow::io::Stringify::kWrapStart,
    const std::string& wrap_end = flow::io::Stringify::kWrapEnd,
    const std::string& pair_div = flow::io::Stringify::kPairDiv,
    const std::string& elem_wrap_start = flow::io::Stringify::kElemWrapStart,
    const std::string& elem_wrap_end = flow::io::Stringify::kElemWrapEnd,
    const std::string& string_wrap = flow::io::Stringify::kStringWrap);

// Returns the string representation for the given value, args are ignored.
template<typename T>
std::string Str(const T& value,
    const std::string& delim = flow::io::Stringify::kDelim,
    const std::string& wrap_start = flow::io::Stringify::kWrapStart,
    const std::string& wrap_end = flow::io::Stringify::kWrapEnd,
    const std::string& pair_div = flow::io::Stringify::kPairDiv,
    const std::string& elem_wrap_start = flow::io::Stringify::kElemWrapStart,
    const std::string& elem_wrap_end = flow::io::Stringify::kElemWrapEnd,
    const std::string& string_wrap = flow::io::Stringify::kStringWrap) {
  return Convert<std::string>(value);
}

// Returns the string representation for the given string.
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
    const std::string& delim = flow::io::Stringify::kDelim,
    const std::string& wrap_start = flow::io::Stringify::kWrapStart,
    const std::string& wrap_end = flow::io::Stringify::kWrapEnd,
    const std::string& pair_div = flow::io::Stringify::kPairDiv,
    const std::string& elem_wrap_start = flow::io::Stringify::kElemWrapStart,
    const std::string& elem_wrap_end = flow::io::Stringify::kElemWrapEnd,
    const std::string& string_wrap = flow::io::Stringify::kStringWrap) {
  std::stringstream ss;
  ss << wrap_start
     << Str(pair.first, delim, wrap_start, wrap_end, pair_div,
            elem_wrap_start, elem_wrap_end, string_wrap)
     << pair_div
     << Str(pair.second, delim, wrap_start, wrap_end, pair_div,
            elem_wrap_start, elem_wrap_end, string_wrap)
     << wrap_end;
  return ss.str();
}

// Helper template for tuple iteration.
template<size_t I = 0, typename... Tp>
typename std::enable_if<I == sizeof...(Tp), std::ostream&>::type
TupleStream(const std::tuple<Tp...>& t, std::ostream& stream,
    const std::string& delim = flow::io::Stringify::kDelim,
    const std::string& wrap_start = flow::io::Stringify::kWrapStart,
    const std::string& wrap_end = flow::io::Stringify::kWrapEnd,
    const std::string& pair_div = flow::io::Stringify::kPairDiv,
    const std::string& elem_wrap_start = flow::io::Stringify::kElemWrapStart,
    const std::string& elem_wrap_end = flow::io::Stringify::kElemWrapEnd,
    const std::string& string_wrap = flow::io::Stringify::kStringWrap) {
  return stream;
}

// Helper template for tuple iteration.
template<size_t I = 0, typename... Tp>
typename std::enable_if<I < sizeof...(Tp), std::ostream&>::type
TupleStream(const std::tuple<Tp...>& t, std::ostream& stream,
    const std::string& delim = flow::io::Stringify::kDelim,
    const std::string& wrap_start = flow::io::Stringify::kWrapStart,
    const std::string& wrap_end = flow::io::Stringify::kWrapEnd,
    const std::string& pair_div = flow::io::Stringify::kPairDiv,
    const std::string& elem_wrap_start = flow::io::Stringify::kElemWrapStart,
    const std::string& elem_wrap_end = flow::io::Stringify::kElemWrapEnd,
    const std::string& string_wrap = flow::io::Stringify::kStringWrap) {
  if (I > 0) {
    stream << delim;
  }
  stream << Str(std::get<I>(t), delim, wrap_start, wrap_end, pair_div,
      elem_wrap_start, elem_wrap_end, string_wrap);
  return TupleStream<I + 1, Tp...>(t, stream, delim, wrap_start, wrap_end,
      pair_div, elem_wrap_start, elem_wrap_end, string_wrap);
}

// Returns the string representation for the given tuple based on provided
// delimiter and wrappers.
template<typename... Tp>
std::string Str(const std::tuple<Tp...>& t,
    const std::string& delim = flow::io::Stringify::kDelim,
    const std::string& wrap_start = flow::io::Stringify::kWrapStart,
    const std::string& wrap_end = flow::io::Stringify::kWrapEnd,
    const std::string& pair_div = flow::io::Stringify::kPairDiv,
    const std::string& elem_wrap_start = flow::io::Stringify::kElemWrapStart,
    const std::string& elem_wrap_end = flow::io::Stringify::kElemWrapEnd,
    const std::string& string_wrap = flow::io::Stringify::kStringWrap) {
  std::stringstream stream;
  stream << wrap_start;
  TupleStream(t, stream, delim, wrap_start, wrap_end, pair_div,
      elem_wrap_start, elem_wrap_end, string_wrap);
  stream << wrap_end;
  return stream.str();
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
