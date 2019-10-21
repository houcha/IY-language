// Protected data realization.
//==============================================================================

template <typename T, size_t N>
ProtectedData<T, N>::ProtectedData(size_t capacity)
    : l_canary_(),
      hash_(0),
      size_(0),
      capacity_(capacity) {

  for (int i = 0; i < capacity_; ++i) {
    SetPoison(buffer_[i]);
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
    SetPoison(buffer_[i]);
  }
}

template <typename T, size_t N>
ProtectedData<T, N>::ProtectedData(ProtectedData&& other)
    : l_canary_(),
      hash_(0),
      size_(other.size_),
      capacity_(other.capacity_) {

  for (int i = 0; i < size_; ++i) {
    buffer_[i] = std::move(other.buffer_[i]);
  }

  for (int i = size_; i < capacity_; ++i) {
    SetPoison(buffer_[i]);
  }
}

template <typename T, size_t N>
ProtectedData<T, N>& ProtectedData<T, N>::operator=(const ProtectedData& other) {
  if (this == &other) return *this;
  assert(capacity_ == other.capacity_);

  for (int i = 0; i < other.size_; ++i) {
    buffer_[i] = other.buffer_[i];
  }

  for (int i = size_; i < other.capacity_; ++i) {
    SetPoison(buffer_[i]);
  }

  return *this;
}

template <typename T, size_t N>
ProtectedData<T, N>& ProtectedData<T, N>::operator=(ProtectedData&& other) {
  if (this == &other) return *this;
  assert(capacity_ == other.capacity_);

  for (int i = 0; i < other.size_; ++i) {
    buffer_[i] = std::move(other.buffer_[i]);
  }

  for (int i = size_; i < other.capacity_; ++i) {
    SetPoison(buffer_[i]);
  }

  return *this;
}

// Dynamic protected data realization.
//==============================================================================

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
DynamicProtectedData<T>&
DynamicProtectedData<T>::operator=(const DynamicProtectedData& other) {
  if (this == &other) return *this;
  // Just call base class operator=.
  ProtectedData<T, 0>::operator=(other);

  return *this;
}

template <typename T>
DynamicProtectedData<T>&
DynamicProtectedData<T>::operator=(DynamicProtectedData&& other) {
  if (this == &other) return *this;

  ProtectedData<T, 0>::operator=(std::move(other));

  return *this;
}

template <typename T>
DynamicProtectedData<T>::~DynamicProtectedData() {
  // Call destructors of real objects (NOT POISONS!).
  for (int i = 0; i < this->size_; ++i) {
    this->buffer_[i].~T();
  }
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


// Static protected data realization.
//==============================================================================
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
StaticProtectedData<T, N>&
StaticProtectedData<T, N>::operator=(const StaticProtectedData& other) {
  if (this == &other) return *this;
  // Call base class operator=.
  ProtectedData<T, 0>::operator=(other);

  delete dynamic_;
  dynamic_ = new DynamicStack(*other.dynamic_);

  return *this;
}

template <typename T, size_t N>
StaticProtectedData<T, N>&
StaticProtectedData<T, N>::operator=(StaticProtectedData&& other) {
  if (this == &other) return *this;
  // call base class operator=.
  ProtectedData<T, 0>::operator=(std::move(other));

  delete dynamic_;
  dynamic_ = other.dynamic_;
  other.dynamic_ = nullptr;

  return *this;
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

