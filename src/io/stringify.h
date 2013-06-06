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
  static constexpr char* kDelim = ", ";
  static constexpr char* kPairDiv = ": ";
  static constexpr char* kWrapStart = "(";
  static constexpr char* kWrapEnd = ")";
  static constexpr char* kElemWrapStart = "";
  static constexpr char* kElemWrapEnd = "";
  static constexpr char* kStringWrap = "\"";

  static const std::string& Delim() {
    static std::string delim = kDelim;
    return delim;
  }

  static const std::string& PairDiv() {
    static std::string pair_div = kPairDiv;
    return pair_div;
  }

  static const std::string& WrapStart() {
    static std::string wrap_start = kWrapStart;
    return wrap_start;
  }

  static const std::string& WrapEnd() {
    static std::string wrap_end = kWrapEnd;
    return wrap_end;
  }

  static const std::string& ElemWrapStart() {
    static std::string elem_wrap_start = kElemWrapStart;
    return elem_wrap_start;
  }

  static const std::string& ElemWrapEnd() {
    static std::string elem_wrap_end = kElemWrapEnd;
    return elem_wrap_end;
  }

  static const std::string& StringWrap() {
    static std::string string_wrap = kStringWrap;
    return string_wrap;
  }
};

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
  return stream << pair.first << flow::io::Stringify::PairDiv() << pair.second;
}

// Stream operator overload for tuples.
template<typename... Tp>
std::ostream& operator<<(std::ostream& stream, const std::tuple<Tp...>& t) {
  return tuple_stream(t, stream, flow::io::Stringify::Delim(),
      flow::io::Stringify::WrapStart(), flow::io::Stringify::WrapEnd(),
      flow::io::Stringify::PairDiv(),
      flow::io::Stringify::ElemWrapStart(), flow::io::Stringify::ElemWrapEnd(),
      flow::io::Stringify::StringWrap());
}

template<template<typename...> class Container, typename... Args>
typename std::enable_if<!std::is_same<Container<Args...>, std::string>::value,
         std::ostream&>::type
operator<<(std::ostream& stream, const Container<Args...>& con) {
  auto begin = con.begin();
  const auto end = con.end();
  stream << flow::io::Stringify::WrapStart();
  if (begin != end) {
    stream << *begin;
    while (++begin != end) {
      stream << flow::io::Stringify::Delim() << *begin;
    }
  }
  return stream << flow::io::Stringify::WrapEnd();
}

namespace flow {
namespace io {

// Returns the string representation for the given container based on provided
// delimiter and wrapper.
template<template<typename...> class Container, typename... Args>
inline std::string Str(const Container<Args...>& con,
    const std::string& delim = flow::io::Stringify::kDelim,
    const std::string& wrap_start = flow::io::Stringify::kWrapStart,
    const std::string& wrap_end = flow::io::Stringify::kWrapEnd,
    const std::string& pair_div = flow::io::Stringify::kPairDiv,
    const std::string& elem_wrap_start = flow::io::Stringify::kElemWrapStart,
    const std::string& elem_wrap_end = flow::io::Stringify::kElemWrapEnd,
    const std::string& string_wrap = flow::io::Stringify::kStringWrap);

// Returns the string representation for the given tuple based on provided
// delimiter and wrappers.
template<typename... Tp>
inline std::string Str(const std::tuple<Tp...>& t,
    const std::string& delim = flow::io::Stringify::kDelim,
    const std::string& wrap_start = flow::io::Stringify::kWrapStart,
    const std::string& wrap_end = flow::io::Stringify::kWrapEnd,
    const std::string& pair_div = flow::io::Stringify::kPairDiv,
    const std::string& elem_wrap_start = flow::io::Stringify::kElemWrapStart,
    const std::string& elem_wrap_end = flow::io::Stringify::kElemWrapEnd,
    const std::string& string_wrap = flow::io::Stringify::kStringWrap);

// Returns the string representation for the given value, args are ignored.
template<typename T>
inline std::string Str(const T& value,
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
inline std::string Str(const std::string& value, const std::string& delim,
    const std::string& wrap_start, const std::string& wrap_end,
    const std::string& pair_div,
    const std::string& elem_wrap_start, const std::string& elem_wrap_end,
    const std::string& string_wrap) {
  return string_wrap + value + string_wrap;
}

// Returns the string representation for the given pair based on provided
// delimiter, wrapper and pair delimiter.
template<typename T1, typename T2>
inline std::string Str(const std::pair<T1, T2>& pair,
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
inline std::string Str(const std::tuple<Tp...>& t,
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
inline std::string Str(const Container<Args...>& con, const std::string& delim,
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

template<template<typename...> class Container, typename... Args>
inline std::string JsonArray(const Container<Args...>& con) {
  return Str(con, ",", "[", "]", ",", "", "", "\"");
}

}  // namespace io
}  // namespace flow
#endif  // SRC_IO_STRINGIFY_H_
