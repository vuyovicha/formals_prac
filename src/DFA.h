#ifndef FORMALS_SRC_DFA_H_
#define FORMALS_SRC_DFA_H_

#include "Automata.h"
#include "StateDFA.h"
#pragma once

class Automata;

class DFA {
 public:
  std::vector<StateDFA> states;

  explicit DFA(Automata& NFA);

 private:
  bool CheckIfNFAStatesSetExists(const std::vector<int>& nfa_states_set);

  int GetIndexOfNFAStateSet(const std::vector<int>& nfa_states_set);

  static bool CheckIfStateExists(const std::vector<int>& current_input_word_nfa_states_set_destination_indexes, int index);
};

#endif //FORMALS_SRC_DFA_H_
