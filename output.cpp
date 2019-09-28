#include "output.h"

void OutputLines(const std::vector<std::string_view>& lines,
                 FILE* output_file) {

  for (int i = 0; i < lines.size(); ++i) {
    fprintf(output_file, "%s", lines[i].data());
    fprintf(output_file, "\n");
  }
}

void OutputText(const std::string& text, FILE* output_file) {
  for (int i = 0; i < text.size(); ++i) {
    if (text[i] == '\0') {
      fprintf(output_file, "\n");
    } else {
      fprintf(output_file, "%c", text[i]);
    }
  }

  fclose(output_file);
}
