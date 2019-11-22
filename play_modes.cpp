#include "play_modes.hpp"
#include "node.hpp"
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>


//========================= MINOR FUNCTIONS ====================================

void ReadString(char* string, bool get_char = true) {
  if (get_char) {
    getchar(); // Remove trailing \n from scanf (IMPORTANT!).
  }
  fgets(string, MAX_STR_LEN, stdin);
  // Remove \n if it is read.
  size_t name_size = strlen(string);
  if (*(string + name_size - 1) == '\n') {
    *(string + name_size - 1) = 0;
    --name_size;
  }
}

// Return true is string has negative meaning.
bool FixString(char*& string, bool is_question) {
  bool is_negative = false;
  if (is_question) {
    size_t no_len = strlen("не ");
    if (memcmp(string, "не ", no_len) == 0) {
      string += no_len;
      is_negative = true;
    }
  }
  for (size_t i = 0; string[i] != 0; ++i) {
    if (string[i] == '"') {
      string[i] = '\'';
    }
  }
  return is_negative;
}

void AddNewObject(DecisionTree& tree, DecisionTree::value_type* node) {
  DecisionTree::value_type* new_leaf  = new DecisionTree::value_type;
  DecisionTree::value_type* new_query = new DecisionTree::value_type;

  printf("Что ты загадал?\n");
  char* new_name = (char*)calloc(MAX_STR_LEN, sizeof(char));
  ReadString(new_name);
  FixString(new_name, false);
  new_leaf->str_ = std::string_view(new_name);
  printf("Какой \"%.*s\" по сравнению с \"%.*s\"?\n"
      "Введи предложение, на которое можно ответить 'да' или 'нет': ",
      static_cast<int>(node->str_.size()), node->str_.data(),
      static_cast<int>(new_leaf->str_.size()), new_leaf->str_.data());
  char* feature = (char*)calloc(MAX_STR_LEN, sizeof(char));
  ReadString(feature, false);
  bool is_negative = FixString(feature, true);
  size_t len = strlen(feature);
  feature[len] = '?';
  new_query->str_ = std::string_view(feature, len + 1); // +1 is extra '?'.

  if (node->IsLeftChild()) {
    node->parent_->left_ = new_query;
  } else {
    node->parent_->right_ = new_query;
  }
  new_query->parent_ = node->parent_;
  if (is_negative) {
    swap(new_left, node);
  }
  tree.AddChild(new_query, node);
  tree.AddChild(new_query, new_leaf);
}

template <typename Node>
void GetDescription(Node* node, std::vector<std::string_view>& description) {
  // Skip first node string.
  char prefix[] = "не ";
  size_t prefix_size = strlen(prefix);
  while (!node->IsRoot()) {
    Node* parent = node->parent_;
    if (node->IsLeftChild()) {
      size_t str_with_prefix_size = prefix_size + parent->str_.size();
      char* str_with_prefix = (char*)calloc(str_with_prefix_size, sizeof(char));
      strcpy(str_with_prefix, prefix);
      memcpy(str_with_prefix + prefix_size,
          parent->str_.data(), parent->str_.size());
      description.emplace_back(
          std::string_view(str_with_prefix, str_with_prefix_size));
    } else {
      description.push_back(parent->str_);
    }
    node = parent;
  }
}

template <typename String>
std::vector<std::string_view> SearchDefinition(const DecisionTree& tree,
                                               const String& name) {
  // Search for object.
  auto it = tree.Begin();
  while (it != tree.End() && (!(*it).IsLeaf() || (*it).str_ != name)) {
    // Optimize string comparsion ---^
    ++it;
  }
  // If object exists.
  std::vector<std::string_view> description;
  if (it != tree.End()) {
    GetDescription(&*it, description);
  }
  return description;
}

void PrintDescription(const std::vector<std::string_view>& description) {
  for (auto it = description.crbegin(); it != description.crend(); ++it) {
    // (size - 1) because last character is '?'.
    printf("\t%.*s\n",
        static_cast<int>((*it).size()) - 1, (*it).data());
  }
}

//========================= MAJOR FUNCTIONS ====================================

char GetCorrectAnswer() {
  char answer = 0;
  getchar(); // Remove trailing \n (IMPORTANT!).
  while (!(answer == 'y' || answer == 'n')) {
    answer = getchar();
    if (!(answer == 'y' || answer == 'n')) {
      printf("Надо ввести y(английскую!) или n.\n");
    }
  }
  return answer;
}

void PlayGuess(DecisionTree& tree) {
  char answer = 0;
  DecisionTree::value_type* node = tree.GetRoot();
  while (!node->IsLeaf()) {
    printf("Вопрос: %.*s\nОтвет(y/n): ",
        static_cast<int>(node->str_.size()), node->str_.data());
    answer = GetCorrectAnswer();
    if (answer == 'y') {
      node = node->right_;
    } else
    if (answer == 'n') {
      node = node->left_;
    }
  }
  printf("Это %.*s?\nОтвет(y/n): ",
      static_cast<int>(node->str_.size()), node->str_.data());
  answer = GetCorrectAnswer();
  if (answer == 'y') {
    printf("Объект угадан.\n");
  } else
  if (answer == 'n') {
    AddNewObject(tree, node);
  }
}

void PlayDefinition(DecisionTree& tree) {
  char object_name[MAX_STR_LEN];
  printf("Введи имя: ");
  ReadString(object_name);
  std::string_view str_view(object_name);

  std::vector<std::string_view> description =
      SearchDefinition(tree, str_view);

  if (description.size() != 0) {
    printf("Описание:\n");
    PrintDescription(description);
  } else {
    printf("%s - имя не найдено.\n", object_name);
  }
}

// TODO: rewrite to LCA.
void PlayDifference(DecisionTree& tree) {
  char object1_name[MAX_STR_LEN];
  char object2_name[MAX_STR_LEN];
  printf("Введи имя первого объекта: ");
  ReadString(object1_name);
  printf("Введи имя второго объекта: ");
  ReadString(object2_name, false);
  std::string_view str1_view(object1_name);
  std::string_view str2_view(object2_name);

  std::vector<std::string_view> description1 =
      SearchDefinition(tree, str1_view);
  std::vector<std::string_view> description2 =
      SearchDefinition(tree, str2_view);

  if (description1.size() != 0 && description2.size() != 0) {
    printf("Общее:\n");
    auto it1 = description1.crbegin();
    auto it2 = description2.crbegin();
    while (it1 != description1.crend() && it2 != description2.crend() &&
           *it1 == *it2) {
      // size - 1 -- skip '?'.
      printf("\t%.*s\n", static_cast<int>((*it1).size() - 1), (*it1).data());
      ++it1;
      ++it2;
    }
    printf("Различия:\n");
    printf("\t%s:\n", object1_name);
    while (it1 != description1.crend()) {
      printf("\t\t%.*s\n", static_cast<int>((*it1).size() - 1), (*it1).data());
      ++it1;
    }
    printf("\t%s:\n", object2_name);
    while (it2 != description2.crend()) {
      printf("\t\t%.*s\n", static_cast<int>((*it2).size() - 1), (*it2).data());
      ++it2;
    }
  } else {
    if (description1.size() == 0) {
      printf("%s - имя не найдено.\n", object1_name);
    } else {
      printf("%s - имя не найдено.\n", object2_name);
    }
  }
}

void PlayShowAll(DecisionTree& tree) {
  printf("Все объекты:\n\n");
  for (auto it = tree.Begin(); it != tree.End(); ++it) {
    if ((*it).IsLeaf()) {
      std::vector<std::string_view> description;
      GetDescription(&*it, description);
      printf("Объект: %.*s\nОписание:\n",
          static_cast<int>((*it).str_.size()), (*it).str_.data());
      PrintDescription(description);
      printf("\n");
    }
  }
}

