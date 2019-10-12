#ifndef VERIFIER_H
#define VERIFIER_H

#include "dump.h"

template <typename T>
class Verifier;

/// Create Verifier from struct T.
template <typename T>
Verifier<T> MakeVerifier(const T& object_,
                         const std::string& hat,
                         std::ostream& out) {
// Let MakeVerifier deduce template patameter T itself.
  return Verifier<T>(object_, hat, out);
}

/**
 * VERIFIED( 'std::ostream' out)
 *
 * While used in a class method this macro dump info-hat and calls user_defined
 * prototypes:
 *
 * // Return true if class is in good state and false otherwise.
 * bool OK();
 *
 * and
 *
 * // Dump into `out` stream. `is_ok` is a result of OK() call.
 * void Dump(std::ostream& out, bool is_ok);
 */
#define VERIFIED(out) MakeVerifier(*this, HAT(), out);

/**
 * @brief Call `OK()` method of captured object
 * @brief during contruction and destruction and call its `Dump()` methods.
 */
template <class T>
class Verifier {
  public:

  Verifier(const T& object_, const std::string& hat, std::ostream& out)
      : object_(object),
        hat_(hat),
        out_(out) {

    Check();
  }

  ~Verifier() {
    Check();
  }

  private:
    const T& object_;
    const std::string& hat_;
    std::ostream& out_;

    void Check() {

      std::string message = "";

#if DUMP_LEVEL >= FAILURES_ONLY
      if (!object_->OK(message)) {
        out_ << BEGIN_LINE << '\n';
        out_ << Error(hat_) << '\n';
        if (message != "") { out_ << "Error: " << message << '\n'; }
        object_->Dump(out_, false);
        out_ << END_LINE << '\n';
      }
#if DUMP_LEVEL >= DUMP_ALL
      else {
        out_ << BEGIN_LINE << '\n';
        out_ << Paint(hat_, CYAN) << '\n';
        if (message != "") { out_ << "Warning: " << message << '\n'; }
        object_->Dump(out_, true);
        out_ << END_LINE << '\n';
      }
#endif
#endif
    }
};

#endif // VERIFIER_H

