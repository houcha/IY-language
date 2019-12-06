template <typename T>
MathNode* ConstNode<T>::DiffThis(const char* var, FILE* texfile) const {
  return new ConstNode<int>(0);
}

template <typename T>
void ConstNode<T>::DumpDiffThis(const char* var, FILE* texfile) const {
  fprintf(texfile, "0");
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
uint32_t ConstNode<T>::GetThisComplexity() const {
  if (std::is_integral<T>::value) {
    return 1;
  }
  return 2;
}

template <typename T>
const std::string ConstNode<T>::GetString() const {
  return std::to_string(value_);
}

template <typename T>
const char* ConstNode<T>::GetColor() const {
  return "floralwhite";
}

