#include "expression_tree.hpp"
#include <cassert>


void DumpBeforeDiff(const MathNode* root, FILE* texfile) {
  fprintf(texfile, "Initial expression:\n");
  fprintf(texfile, "$$\\left(");
  root->WriteToTex(texfile);
  fprintf(texfile, "\\right)_x^{'}$$\nDifferentiation:\n");
}

void DumpAfterDiff(const MathNode* root, const MathNode* diff, FILE* texfile) {
  fprintf(texfile, "Sum up:\n$$\\left(");
  root->WriteToTex(texfile);
  fprintf(texfile, "\\right)_x^{'} = ");
  diff->WriteToTex(texfile);
  fprintf(texfile, "$$\n");
}

void DumpAfterSimpl(const MathNode* root, FILE* texfile) {
  fprintf(texfile, "Finally simplify the expression and get the answer:\n$$");
  root->WriteToTex(texfile);
  fprintf(texfile, "$$\n");
}

ExprTree::ExprTree(MathNode* node) {
  root_ = node;
  for (auto it = Begin(); it != End(); ++it) {
    ++count_nodes_;
  }
}

ExprTree ExprTree::Differentiate(const char* var, FILE* texfile) const {
  assert(texfile != nullptr);
  DumpBeforeDiff(GetRoot(), texfile);
  MathNode* differentiated = GetRoot()->Differentiate(var, texfile);
  DumpAfterDiff(GetRoot(), differentiated, texfile);
  auto tree = ExprTree(differentiated->Simplify(texfile));
  DumpAfterSimpl(tree.GetRoot(), texfile);
  return tree;
}

void ExprTree::Dump(FILE* graphfile) const {
  assert(graphfile != nullptr);
  fprintf(graphfile, "digraph {\nnode [shape=circle]\n");
  auto it = Begin();
  ++it;
  for ( ; it != End(); ++it) {
    (*it).WriteToGraphviz(graphfile);
  }
  fprintf(graphfile, "}\n");
}

void ExprTree::AddChild(value_type* parent, value_type* son) {
  parent->AddChild(son);
  ++count_nodes_;
}

void ExprTree::ReplaceNode(MathNode* replaced, MathNode* new_node) {
  if (replaced == root_) {
    root_ = new_node;
  } else {
    // Fix parent.
    if (replaced->IsLeftChild()) {
      replaced->GetParent()->ReplaceLeft(new_node);
    } else {
      replaced->GetParent()->ReplaceRight(new_node);
    }
  }
  // Fix node.
  new_node->AddChild(replaced->GetLeft());
  new_node->AddChild(replaced->GetRight());
}

