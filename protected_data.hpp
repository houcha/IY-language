#ifndef PROTECTED_DATA_HPP
#define PROTECTED_DATA_HPP

#include <cstdint>
#include <utility>
#include "canary.hpp"
#include "poison.hpp"
#include "dumptools.hpp"


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

   ProtectedData& operator=(const ProtectedData& other);
   ProtectedData& operator=(ProtectedData&&      other);

  ~ProtectedData() = default;

  // The method is overridden in derived classes.
  const Canary& GetRCanary() const;
};



//==============================================================================
template <typename T>
struct DynamicProtectedData : ProtectedData<T, 0> {

   DynamicProtectedData(size_t capacity);
   DynamicProtectedData(const DynamicProtectedData& other);
   DynamicProtectedData(DynamicProtectedData&&      other);

   DynamicProtectedData& operator=(const DynamicProtectedData& other);
   DynamicProtectedData& operator=(DynamicProtectedData&&      other);

  ~DynamicProtectedData();

  // Delete ordinary new operator, for it works incorrect with this data.
  static void* operator new(size_t size) = delete;
  // Make a new specialization.
  static void* operator new(size_t size, size_t capacity);
  // Trivial delete.
  static void  operator delete(void* ptr);                                    //Does it call D-tor on buffer_[1] .. buffer_[n]?

  void  CreateRCanary();
  const Canary& GetRCanary() const;
};



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

   StaticProtectedData& operator=(const StaticProtectedData& other);
   StaticProtectedData& operator=(StaticProtectedData&&      other);

  ~StaticProtectedData() { delete dynamic_; }

  const Canary& GetRCanary() const { return r_canary_; }
};


#include "protected_data.inl"

#endif // PROTECTED_DATA_H

