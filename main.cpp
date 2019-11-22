#include <cstdio>
#include <cstring>
#include "play_modes.hpp"
#include "text.hpp"
#include "decision_tree.hpp"
#include "tree_dump.hpp"


template <typename Node>
void WriteNode(Node* node, char* indent, ssize_t indent_size, FILE* file) {
  fprintf(file, "\n%.*s{\"%.*s\"",
      static_cast<int>(indent_size), indent,
      static_cast<int>(node->str_.size()), node->str_.data());
  if (!node->IsLeaf()) {
    WriteNode(node->right_, indent, indent_size + 1, file);
    WriteNode(node->left_, indent, indent_size + 1, file);
  }
  fprintf(file, "}");
}

void SaveTree(const DecisionTree& tree, FILE* file) {
  char indent[MAX_STR_LEN];
  memset(indent, '\t', MAX_STR_LEN);
  WriteNode(tree.GetRoot(), indent, 0, file);
}

void ShowHelp() {
  printf("Акинатор имеет 4 режима игры:\n\
      1. Отгадывание. Пытается отгадать загаданный объект.\n\
      2. Определение. Даёт определение объекта.\n\
      3. Разница. Показывает сходство и различие объектов.\n\
      4. Показ. Показывает все объекты и их определения.\n");
}


int main() {
  Text base;
  FILE* database_file = fopen("database.txt", "r");
  base.Load(database_file);
  fclose(database_file);

  DecisionTree tree(base.Data());

  printf("Акинатор приветствует тебя.\n\n");
  ShowHelp();

  int mode = 0;
  bool exit = 0;

  while (!exit) {
    printf("\nВведи номер режима: ");
    scanf("%d", &mode);
    switch(mode) {
      case 0:
          ShowHelp();
          break;
      case 1:
          PlayGuess(tree);
          break;
      case 2:
          PlayDefinition(tree);
          break;
      case 3:
          PlayDifference(tree);
          break;
      case 4:
          PlayShowAll(tree);
          break;
      default:
          printf("Неверный номер режима.\n");
          printf("Нажми 0, чтобы обратиться за помощью.\n");
    }
    printf("Хочешь продолжить?\nОтвет(y/n): ");
    getchar(); // Skip trailing \n.
    char confirm = getchar();
    if (confirm == 'n') {
      exit = 1;
    }
  }

  printf("Ты заходи, если что...\n");

  FILE* dump_file = fopen("graph.dot", "w");
  DumpTree(tree, dump_file);
  fclose(dump_file);

  FILE* save_file = fopen("database.txt", "w");
  SaveTree(tree, save_file);
  fclose(save_file);

  return 0;
}

