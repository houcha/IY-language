/**
 * @file dumptools.h
 * @date 2019-09-29
 *
 * @brief There are tools for containers dumping.
 *
 */



#ifndef DUMPTOOLS_H
#define DUMPTOOLS_H



//============================ INCLUDES ========================================
#include <typeinfo>
#include <cxxabi.h>
#include <cassert>
#include <vector>
#include <string>
#include <cstring>
#include <ostream>
#include "color.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//============================ CONSTANTS =======================================
static const std::string BEGIN_LINE = "=======================================";
static const std::string END_LINE   = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
static const std::string INDENT = "|   ";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//======================== GLOBAL VARIABLES ====================================
static std::string cur_indent_gl = "";
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//============================== DECLARATIONS ==================================
std::string Paint  (const std::string& str, const std::string& color);
std::string Warning(const std::string& str);
std::string Error  (const std::string& str);

template <typename T>
struct PrintVecHelper;

template <typename T>
std::ostream& operator<<(std::ostream& out, const PrintVecHelper<T>& helper);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector);

template <typename T>
char* GetClassTypeStr(const T* obj);

template<typename H1>
std::ostream& DumpVariables(std::ostream& out, const char* label, H1&& value);

template<typename H1, typename ...T>
std::ostream& DumpVariables(std::ostream& out, const char* label,
                            H1&& value, T&&... rest);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//=============================== MACROS =======================================

// OPEN/CLOSE_SCOPE are macros but no functions to avoid writing evident
// std::ostream& out as an argument.

#define OPEN_SCOPE()                                                           \
  out << '\n';                                                                 \
  cur_indent_gl += INDENT;

#define CLOSE_SCOPE()                                                          \
  cur_indent_gl = std::string(                                                 \
      cur_indent_gl, 0, cur_indent_gl.size() - INDENT.size());                 \
  out << '\n' << cur_indent_gl;

#define HAT() std::string(__FILE__) + std::string(" : ") +                     \
              std::string(__func__) + std::string(" : ") +                     \
              std::to_string(__LINE__)

/// Dump container state (fields values) into out stream (e.g. file stream).
#define DUMP_CONTAINER_STATE(out, container_ptr, /* Fields to dump */ ...)     \
                                                                               \
  /* Header. */                                                                \
  char* class_name = GetClassTypeStr(container_ptr);                           \
  out << cur_indent_gl << class_name << " [" << container_ptr << "]";          \
  free(class_name);                                                            \
                                                                               \
  OPEN_SCOPE();                                                                \
                                                                               \
  /* Fields. */                                                                \
  DumpVariables(out, #__VA_ARGS__, __VA_ARGS__);                               \
                                                                               \
  CLOSE_SCOPE();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//============================== FUNCTIONS =====================================
/*
 * TODO: make function that gives FULL type (without removing const and &)
 * https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
 */

std::string Paint(const std::string& str, const std::string& color) {
  return color + str + RESET;
}

std::string Warning(const std::string& str) {
  return Paint(str, YELLOW);
}

std::string Error(const std::string& str) {
  return Paint(str, RED);
}

// C-style array seems to be tought printable, for its size goes apart.
// Also it is impossible to overload operator<< for C-style array because
// operator<< takes only 2 args. Thus make our own helper which every
// class can be convert to.

template <typename T>
struct PrintVecHelper {

  const T* data_;
  size_t size_;
  size_t capacity_;

  PrintVecHelper() = default;

  template <typename ValueType>
  PrintVecHelper(const ValueType* buffer, size_t size, size_t capacity)
      : data_(buffer),
        size_(size),
        capacity_(capacity) {}
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const PrintVecHelper<T>& helper) {

  OPEN_SCOPE();

  for (int i = 0; i < helper.capacity_; ++i) {
    char ch = (i < helper.size_) ? '*' : ' ';

    out << cur_indent_gl << ch << " [" << i << "] = " << helper.data_[i];

    // If element is last, do not enter new line after it.
    // CLOSE_SCOPE makes it for us.
    if (i != (helper.capacity_ - 1)) { out << '\n'; }
  }

  CLOSE_SCOPE();

  return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  return out << PrintVecHelper(vector.data(), vector.size(), vector.capacity());
}

template <typename T>
char* GetClassTypeStr(const T* obj) {
  int status;
  return abi::__cxa_demangle(typeid(*obj).name(), nullptr, nullptr, &status);
}

//https://stackoverflow.com/a/32230306
template<typename H1>
std::ostream& DumpVariables(std::ostream& out, const char* label, H1&& value) {
  return out << cur_indent_gl << label << " = " << std::forward<H1>(value);
}

template<typename H1, typename ...T>
std::ostream& DumpVariables(std::ostream& out, const char* label,
                            H1&& value, T&&... rest) {
  const char* pcomma = strchr(label, ',');
  out << cur_indent_gl;
  out.write(label, pcomma - label);
  return DumpVariables(out << " = " << std::forward<H1>(value) << '\n',
                       pcomma + 1 + 1, // Second +1 is to ommit space.
                       std::forward<T>(rest)...);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



#endif // DUMPTOOLS_H

