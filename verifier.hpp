/**
 * @file verifier.h
 * @date 2019-09-29
 *
 * @brief This file contains Verifier.
 *
 * To enable dumping for your class, define next methods:
 *
 * // Return true if class is in good state and false otherwise.
 * // Message is an extra message which will be dumped with state.
 * bool OK(std::string& message);
 *
 * and
 *
 * // Dump into `out` stream. `is_ok` is a result of OK() call.
 * void Dump(std::ostream& out, bool is_ok);
 *
 */



#ifndef VERIFIER_H
#define VERIFIER_H

#include "dumptools.hpp"


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


template <typename T>
class Verifier;

template <typename T>
Verifier<T> MakeVerifier(const T& object,
                         const std::string& hat,
                         std::ostream& out);

#define VERIFIED() auto verifier = MakeVerifier(*this, HAT(), dump_stream);

#define ENABLE_VERIFICATION(stream)                                            \
  std::ostream& dump_stream = stream;                                          \
  template <class U> friend class Verifier;

#define ENABLE_VERIFICATION(stream)                                            \
  std::ostream& dump_stream = stream;                                          \
  template <class U> friend class Verifier;

/// Create Verifier from struct T.
// Let MakeVerifier deduce template patameter T itself.
template <typename T>
Verifier<T> MakeVerifier(const T& object,
                         const std::string& hat,
                         std::ostream& out) {
  return Verifier<T>(object, hat, out);
}

/**
 * @brief Call `OK()` method of captured object
 * @brief during contruction and destruction and call its `Dump()` methods.
 */
template <class T>
class Verifier {

  private:

    T& object_;
    const std::string hat_;
    std::ostream& out_;

  public:

     Verifier(const T& object, const std::string& hat, std::ostream& out);
    ~Verifier() { Check("End"); }

  private:

    void Check(const std::string& additional_message);
};

template <class T>
Verifier<T>::Verifier(const T& object,
                      const std::string& hat,
                      std::ostream& out)
      // const_cast lets us dump const objects.
    : object_(const_cast<T&>(object)),
      hat_   (hat),
      out_   (out) {

  Check("Begin");
}

template <class T>
void Verifier<T>::Check(const std::string& additional_message) {

  std::string message = "";

#if DUMP_LEVEL >= FAILURES_ONLY
  if (!object_.OK(message)) {
    out_ << BEGIN_LINE  << '\n';
    out_ << Error(hat_) << '\n';
    out_ << additional_message << '\n';

    if (message != "") { out_ << "Error: " << Error(message) << '\n'; }
    out_ << '\n';

    object_.Dump(out_, false);
    out_ << END_LINE << '\n';
  }
#if DUMP_LEVEL >= DUMP_ALL
  else {
    out_ << BEGIN_LINE        << '\n';
    out_ << Paint(hat_, CYAN) << '\n';
    out_ << additional_message << '\n';

    if (message != "") { out_ << "Warning: " << Warning(message) << '\n'; }
    out_ << '\n';

    object_.Dump(out_, true);
    out_ << END_LINE << '\n';
  }
#endif
#endif
}


#endif // VERIFIER_H

