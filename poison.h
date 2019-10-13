#ifndef POISON_H
#define POISON_H


// TODO: expand types range.
template <typename T>
T Poison() {
  return ~0;
}


#endif // POISON_H

