#include "play_modes.hpp"
#include "node.hpp"
#include "garbage_collector.hpp"
#include "tree_dump.hpp"
#include "strview_out.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#include <string_view>
#include <vector>
#include <queue>


static GarbageCollector collector;


//========================= MINOR FUNCTIONS ====================================

char GetCorrectAnswer() {
  char answer = 0;
  while (!(answer == 'y' || answer == 'n')) {
    scanf("%c", &answer);
    if (!(answer == 'y' || answer == 'n')) {
      printf("Ввод: '%c'. Надо ввести (английские) y или n.\n", answer);
    }
  }
  return answer;
}

char GetUnshureCorrectAnswer() {
  char answer = 0;
  while (!(answer == 'y' || answer == 'n' || answer == 's' || answer == 'u')) {
    scanf("%c", &answer);
    if (!(answer == 'y' || answer == 'n' || answer == 's' || answer == 'u')) {
      printf("Ввод: '%c'. Надо ввести (английские) y, s, u или n.\n", answer);
    }
  }
}

void ReadString(char* string, bool get_char = true) {
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

  printf("Что ты загадал?\n"); getchar();

  char* new_name = (char*)collector.Allocate(MAX_STR_LEN, sizeof(char));
  ReadString(new_name);
  FixString(new_name, false);
  new_leaf->str_ = std::string_view(new_name);

  printf("Какой \"" STRVIEW "\" по сравнению с \"" STRVIEW "\"?\n"
      "Введи предложение, на которое можно ответить 'да' или 'нет':\n",
      VIEW(node->str_), VIEW(new_leaf->str_));

  char* feature = (char*)collector.Allocate(MAX_STR_LEN, sizeof(char));
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

  if (is_negative) { std::swap(new_leaf, node); }
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
      char* str_with_prefix = (char*)collector.Allocate(
          str_with_prefix_size, sizeof(char));

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
    printf("\t" STRVIEW "\n", VIEW_LEN((*it), (*it).size() - 1));
  }
}

//========================= MAJOR FUNCTIONS ====================================

template <typename Node>
struct WaitNode {

  Node* node_;
  size_t depth_;
  bool high_priority_;

  WaitNode()
      : node_(nullptr),
        depth_(std::numeric_limits<int>::max()),
        high_priority_(false) {}

  WaitNode(Node* node, size_t depth, bool is_shure)
      : node_(node),
        depth_(depth),
        high_priority_(is_shure) {}
};

struct WaitNodeCmp {
  /// Operator > for WaitNode.
  template <typename Node>
  bool operator()(const WaitNode<Node>& a, const WaitNode<Node>& b) {
    if (a.high_priority_ == b.high_priority_) {
      return a.depth_ > b.depth_;
    }
    return a.high_priority_ < b.high_priority_;
  }
};

void PlayGuess(DecisionTree& tree) {
  DecisionTree::value_type* node = tree.GetRoot();
  std::priority_queue<WaitNode<DecisionTree::value_type>,
                      std::vector<WaitNode<DecisionTree::value_type>>,
                      WaitNodeCmp> wait_nodes;
  wait_nodes.emplace(WaitNode<DecisionTree::value_type>());
  size_t depth = 0;
  char answer = 0;

  printf("y - yes, s (sure) - mostly yes, u (unshure) - mostly no, n - no\n");
  while (true) {
    if (node->IsLeaf()) {
      printf("Это " STRVIEW "?\nОтвет(y/n): ", VIEW(node->str_)); getchar();
      answer = GetCorrectAnswer();
      if (answer == 'y') {
        printf("Объект угадан.\n");
        return;
      } else
      if (wait_nodes.size() == 1) {
        AddNewObject(tree, node);
        return;
      }
      node = wait_nodes.top().node_;
      depth = wait_nodes.top().depth_;
      wait_nodes.pop();
    }

    printf("Вопрос: " STRVIEW "\nОтвет(y/s/u/n): ", VIEW(node->str_));
    getchar();

    answer = GetUnshureCorrectAnswer();
    if (answer == 'y') {
      node = node->right_;
    } else
    if (answer == 's') {
      wait_nodes.emplace(
          WaitNode<DecisionTree::value_type>(node->left_, depth, false));
      node = node->right_;
    } else
    if (answer == 'u') {
      wait_nodes.emplace(
          WaitNode<DecisionTree::value_type>(node->right_, depth, true));
      node = node->left_;
    } else
    if (answer == 'n') {
      node = node->left_;
    }
    depth++;
  }
}

void PlayDefinition(DecisionTree& tree) {
  char object_name[MAX_STR_LEN];
  printf("Введи имя: "); getchar();
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
  printf("Введи имя первого объекта: "); getchar();
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
    auto it1 = description1.crbegin();
    auto it2 = description2.crbegin();
    if (*it1 == *it2) {
      printf("%s и %s оба ", object1_name, object2_name);
    } else {
      printf("У %s и %s нет ничего общего", object1_name, object2_name);
    }
    while (it1 != description1.crend() && it2 != description2.crend() &&
           *it1 == *it2) {
      // size - 1 -> skip '?'.
      printf(STRVIEW ", ", VIEW_LEN((*it1), (*it1).size() - 1));
      ++it1; ++it2;
    }
    printf(".\n");
    printf("Однако %s", object1_name);
    bool is_first_iteration = true;
    while (it1 != description1.crend()) {
      if (it1 + 1 != description1.crend() && !is_first_iteration) {
        printf(",");
        is_first_iteration = false;
      }
      printf(" " STRVIEW, VIEW_LEN((*it1), (*it1).size() - 1));
      ++it1;
    }
    printf(", a %s", object2_name);
    is_first_iteration = true;
    while (it2 != description2.crend()) {
      if (it2 + 1 != description2.crend() && !is_first_iteration) {
        printf(",");
        is_first_iteration = false;
      }
      printf(" " STRVIEW, VIEW_LEN((*it2), (*it2).size() - 1));
      ++it2;
    }
    printf(".\n");
  } else {
    if (description1.size() == 0) {
      printf("%s - имя не найдено.\n", object1_name); getchar();
    } else {
      printf("%s - имя не найдено.\n", object2_name); getchar();
    }
  }
}

void PlayShowAll(DecisionTree& tree) {
  FILE* tmp = fopen(".tmp.dot", "w");
  DumpTree(tree, tmp);
  fclose(tmp);
  system("xdot .tmp.dot");
}

