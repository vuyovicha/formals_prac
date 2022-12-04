#ifndef FORMALS_SRC_STATEDFA_H_
#define FORMALS_SRC_STATEDFA_H_

#include "Transition.h"
#include <vector>
#pragma once

class StateDFA {
 public:
  int index;
  std::vector<int> nfa_states_set_indexes;
  bool is_final;
  bool is_start;
  std::vector<Transition> transitions;

  StateDFA(int get_index, std::vector<int> get_nfa_indexes, bool get_is_start, bool get_is_final) {
    index = get_index;
    nfa_states_set_indexes = std::move(get_nfa_indexes);
    is_final = get_is_final;
    is_start = get_is_start;
  }

};

#endif //FORMALS_SRC_STATEDFA_H_
