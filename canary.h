#ifndef CANARY_H
#define CANARY_H


class Canary;

/// Count Canary OK key.
int GetCanaryOkKey(const Canary* canary) {
  // Count according to canary address.
  std::srand(reinterpret_cast<uint64_t>(canary));
  return std::rand();
}

/**
 * @brief Signal if storing value is changed.
 */
class Canary {

  private:

    int value_;

  public:

    Canary() : value_(GetCanaryOkKey(this)) {}

    // Do not copy canaries.
    Canary(const Canary&) = delete;
    Canary(Canary&&)      = delete;

    bool OK() const { return value_ == GetCanaryOkKey(this); }

  private:

    friend std::ostream& operator<<(std::ostream& out, const Canary& canary);
};

std::ostream& operator<<(std::ostream& out, const Canary& canary) {
  out << canary.value_;
}


#endif // CANARY_H

