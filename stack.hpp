#ifndef STACK_HPP
#define STACK_HPP

#define DUMP_LEVEL DUMP_ALL

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <utility>
#include <string>
#include <string_view>
#include "canary.h"
#include "poison.h"
#include "verifier.h"
#include "dumptools.h"
#include "protected_data.hpp"


//==============================================================================
template <typename T, class Container>
class BaseStack {

    ENABLE_VERIFICATION(std::cout);

  protected:

    Canary l_canary_;
    Container* data_;
    Canary r_canary_;

  public:

    typedef typename Container::size_type size_type;
    typedef typename Container::hash_type hash_type;

     /// Capture container pointer.
     BaseStack(Container* container);
     BaseStack(BaseStack&& other);
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ WARNING ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     /// Base copy constructor realization depends on Container type.
     /// (Because container creation depends on Container type)
     /// So it is deleted in order to avoid unexpected behaviour.
     BaseStack(const BaseStack& other) = delete;
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Do not delete captured data pointer.
    ~BaseStack() = default;

    // Capacity methods.
    virtual bool      empty()    const { return size() == 0; }
    virtual size_type size()     const = 0;
    virtual size_type capacity() const = 0;

    // Elements access.
          T& top();
    const T& top() const;

    // Modifiers.
    void push(const T& value);
    void emplace(T&& value);
    void pop();

    // Dump.
    void SetDumpStream();

  protected:

    virtual void IncrementSize() const = 0;
    virtual void DecrementSize() const = 0;

    /* Getters. */

    hash_type&    GetHash()        const { return data_->hash_;        }
    const Canary& GetDataLCanary() const { return data_->l_canary_;    }
    const Canary& GetDataRCanary() const { return data_->GetRCanary(); }

    /* Change state. */

    void Update();
    hash_type CalculateHash() const;

    /* Validity. */

    bool CheckPoison()                       const;
    /// Check stack state. `message` is error/warning message.
    bool OK(std::string& message)            const;
    void Dump(std::ostream& out, bool is_ok) const;

    /* Virtual methods. */

    virtual void     Reallocate()           = 0;
    virtual       T& Get(size_type i)       = 0;
    virtual const T& Get(size_type i) const = 0;
};
//.............................................................................
template <typename T, class Container>
BaseStack<T, Container>::BaseStack(Container* container)
    : l_canary_(),
      data_(container),
      r_canary_() {}

template <typename T, class Container>
BaseStack<T, Container>::BaseStack(BaseStack&& other)
    : l_canary_(),
      data_(other.data_),
      r_canary_() {

  other.data_ = nullptr;
}

template <typename T, class Container>
void BaseStack<T, Container>::Update() {
  // Update hash.
  GetHash() = CalculateHash();
}

template <typename T, class Container>
bool BaseStack<T, Container>::CheckPoison() const {

  for (int i = size(); i < capacity(); ++i) {
    if (!IsPoison(Get(i))) {
      return false;
    }
  }

  return true;
}

template <typename T, class Container>
bool BaseStack<T, Container>::OK(std::string& message) const {

  message = "";

  if (size() < 0) {
    message = "size < 0";
    return false;
  }

  if (capacity() < 0) {
    message = "capacity < 0";
    return false;
  }

  if (capacity() < size()) {
    message = "capacity < size";
    return false;
  }

  if (!l_canary_.OK()) {
    message = "Left stack ptr canary is broken";
    return false;
  }

  if (!r_canary_.OK()) {
    message = "Right stack ptr canary is broken";
    return false;
  }

  if (!GetDataLCanary().OK()) {
    message = "Left data canary is broken";
    return false;
  }

  if (!GetDataRCanary().OK()) {
    message = "Right data canary is broken";
    return false;
  }

  if (GetHash() != CalculateHash()) {                                               // GetHash is not const
    message = "Wrong hash";
    return false;
  }

  if (!CheckPoison()) {
    message = "Non-poison value has been come across in capacity area";
    return false;
  }

  return true;
}

template <typename T, class Container>
void BaseStack<T, Container>::Dump(std::ostream& out, bool is_ok) const {
  // See dumptools.h.
  DUMP_CONTAINER_STATE(out, this, l_canary_,
                                  *data_,
                                  r_canary_);
}

template <typename T, class Container>
typename BaseStack<T, Container>::hash_type
BaseStack<T, Container>::CalculateHash() const {
  // Hash buffer.
  return std::hash<std::string_view>{}(std::string_view
      // &Get(0) is a pointer to buffer begin.
      ((const char*)&Get(0), sizeof(T) * capacity()));
}

template <typename T, class Container>
T& BaseStack<T, Container>::top() {
  assert(size() > 0);

  return Get(size() - 1);
}

template <typename T, class Container>
const T& BaseStack<T, Container>::top() const {
  assert(size() > 0);

  return Get(size() - 1);
}

template <typename T, class Container>
void BaseStack<T, Container>::push(const T& value) { VERIFIED();

  if (capacity() <= size()) {
    Reallocate();
  }

  Get(size()) = value;
  IncrementSize();

  Update();
}

template <typename T, class Container>
void BaseStack<T, Container>::pop() { VERIFIED();

  // Do nothing when pop empty stack.
  if (empty()) return;

  top() = Poison<T>();
  DecrementSize();

  Update();
}



//==============================================================================
template <typename T>
class DynamicStack : public BaseStack<T, DynamicProtectedData<T>> {

    ENABLE_VERIFICATION(std::cout);

    typedef BaseStack<T, DynamicProtectedData<T>> Base;

  public:

    typedef typename Base::size_type size_type;
    typedef typename Base::hash_type hash_type;

     /// If no capacity is specified, create Stack with capacity = 1.
     DynamicStack(size_t capacity = 1);
     DynamicStack(const DynamicStack& other);
     DynamicStack(DynamicStack&&      other);

    // Delete allocated data.
    ~DynamicStack() { delete Base::data_; }

    size_type size()     const { return Base::data_->size_;     }
    size_type capacity() const { return Base::data_->capacity_; }

    // Returns container pointer.
    DynamicProtectedData<T>* data() const { return Base::data_; }

  protected:

    void IncrementSize() const { ++Base::data_->size_; }
    void DecrementSize() const { --Base::data_->size_; }

    // Used in StaticStack::Get(i) implementation.
    template <typename U, size_t N>
    friend class StaticStack;

    void DoubleCapacity() const { this->data_->capacity_ *= 2; }

    /* Overridden methods. */

    void     Reallocate()           override;
          T& Get(size_type i)       override;
    const T& Get(size_type i) const override;
};
//.............................................................................

template <typename T>
DynamicStack<T>::DynamicStack(size_t capacity)
    : Base(new (capacity) DynamicProtectedData<T>(capacity)) {
  // We cannot place Update() in Base class, for it uses abstract method Get(i).
  Base::Update();
}

template <typename T>
DynamicStack<T>::DynamicStack(const DynamicStack& other) {
  Base::data_ = new (other.capacity()) DynamicProtectedData(other.data_);

  Base::Update();
}

template <typename T>
DynamicStack<T>::DynamicStack(DynamicStack&& other)
    : Base(std::move(other)) {

  Base::Update();
}

template <typename T>
void DynamicStack<T>::Reallocate() {
  DoubleCapacity();

  DynamicProtectedData<T>* old = Base::data_;
  Base::data_ = new (capacity()) DynamicProtectedData<T>(*old);
  delete old;
}

template <typename T>
T& DynamicStack<T>::Get(size_type i) {
  return Base::data_->buffer_[i];
}

template <typename T>
const T& DynamicStack<T>::Get(size_type i) const {
  return Base::data_->buffer_[i];
}



//==============================================================================
template <typename T, size_t N>
class StaticStack : protected StaticProtectedData<T, N>,
                    public    BaseStack<T, StaticProtectedData<T, N>> {

    ENABLE_VERIFICATION(std::cout);

    typedef BaseStack<T, StaticProtectedData<T, N>> Base;

  public:

    typedef typename Base::size_type size_type;

     StaticStack();
     StaticStack(const StaticStack& other);
     StaticStack(StaticStack&&      other);

    ~StaticStack() = default;

    size_type size()     const;
    size_type capacity() const;

  protected:

    /* Overridden methods. */

    void IncrementSize() const;
    void DecrementSize() const;

    bool OK(std::string& message) const;

    void     Reallocate()           override;
          T& Get(size_type i)       override;
    const T& Get(size_type i) const override;
};
//.............................................................................

template <typename T, size_t N>
StaticStack<T, N>::StaticStack()
    : StaticProtectedData<T, N>(),
      // There is a hack: we cast pointer from a left data canary to
      // a StaticProtectedData (get pointer to the data begin)
      // and consider it is an allocated container given to a Base.
      // It let us treat Static and Dynamic data the same way.
      Base(static_cast<StaticProtectedData<T, N>*>(this)) {

  Base::Update();
}

template <typename T, size_t N>
StaticStack<T, N>::StaticStack(const StaticStack& other)
    : StaticProtectedData<T, N>(other),
      Base(static_cast<StaticProtectedData<T, N>*>(this)) {

  Base::Update();
}

template <typename T, size_t N>
StaticStack<T, N>::StaticStack(StaticStack&& other)
    : StaticProtectedData<T, N>(std::move(other)),
      Base(static_cast<StaticProtectedData<T, N>*>(this)) {

  Base::Update();
}

template <typename T, size_t N>
typename StaticStack<T, N>::size_type
StaticStack<T, N>::size() const {
  return this->dynamic_ == nullptr ?
      this->data_->size_ : this->data_->size_ + this->dynamic_->size();
}

template <typename T, size_t N>
typename StaticStack<T, N>::size_type
StaticStack<T, N>::capacity() const {
  return this->dynamic_ == nullptr ? N : N + this->dynamic_->capacity();
}

template <typename T, size_t N>
void StaticStack<T, N>::IncrementSize() const {
  if (size() < N) {
    ++this->data_->size_;
  } else {
    this->dynamic_->IncrementSize();
  }
}

template <typename T, size_t N>
void StaticStack<T, N>::DecrementSize() const {
  if (size() <= N) {
    --this->data_->size_;
  } else {
    this->dynamic_->DecrementSize();
  }
}

template <typename T, size_t N>
void StaticStack<T, N>::Reallocate() {

  if (this->dynamic_ == nullptr) {
    this->dynamic_ = new DynamicStack<T>();
  } else {
    this->dynamic_->Reallocate();
  }
}

template <typename T, size_t N>
bool StaticStack<T, N>::OK(std::string& message) const {
  return this->dynamic_ == nullptr ?
      Base::OK() : Base::OK() && this->dynamic_->OK();
}

template <typename T, size_t N>
T& StaticStack<T, N>::Get(size_type i) {
  // If static capacity < element index then element is located in dynamic_.
  return (i < N) ? this->buffer_[i] : this->dynamic_->Get(i - N);
}

template <typename T, size_t N>
const T& StaticStack<T, N>::Get(size_type i) const {
  // If static capacity < element index then element is located in dynamic_.
  return (i < N) ? this->buffer_[i] : this->dynamic_->Get(i - N);
}


#endif // STACK_HPP

