#pragma once

#include <cstring>
#include <exception>
#include <string_view>

class BaseException : public std::exception {
protected:
  static constexpr size_t sizebuff = 256;
  char errormsg[sizebuff]{};

public:
  BaseException() noexcept = default;
  ~BaseException() override = default;

  explicit BaseException(std::string_view msg) noexcept {
    const auto len = std::min(msg.size(), sizebuff - 1);
    std::memcpy(errormsg, msg.data(), len);
    errormsg[len] = '\0';
  };
  const char *what() const noexcept override { return errormsg; };
};

class InputException : public BaseException {
public:
  InputException() noexcept : BaseException("InputException") {}

  explicit InputException(std::string_view msg) noexcept : BaseException(msg) {}
};

class NoStrategyException : public BaseException {
public:
  NoStrategyException() noexcept : BaseException("NoStrategyException") {}

  explicit NoStrategyException(std::string_view msg) noexcept
      : BaseException(msg) {}
};