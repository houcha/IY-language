#ifndef CANARY_H
#define CANARY_H


#define RANDOM_INT 12345                                                          // How to get compile-time random int?

/**
 * @brief Signal if storing value is changed.
 */
class Canary {

  private:

    int value_;
    constexpr static int ok_key = RANDOM_INT + 10000;

  public:

    Canary() : value_(ok_key) {}

    // Do not copy canaries.
    Canary(const Canary&) = delete;
    Canary(Canary&&)      = delete;

    bool OK() const { return value_ == ok_key; }

  private:

    friend std::ostream& operator<<(std::ostream& out, const Canary& canary);
};

std::ostream& operator<<(std::ostream& out, const Canary& canary) {
  out << canary.value_;
}


#endif // CANARY_H

