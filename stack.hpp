#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <utility>
#include <string>
#include <string_view>
#include "canary.h"
#include "poison.h"

template <typename T>
class DynamicStack;

template <typename T, int N>
class StaticStack;

                                                                        // How to move it to another hpp? DynamicStack hinders.
template <typename T, int N>
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

template <typename T, int N>
ProtectedData<T, N>::ProtectedData(size_t capacity)
    : l_canary_(),
      hash_(0),
      size_(0),
      capacity_(capacity) {

  for (int i = 0; i < capacity_; ++i) {
    buffer_[i] = Poison<T>();
  }
}

template <typename T, int N>
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

template <typename T, int N>
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

template <typename T>
struct DynamicProtectedData : ProtectedData<T, 0> {

   DynamicProtectedData(size_t capacity);
   DynamicProtectedData(const DynamicProtectedData& other);
   DynamicProtectedData(DynamicProtectedData&&      other);

  ~DynamicProtectedData() = default;                                                    // Should it really be default?

  // Delete ordinary new operator, for it works incorrect with this data.
  static void* operator new(size_t size) = delete;
  // Make a new specialization.
  static void* operator new(size_t size, size_t capacity);
  // Trivial delete.
  static void  operator delete(void* ptr);                                    //Does it call D-tor on buffer_[1] .. buffer_[n]?

  void  CreateRCanary();
  const Canary& GetRCanary() const;
};

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
  ::new (this->buffer_ + this->capacity_) Canary();
}

template <typename T>
const Canary& DynamicProtectedData<T>::GetRCanary() const {
  return *(Canary*)(this->buffer_ + this->capacity_);
}

template <typename T, int N>
struct StaticProtectedData : ProtectedData<T, N> {

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

template <typename T, int N>
StaticProtectedData<T, N>::StaticProtectedData()
    : ProtectedData<T, N>(N),
      dynamic_(nullptr),
      r_canary_() {}

template <typename T, int N>
StaticProtectedData<T, N>::StaticProtectedData(const
                                               StaticProtectedData& other)
    : ProtectedData<T, N>(other),
      // Deep copy of dynamic stack.
      dynamic_(new DynamicStack(*other.dynamic_)),
      r_canary_() {}

template <typename T, int N>
StaticProtectedData<T, N>::StaticProtectedData(StaticProtectedData&& other)                        // Does default move make the same?
    : ProtectedData<T, N>(std::move(other)),                                                       // Is std::move redundant?
      dynamic_(other.dynamic_),  // Copy pointer.
      r_canary_() {
  // Null moved pointer.
  other.dynamic_ = nullptr;
}



//==============================================================================

template <typename T, class Container>
class BaseStack {

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

    // Elements access.
          T& top()       { return Get(size() - 1); }
    const T& top() const { return Get(size() - 1); }

    // Capacity methods.
    bool      empty()    const { return size() == 0;   }
    size_type size()     const { return GetSize();     }
    size_type capacity() const { return GetCapacity(); }

    // Modifiers.
    void push(const T& value);
    void emplace(T&& value);
    void pop();

  protected:

    /* Getters. */

    hash_type& GetHash()     const { return data_->hash_;     }
    size_type& GetSize()     const { return data_->size_;     }
    size_type& GetCapacity() const { return data_->capacity_; }

    const Canary& GetDataLCanary() const { return data_->l_canary_;    }
    const Canary& GetDataRCanary() const { return data_->GetRCanary(); }

    /* Change state. */

    void Update();

    hash_type CalculateHash() const;

    /// Return size of full [left_canary, hash, ..., right_canary] data.
    size_type DataSizeInBytes() const;

    /* Validity. */

    /// Check stack state. `message` is error/warning message.
    bool OK(std::string& message) const;
    bool CheckPoison()            const;
    void Dump(bool is_ok)         const;

    /* Overridden methods. */

    virtual void Reallocate() = 0;

    /// Return element with index `i`;
    virtual       T& Get(size_type i) = 0;
    virtual const T& Get(size_type i) const = 0;
};

//.............................................................................

template <typename T, class Container>
BaseStack<T, Container>::BaseStack(Container* container)
    : l_canary_(),
      data_(container),
      r_canary_() {

  Update();
}

template <typename T, class Container>
BaseStack<T, Container>::BaseStack(BaseStack&& other)
    : l_canary_(),
      data_(other.data_),
      r_canary_() {

  other.data_ = nullptr;

  Update();
}

template <typename T, class Container>
typename BaseStack<T, Container>::size_type
BaseStack<T, Container>::DataSizeInBytes() const {
  // +1 to count the very r_canary_
  return (char*)(&GetDataRCanary() + 1) - (char*)(&GetDataLCanary());
}

template <typename T, class Container>
void BaseStack<T, Container>::Update() {
  // Update hash.
  GetHash() = CalculateHash();
}

template <typename T, class Container>
bool BaseStack<T, Container>::CheckPoison() const {

  for (int i = size(); i < capacity(); ++i) {
    if (Get(i) != Poison<T>()) {
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

  if (size() >= capacity()) {
    message = "size >= capacity";
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

  if (!GetDataLCanary.OK()) {
    message = "Left data canary is broken";
    return false;
  }

  if (!GetDataRCanary.OK()) {
    message = "Right data canary is broken";
    return false;
  }

  if (GetHash() != CalculateHash()) {                   // GetHash is not const
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
typename BaseStack<T, Container>::hash_type
BaseStack<T, Container>::CalculateHash() const {
  // Hash all struct.
  const char* ptr = (const char*)&GetDataLCanary();
  std::string_view str(ptr, DataSizeInBytes());
  return std::hash<std::string_view>{}(str);
}

template <typename T, class Container>
void BaseStack<T, Container>::push(const T& value) {

  if (capacity() <= size()) {
    Reallocate();
  }

  Get(size()) = value;
  ++GetSize();

  Update();
}

template <typename T, class Container>
void BaseStack<T, Container>::pop() {

  // Do nothing if pop empty stack
  if (empty()) return;

  top() = Poison<T>();
  --GetSize();

  Update();
}



//==============================================================================

template <typename T>
class DynamicStack : public BaseStack<T, DynamicProtectedData<T>> {

    typedef BaseStack<T, DynamicProtectedData<T>> Base;

  public:

    typedef typename Base::size_type size_type;

     /// If no capacity is specified, create Stack with capacity = 1.
     DynamicStack(size_t capacity = 1);
     DynamicStack(const DynamicStack& other);
     DynamicStack(DynamicStack&&      other) = default;                               // Does it call base move C-tor?

    // Delete allocated data.
    ~DynamicStack() { delete this->data_; }

    // Used in StaticStack::Get(i) implementation.
    template <typename U, int N>
    friend class StaticStack;

  protected:

    /* Overridden methods. */

    void Reallocate() override;

          T& Get(size_type i)       override;
    const T& Get(size_type i) const override;
};

//.............................................................................

template <typename T>
DynamicStack<T>::DynamicStack(size_t capacity)
    : Base(new (capacity) DynamicProtectedData<T>(capacity)) {}

template <typename T>
DynamicStack<T>::DynamicStack(const DynamicStack& other) {
  this->data_ = new (other.capacity()) DynamicProtectedData(other.data_);
}

template <typename T>
void DynamicStack<T>::Reallocate() {
  this->GetCapacity() *= 2;

  DynamicProtectedData<T>* old = this->data_;
  this->data_ = new (this->capacity()) DynamicProtectedData<T>(*old);
  delete old;
}

template <typename T>
T& DynamicStack<T>::Get(size_type i) {
  return this->data_->buffer_[i];
}

template <typename T>
const T& DynamicStack<T>::Get(size_type i) const {
  return this->data_->buffer_[i];
}



//==============================================================================

template <typename T, int N>
class StaticStack : public StaticProtectedData<T, N>,
                    public BaseStack<T, StaticProtectedData<T, N>> {

    typedef BaseStack<T, StaticProtectedData<T, N>> Base;

  public:

    typedef typename Base::size_type size_type;

     StaticStack();
     StaticStack(const StaticStack& other);
     StaticStack(StaticStack&&      other);

    ~StaticStack() = default;

  protected:

    /* Overridden methods. */

    void Reallocate() override;

          T& Get(size_type i)       override;
    const T& Get(size_type i) const override;
};

//.............................................................................

template <typename T, int N>
StaticStack<T, N>::StaticStack()
    : StaticProtectedData<T, N>(),
      // There is a hack: we cast pointer from a left data canary to
      // a StaticProtectedData (get pointer to the data begin)
      // and consider it is an allocated container given to a Base.
      // It let us treat Static and Dynamic data the same way.
      Base(static_cast<StaticProtectedData<T, N>*>(this)) {}

template <typename T, int N>
StaticStack<T, N>::StaticStack(const StaticStack& other)
    : StaticProtectedData<T, N>(other),
      Base(static_cast<StaticProtectedData<T, N>*>(this)) {}

template <typename T, int N>
StaticStack<T, N>::StaticStack(StaticStack&& other)
    : StaticProtectedData<T, N>(std::move(other)),
      Base(static_cast<StaticProtectedData<T, N>*>(this)) {}

template <typename T, int N>
void StaticStack<T, N>::Reallocate() {

  if (this->dynamic_ == nullptr) {
    this->dynamic_ = new DynamicStack<T>();
  } else {
    this->dynamic_->Reallocate();
  }

  this->GetCapacity() = N + this->dynamic_->capacity();
}

template <typename T, int N>
T& StaticStack<T, N>::Get(size_type i) {
  // If static capacity < element index then element is located in dynamic_.
  return i < N ? this->buffer_[i] : this->dynamic_->Get(i - N);
}

template <typename T, int N>
const T& StaticStack<T, N>::Get(size_type i) const {
  // If static capacity < element index then element is located in dynamic_.
  return i < N ? this->buffer_[i] : this->dynamic_->Get(i - N);
}

