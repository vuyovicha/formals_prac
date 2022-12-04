#ifndef FORMALS_SRC_TRANSITION_H_
#define FORMALS_SRC_TRANSITION_H_

#include <vector>
#include <string>
#pragma once

class Transition {
 public:
  std::string input;
  int destination_state_index;

  Transition(std::string get_input, int get_destination_state_index) {
    input = std::move(get_input);
    destination_state_index = get_destination_state_index;
  }
};

#endif //FORMALS_SRC_TRANSITION_H_
