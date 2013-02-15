// Copyright 2012, 2013 Eugen Sawin <esawin@me73.com>
#ifndef SRC_TIME_CLOCK_H_
#define SRC_TIME_CLOCK_H_

#include <cstdint>
#include <ctime>
#include <string>
#include <sstream>

namespace flow {
namespace time {

// Type for time differences.
class ClockDiff {
 public:
  typedef int64_t ValueType;

  // Constants used to convert between different time units.
  static const typename ClockDiff::ValueType kSecInMin = 60;
  static const typename ClockDiff::ValueType kMilliInSec = 1000;
  static const typename ClockDiff::ValueType kMicroInMilli = 1000;
  static const typename ClockDiff::ValueType kNanoInMicro = 1000;
  static const typename ClockDiff::ValueType kMicroInSec = kMilliInSec *
      kMicroInMilli;
  static const typename ClockDiff::ValueType kMicroInMin = kMicroInSec *
      kSecInMin;
  static constexpr double kMilliInMicro = 1.0 / kMicroInMilli;
  static constexpr double kMicroInNano = 1.0 / kNanoInMicro;
  static constexpr double kSecInMicro = 1.0 / kMicroInSec;
  static constexpr double kMinInMicro = 1.0 / kMicroInMin;

  // Defaul constructor.
  ClockDiff()  // NOLINT
      : value_() {}

  // Implicit constructor.
  ClockDiff(const ValueType& value)  // NOLINT
      : value_(value) {}

  // Self-assignment addition.
  ClockDiff& operator+=(const ClockDiff& rhs) {
    value_ += rhs.value_;
    return *this;
  }

  // Self-assignment subtraction.
  ClockDiff& operator-=(const ClockDiff& rhs) {
    value_ -= rhs.value_;
    return *this;
  }

  // Addition.
  ClockDiff operator+(const ClockDiff& rhs) const {
    return ClockDiff(value_ + rhs.value_);
  }

  // Subtraction.
  ClockDiff operator-(const ClockDiff& rhs) const {
    return ClockDiff(value_ - rhs.value_);
  }

  // Division.
  ClockDiff operator/(const ClockDiff& rhs) const {
    return ClockDiff(value_ / rhs.value_);
  }

  // Multiplication.
  ClockDiff operator*(const ClockDiff& rhs) const {
    return ClockDiff(value_ * rhs.value_);
  }

  // Returns the string representation.
  std::string Str() const {
    std::stringstream ss;
    ss.setf(std::ios::fixed, std::ios::floatfield);
    ss.precision(2);
    if (value_ >= kMicroInMin) {
      const double min = value_ * kMinInMicro;
      ss << min << "min";
    } else if (value_ >= kMicroInSec) {
      const double sec = value_ * kSecInMicro;
      ss << sec << "s";
    } else if (value_ >= kMicroInMilli) {
      const double milli = value_ * kMilliInMicro;
      ss << milli << "ms";
    } else {
      ss << value_ << "µs";
    }
    return ss.str();
  }

  // Returns the integral value of the diff (microseconds).
  const ValueType& value() const {
    return value_;
  }

 private:
  ValueType value_;
};

// High-resolution clock used for performance measuring.
// Use this until you have C++11 support via chrono.
template<uint8_t _ClockType = CLOCK_MONOTONIC>
class BasicClock {
 public:
  typedef ClockDiff Diff;

  // Clock types used to switch between process, thread and wallclock time.
  enum Type {
    kProcessCpuTime = CLOCK_PROCESS_CPUTIME_ID,
    kThreadCpuTime = CLOCK_THREAD_CPUTIME_ID,
    kRealMonotonic = CLOCK_MONOTONIC,
  };

  // Initializes the clock with given type and the current clock time.
  BasicClock() {
    clock_gettime(_ClockType, &time_);
  }

  // Returns the time difference in microseconds between this and the given
  // clock's time.
  Diff operator-(const BasicClock& rhs) const {
    return Diff((time_.tv_sec - rhs.time_.tv_sec) * Diff::kMicroInSec +
                (time_.tv_nsec - rhs.time_.tv_nsec) * Diff::kMicroInNano);
  }

  // Returns the system time resolution.
  // Remark: Usually returns 0µs (1ns), this is however a bad promise and does
  // not reflect the (dynamic) underlying clock event resolution.
  static Diff Resolution(Type type) {
    timespec res;
    clock_getres(type, &res);
    return Diff(res.tv_sec * Diff::kMicroInSec + res.tv_nsec *
                Diff::kMicroInNano);
  }

 private:
  timespec time_;
};

typedef BasicClock<> Clock;
typedef BasicClock<CLOCK_PROCESS_CPUTIME_ID> ProcessClock;
typedef BasicClock<CLOCK_THREAD_CPUTIME_ID> ThreadClock;

// Stream output operator overload for Diff.
inline std::ostream& operator<<(std::ostream& stream, const ClockDiff& diff) {
  return stream << diff.Str();
}

}  // namespace time
}  // namespace flow
#endif  // SRC_TIME_CLOCK_H_
