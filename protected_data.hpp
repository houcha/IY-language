#ifndef PROTECTED_DATA_HPP
#define PROTECTED_DATA_HPP

#include <cstdint>
#include <utility>
#include "canary.h"
#include "poison.h"
#include "dumptools.h"


template <typename T>
class DynamicStack;

template <typename T, size_t N>
class StaticStack;

//==============================================================================
template <typename T, size_t N>
struct ProtectedData {

  typedef size_t   size_type;
  typedef uint64_t hash_type;

  /// Left canary.
  Canary l_canary_;

  /// Hash of all elements. Used to checks buffer validity.
  hash_type hash_;

  /// Current number of elements.
  size_type size_;

  /// Max number of elements.
  size_type capacity_;

  T buffer_[N];

   ProtectedData(size_t capacity);
   ProtectedData(const ProtectedData& other);
   ProtectedData(ProtectedData&&      other);

  ~ProtectedData() = default;

  // The method is overridden in derived classes.
  const Canary& GetRCanary() const;
};
//.............................................................................
template <typename T, size_t N>
ProtectedData<T, N>::ProtectedData(size_t capacity)
    : l_canary_(),
      hash_(0),
      size_(0),
      capacity_(capacity) {

  for (int i = 0; i < capacity_; ++i) {
    buffer_[i] = Poison<T>();
  }
}

template <typename T, size_t N>
ProtectedData<T, N>::ProtectedData(const ProtectedData& other)
    : l_canary_(),
      hash_(0),
      size_(other.size_),
      capacity_(other.capacity_) {

  for (int i = 0; i < size_; ++i) {
    buffer_[i] = other.buffer_[i];
  }

  for (int i = size_; i < capacity_; ++i) {
    buffer_[i] = Poison<T>();
  }
}

template <typename T, size_t N>
ProtectedData<T, N>::ProtectedData(ProtectedData&& other)
    : l_canary_(),
      hash_(0),
      size_(other.size_),
      capacity_(other.capacity_) {

  for (int i = 0; i < size_; ++i) {
    buffer_[i] = std::move(other.buffer_[i]);                                         // Should it be move_if_noexcept?
  }

  for (int i = size_; i < capacity_; ++i) {
    buffer_[i] = Poison<T>();
  }
}



//==============================================================================
template <typename T>
struct DynamicProtectedData : ProtectedData<T, 0> {

   DynamicProtectedData(size_t capacity);
   DynamicProtectedData(const DynamicProtectedData& other);
   DynamicProtectedData(DynamicProtectedData&&      other);

  ~DynamicProtectedData() = default;                                         // Should it really be default? i.e. will it call destructor on buffer_[1] .. buffer_[n]?

  // Delete ordinary new operator, for it works incorrect with this data.
  static void* operator new(size_t size) = delete;
  // Make a new specialization.
  static void* operator new(size_t size, size_t capacity);
  // Trivial delete.
  static void  operator delete(void* ptr);                                    //Does it call D-tor on buffer_[1] .. buffer_[n]?

  void  CreateRCanary();
  const Canary& GetRCanary() const;
};
//.............................................................................
template <typename T>
DynamicProtectedData<T>::DynamicProtectedData(size_t capacity)
    : ProtectedData<T, 0>(capacity) {
  CreateRCanary();
}

template <typename T>
DynamicProtectedData<T>::DynamicProtectedData(const
                                              DynamicProtectedData& other)
    : ProtectedData<T, 0>(other) {
  CreateRCanary();
}

template <typename T>
DynamicProtectedData<T>::DynamicProtectedData(DynamicProtectedData&& other)
    : ProtectedData<T, 0>(std::move(other)) {
  CreateRCanary();
}

template <typename T>
void* DynamicProtectedData<T>::operator new(size_t size, size_t capacity) {
//               unused argument------------------^ (but it is to be)
  size_t full_size =
      sizeof(DynamicProtectedData<T>) + sizeof(T) * capacity + sizeof(Canary);

  return malloc(full_size);
}

template <typename T>
void DynamicProtectedData<T>::operator delete(void* ptr) {
  free(ptr);
}

template <typename T>
void DynamicProtectedData<T>::CreateRCanary() {
  // `this` is necessary to specialize scope while template inheritance.
  ::new (ProtectedData<T, 0>::buffer_ + ProtectedData<T, 0>::capacity_)
      Canary();
}

template <typename T>
const Canary& DynamicProtectedData<T>::GetRCanary() const {
  return *(Canary*)(
      ProtectedData<T, 0>::buffer_ + ProtectedData<T, 0>::capacity_);
}

template <typename T>
std::ostream& operator<<(std::ostream& out,
                         const DynamicProtectedData<T>& data) {
  // Print C-style array with PrintVecHelper from dumptools.h.
  // Name it as we want to see it in a dump.
  PrintVecHelper<T> buffer(data.buffer_, data.size_, data.capacity_);

  OPEN_SCOPE();
  DUMP_CONTAINER_STATE(out, &data, data.l_canary_,
                                   data.hash_,
                                   data.size_,
                                   data.capacity_,
                                   buffer);
  CLOSE_SCOPE();

  return out;
}



//==============================================================================
template <typename T, size_t N>
struct StaticProtectedData : ProtectedData<T, N> {

  //TODO: create ProtectedVector and make dynamic_ pointer to vector. Also
  // put all vector-methods into ProtectedVector and make BaseStack pure
  // wrap of container.
  /// When stack overflows, next elements are pushed to dynamic stack.
  DynamicStack<T>* dynamic_;

  /// Right canary.
  Canary r_canary_;

   StaticProtectedData();
   StaticProtectedData(const StaticProtectedData& other);
   StaticProtectedData(StaticProtectedData&&      other);

  ~StaticProtectedData() { delete dynamic_; }

  const Canary& GetRCanary() const { return r_canary_; }
};
//.............................................................................
template <typename T, size_t N>
StaticProtectedData<T, N>::StaticProtectedData()
    : ProtectedData<T, N>(N),
      dynamic_(nullptr),
      r_canary_() {}

template <typename T, size_t N>
StaticProtectedData<T, N>::StaticProtectedData(const
                                               StaticProtectedData& other)
    : ProtectedData<T, N>(other),
      // Deep copy of dynamic stack.
      dynamic_(new DynamicStack(*other.dynamic_)),
      r_canary_() {}

template <typename T, size_t N>
StaticProtectedData<T, N>::StaticProtectedData(StaticProtectedData&& other)                        // Does default move make the same?
    : ProtectedData<T, N>(std::move(other)),                                                       // Is std::move redundant?
      dynamic_(other.dynamic_),  // Copy pointer.
      r_canary_() {
  // Null moved pointer.
  other.dynamic_ = nullptr;
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& out,
                         const StaticProtectedData<T, N>& data) {
  // Print C-style array with PrintVecHelper from dumptools.h.
  // Name it as we want to see it in a dump.
  PrintVecHelper<T> buffer(data.buffer_, std::min(N, data.size_), N);

  OPEN_SCOPE();
  if (data.dynamic_ != nullptr) {
    DynamicProtectedData<T>& dynamic = *data.dynamic_->data();

    DUMP_CONTAINER_STATE(out, &data, data.l_canary_, data.hash_,
                                     data.size_,
                                     data.capacity_,
                                     buffer,
                                     dynamic,
                                     data.r_canary_);
  } else {
    DUMP_CONTAINER_STATE(out, &data, data.l_canary_, data.hash_,
                                     data.size_,
                                     data.capacity_,
                                     buffer,
                                     data.dynamic_,
                                     data.r_canary_);
  }
  CLOSE_SCOPE();

  return out;
}


#endif // PROTECTED_DATA_H

