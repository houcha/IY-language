#ifndef PLAY_MODES_HPP
#define PLAY_MODES_HPP

#include "decision_tree.hpp"


const size_t MAX_STR_LEN = 100;


void PlayGuess(DecisionTree& tree);
void PlayDefinition(DecisionTree& tree);
void PlayDifference(DecisionTree& tree);
void PlayShowAll(DecisionTree& tree);

char GetCorrectAnswer();


#endif // PLAY_MODES_HPP

