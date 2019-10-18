// Base stack implementation.
//==============================================================================

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
BaseStack<T, Container>&
BaseStack<T, Container>::operator=(BaseStack&& other) {

  data_ = other.data_;
  other.data_ = nullptr;

  return *this;
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

  if (GetHash() != CalculateHash()) {
    message = "Wrong hash";
    return false;
  }

  if (!CheckPoison()) {
    message = "Non-poison value in capacity area";
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

  SetPoison(top());
  DecrementSize();

  Update();
}

// Dynamic stack implementation.
//==============================================================================

template <typename T>
DynamicStack<T>::DynamicStack(size_t capacity)
    : Base(new (capacity) DynamicProtectedData<T>(capacity)) {
  // We cannot place Update() in Base class, for it uses abstract method Get(i).
  Base::Update();
}

template <typename T>
DynamicStack<T>::DynamicStack(const DynamicStack& other)
    : Base(new (other.capacity()) DynamicProtectedData<T>(other.data_)) {

  Base::Update();
}

template <typename T>
DynamicStack<T>::DynamicStack(DynamicStack&& other)
    : Base(std::move(other)) {

  Base::Update();
}

template <typename T>
DynamicStack<T>& DynamicStack<T>::operator=(const DynamicStack& other) {

  delete this->data_;
  this->data_ = new (other.capacity()) DynamicProtectedData<T>(other.data_);

  Base::Update();

  return *this;
}

template <typename T>
DynamicStack<T>& DynamicStack<T>::operator=(DynamicStack&& other) {
  // Call base class operator=.
  Base::operator=(std::move(other));

  Base::Update();

  return *this;
}

template <typename T>
void DynamicStack<T>::Reallocate() {
  DoubleCapacity();

  DynamicProtectedData<T>* old = Base::data_;
  Base::data_ = new (capacity()) DynamicProtectedData<T>(
                                                std::move_if_noexcept(*old));
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


// Static stack implementation.
//==============================================================================

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
StaticStack<T, N>& StaticStack<T, N>::operator=(const StaticStack& other) {

  // Copy buffer.
  StaticProtectedData<T, N>::operator=(other);
  // Do not call Base::operator=, for pointer to data does not change.
  Base::Update();

  return *this;
}

template <typename T, size_t N>
StaticStack<T, N>& StaticStack<T, N>::operator=(StaticStack&& other) {

  StaticProtectedData<T, N>::operator=(std::move(other));
  Base::operator=(std::move(other));

  Base::Update();

  return *this;
}

template <typename T, size_t N>
typename StaticStack<T, N>::size_type
StaticStack<T, N>::size() const {
  // `this` is necessary to specialize scope while template inheritance.
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
      Base::OK(message) : Base::OK(message) && this->dynamic_->OK(message);
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

