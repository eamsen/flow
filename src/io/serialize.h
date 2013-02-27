// Copyright 2013 Eugen Sawin <esawin@me73.com>
#ifndef SRC_IO_SERIALIZE_H_
#define SRC_IO_SERIALIZE_H_

#include <cassert>
#include <istream>
#include <ostream>
#include <vector>

namespace flow {
namespace io {

// Returns whether the system is little endian.
inline bool IsLittleEndian() {
  uint16_t i = 1;
  return *reinterpret_cast<uint8_t*>(&i);
}

// Translates given value to network format.
template<typename T>
T ToNetworkFormat(const T& value) {
  static const bool is_le = IsLittleEndian();
  T net_value = value;
  assert(is_le && "Big Endian systems are not supported yet.");
  return net_value;
}

// Translates given value from network to system format.
template<typename T>
T FromNetworkFormat(const T& value) {
  static const bool is_le = IsLittleEndian();
  T net_value = value;
  assert(is_le && "Big Endian systems are not supported yet.");
  return net_value;
}

// Reads a value from the stream and writes it to the given target.
template<typename T>
void Read(std::istream& stream, T* target) {  // NOLINT
  stream.read(reinterpret_cast<char*>(target), sizeof(T));
  *target = FromNetworkFormat(*target);
}

// Reads a string from the stream and writes it to the given target.
inline void Read(std::istream& stream, std::string* target) {  // NOLINT
  uint64_t size;
  Read(stream, &size);
  if (size == 0) {
    return;
  }
  target->clear();
  target->resize(size);
  stream.read(&((*target)[0]), size);
}

// Reads a container from the stream and writes it to the given target.
template<template<typename T, typename...> class Container,
         typename T, typename... Args>
void Read(std::istream& stream, Container<T, Args...>* target);  // NOLINT

// Reads a pair from the stream and writes it to the given target.
template<typename T1, typename T2>
void Read(std::istream& stream, std::pair<T1, T2>* target) {  // NOLINT
  Read(stream, &target->first);
  Read(stream, &target->second);
}

// Proxy reader used to switch between reader for associative containers and
// regular sequenced containers.
template<bool isMap, template<typename T, typename...> class Container,
         typename T, typename... Args>
struct Reader {
  static void _Read(std::istream& stream,  // NOLINT
                    Container<T, Args...>* target) {
    assert(false && "Unsupported serialization object.");
  }
};

// Reads a container from the stream and writes it to the given target.
template<template<typename T, typename...> class Container,
         typename T, typename... Args>
struct Reader<false, Container, T, Args...> {
  static void _Read(std::istream& stream,  // NOLINT
                    Container<T, Args...>* target) {
    uint64_t n;
    Read(stream, &n);
    if (n == 0) {
      return;
    }
    std::vector<T> vec;
    vec.reserve(n);
    while (n--) {
      vec.push_back(T());
      Read(stream, &vec.back());
    }
    *target = Container<T, Args...>(vec.begin(), vec.end());
  }
};

// Reads an associative container from stream and writes it to the given target.
template<template<typename T, typename...> class Container,
         typename T, typename... Args>
struct Reader<true, Container, T, Args...> {
  static void _Read(std::istream& stream,  // NOLINT
                    Container<T, Args...>* target) {
    typedef typename Container<T, Args...>::key_type K;
    typedef typename Container<T, Args...>::mapped_type M;
    uint64_t n;
    Read(stream, &n);
    if (n == 0) {
      return;
    }
    std::vector<std::pair<K, M> > vec;
    vec.reserve(n);
    while (n--) {
      vec.push_back({});
      Read(stream, &vec.back());
    }
    *target = Container<T, Args...>(vec.begin(), vec.end());
  }
};

// Meta-structure used to determine whether two types are equal.
template<typename T1, typename T2>
struct Equal {
  static const bool value = false;
};

// Meta-structure specialization for two equal types.
template<typename T>
struct Equal<T, T> {
  static const bool value = true;
};

template<template<typename T, typename...> class Container,
         typename T, typename... Args>
void Read(std::istream& stream, Container<T, Args...>* target) {  // NOLINT
  return Reader<!Equal<T, typename Container<T, Args...>::value_type>::value,
                Container, T, Args...>::_Read(stream, target);
}

// Writes the given container to the stream.
template<template <typename...> class Container, typename... Args>
void Write(const Container<Args...>& target, std::ostream& stream);  // NOLINT

// Writes the given value to the stream.
template<typename T>
void Write(const T& target, std::ostream& stream) {  // NOLINT
  const T net_target = ToNetworkFormat(target);
  stream.write(reinterpret_cast<const char*>(&net_target), sizeof(T));
}

// Writes the given string to the stream.
inline void Write(const std::string& target, std::ostream& stream) {  // NOLINT
  uint64_t size = target.size();
  Write(size, stream);
  stream.write(target.c_str(), size);
}

// Writes the given pair to the stream.
template<typename T1, typename T2>
void Write(const std::pair<T1, T2>& target, std::ostream& stream) {  // NOLINT
  Write(target.first, stream);
  Write(target.second, stream);
}

template<template <typename...> class Container, typename... Args>
void Write(const Container<Args...>& target, std::ostream& stream) {  // NOLINT
  const uint64_t n = target.size();
  Write(n, stream);
  for (const auto& e: target) {
    Write(e, stream);
  }
}

}  // namespace io
}  // namespace flow
#endif  // SRC_IO_SERIALIZE_H_
