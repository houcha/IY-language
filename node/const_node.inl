template <typename T>
MathNode* ConstNode<T>::Differentiate(const char* var) const {
  return new ConstNode<int>(0);
}


template <typename T>
uint8_t ConstNode<T>::GetPriority() const {
  return 0;
}

template <typename T>
T ConstNode<T>::GetValue() const {
  return value_;
}

template <typename T>
MathNode* ConstNode<T>::CopyThis() const {
  return new ConstNode<T>(*this);
}

template <typename T>
const std::string ConstNode<T>::GetString() const {
  return std::to_string(value_);
}

template <typename T>
const char* ConstNode<T>::GetColor() const {
  return "floralwhite";
}

