#ifndef CANARY_H
#define CANARY_H

// How to get compile-time random int?
#define RANDOM_INT 12345

/**
 * @brief Signal if storing value is changed.
 */
class Canary {
  public:

    Canary() : value_(ok_key) {}

    bool OK() const { return value_ == ok_key; }

  private:

    int value_;

    constexpr static int ok_key = RANDOM_INT + 10000;
};

#endif // CANARY_H

