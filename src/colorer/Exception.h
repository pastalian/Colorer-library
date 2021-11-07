#ifndef COLORER_EXCEPTION_H
#define COLORER_EXCEPTION_H

#include <exception>
#include "colorer/Common.h"

/** Exception class.
    Defines throwable exception.
    @ingroup common
*/
class Exception : public std::exception
{
 public:
  explicit Exception(const char* msg) noexcept;
  explicit Exception(const UnicodeString& msg) noexcept;

  ~Exception() noexcept override = default;

  Exception(const Exception&) = delete;
  Exception& operator=(const Exception& e) = delete;
  Exception(Exception&&) = delete;
  Exception& operator=(Exception&&) = delete;

  /** Returns exception message
   */
  [[nodiscard]] const char* what() const noexcept override;

 protected:
  std::string what_str;
};

/**
    InputSourceException is thrown, if some IO error occurs.
    @ingroup common
*/
class InputSourceException : public Exception
{
 public:
  explicit InputSourceException(const UnicodeString& msg) noexcept;
};

#endif  // COLORER_EXCEPTION_H
