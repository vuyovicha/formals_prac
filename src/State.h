#ifndef FORMALS_SRC_STATE_H_
#define FORMALS_SRC_STATE_H_

#include "Transition.h"
#include <vector>
#pragma once

class State {
 public:
  int index;
  bool is_final;
  bool is_start;
  std::vector<Transition> transitions;

  explicit State(int get_index, bool get_is_final=false, bool get_is_start=false) {
    index = get_index;
    is_final = get_is_final;
    is_start = get_is_start;
  }

  State(int get_index, std::vector<Transition> get_transitions, bool get_is_final=false, bool get_is_start=false) {
    index = get_index;
    is_final = get_is_final;
    is_start = get_is_start;
    transitions = std::move(get_transitions);
  }
};

#endif //FORMALS_SRC_STATE_H_
