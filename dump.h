/**
 * @file dump.h
 * @date 2019-09-29
 *
 * @brief There are tools for containers and adapters dumping.
 *
 * Assume we want to enable dumping for class A which is an adapter:
 *
 *   struct A {
 *     std::vector<int> my_buffer;
 *
 *     A(int n) : my_buffer(n, 0) {}
 *
 *     void add_element(int elenemt) {
 *       my_buffer.push_back(element);
 *     }
 *   };
 *
 * Suppose we also want to dump its state every `add_element` method call using
 * this 'library'.
 *
 * First thing we should do is to define `buffer_` field (which is container)
 * like:
 *
 *   struct A {
 *
 *
 * Then create `bool OK()` and `void Dump()` methods.
 *
 * OK() returns ture if container is in good state (i.e. data is correct,
 * there is no memory leaks and etc.) and false otherwise.
 *
 * Dump(bool is_failure) should dump any info in file. If is_failure is true,
 * error is printed into file.
 *
 */



#ifndef DUMP_H
#define DUMP_H



//============================ DEFINES =========================================
/*
 * Dumping can be activated by assigning appropriate dump level from define-list
 * below to DUMP_LEVEL.
 * For instance, next string:
 *
 *    #define DUMP_LEVEL FAILURES_ONLY
 *
 * activates failures (and only failures) dumping.
 */

#define NO_DUMP       0
#define FAILURES_ONLY 1
#define DUMP_ALL      2
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//============================ INCLUDES ========================================
#include <typeinfo>
#include <cxxabi.h>
#include <cassert>
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



//====================== FUNCTIONS DECLARATIONS ================================
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
#define OPEN_SCOPE()  \
  out << '\n'; /*<< cur_indent_gl << '{' << '\n';*/ \
  cur_indent_gl += INDENT;

#define CLOSE_SCOPE()  \
  cur_indent_gl = std::string(  \
      cur_indent_gl, 0, cur_indent_gl.size() - INDENT.size()); \
  out << '\n' << cur_indent_gl; //<< '}' << '\n';

#define HAT() std::string(__FILE__) + std::string(" : ") +\
              std::string(__func__) + std::string(" : ") +\
              std::to_string(__LINE__)

// Dump container state (fields values) into out stream (e.g. file stream).
#define DUMP_CONTAINER_STATE(out, container_ptr, ...)                               \
                                                                            \
  /* Header. */                                                                 \
  char* class_name = GetClassTypeStr(container_ptr);                            \
  out << cur_indent_gl << class_name << " [" << container_ptr << "]";   \
  free(class_name);                                                           \
                                                          \
  OPEN_SCOPE(); \
                                                          \
  /* Fields. */\
  DumpVariables(out, #__VA_ARGS__, __VA_ARGS__);           \
                                                                            \
  CLOSE_SCOPE();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//============================== FUNCTIONS =====================================
/*
 * TODO: make function that gives FULL type (without removing const and &)
 * https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c
 */

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vector) {
  size_t capacity = vector.capacity();
  size_t size = vector.size();

  OPEN_SCOPE();
  for (int i = 0; i < capacity; ++i) {
    char ch = ' ';
    if (i < size) {
      ch = '*';
    }
    out << cur_indent_gl << ch << " [" << i << "] = " << vector[i];
    // If element is last, do not enter new line after it.
    // CLOSE_SCOPE makes it for us.
    if (i != (capacity - 1)) {
      out << '\n';
    }
  }
  CLOSE_SCOPE();

  return out;
}

template <typename T>
char* GetClassTypeStr(const T* obj) {
  int status;
  return abi::__cxa_demangle(typeid(*obj).name(), nullptr, nullptr, &status);
}

std::string Paint(const std::string& str,
                  const std::string& color) {
  return color + str + RESET;
}

std::string Warning(const std::string& str) {
  return Paint(str, YELLOW);
}

std::string Error(const std::string& str) {
  return Paint(str, RED);
}

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



#endif // DUMP_H

