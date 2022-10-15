#include "Automata.h"
#include "Constants.h"
#include <iostream>
#pragma once

Automata::Automata(int states_amount, const int starting_state_index, const std::vector<int>& final_indexes) {
  for (int i = 0; i < states_amount; i++) {
    states.emplace_back(i);
  }
  states[starting_state_index].is_start = true;
  start_state_index = starting_state_index;
  for (auto& index : final_indexes) {
    states[index].is_final = true;
  }
}

Automata::Automata(DFA& dfa) {
  for (auto& dfa_state : dfa.states) {
    states.emplace_back(dfa_state.index, dfa_state.transitions, dfa_state.is_final, dfa_state.is_start);
    if (dfa_state.is_start) {
      start_state_index = dfa_state.index;
    }
  }
}

void Automata::DFAtoFullDFA() {
  if (!IsDFAFullCheck()) {
    int trash_state_index = (int)states.size();
    std::vector<Transition> trash_state_transitions;
    for (auto& input_word : alphabet) {
      trash_state_transitions.emplace_back(input_word, trash_state_index);
    }
    states.emplace_back(trash_state_index, trash_state_transitions, false, false);
    for (auto& state : states) {
      for (auto& input_word : alphabet) {
        bool transition_not_exist = true;
        for (auto & transition : state.transitions) {
          if (transition.input == input_word) {
            transition_not_exist = false;
          }
        }
        if (transition_not_exist) {
          InsertTransition(state.index, input_word, trash_state_index);
        }
      }
    }
  }
}

void Automata::FullDFAtoComplementDFA() {
  for (auto& state : states) {
    state.is_final = !state.is_final;
  }
}

void Automata::FullDFAtoMinDFA() {
  std::vector<State> final_states;
  std::vector<State> non_final_states;
  for (auto& state : states) {
    if (state.is_final) {
      final_states.push_back(state);
    } else {
      non_final_states.push_back(state);
    }
  }
  bool flag_split_occurred = true;
  std::vector<std::vector<State>> split_states;
  split_states.push_back(final_states);
  split_states.push_back(non_final_states);
  while (flag_split_occurred) {
    flag_split_occurred = false;
    for (int current_input_word_index = 0; current_input_word_index < alphabet.size(); current_input_word_index++) {
      int amount_of_split_sets = (int)split_states.size();
      for (int set_index = 0; set_index < amount_of_split_sets; set_index++) {
        std::vector<int> processed_destination_sets_indexes;
        for (int state_index = 0; state_index < split_states[set_index].size(); state_index++) {
          int destination_state_index = GetDestinationStateIndex(split_states, set_index, state_index, current_input_word_index);
          int destination_set_index = GetSplitSetIndex(destination_state_index, amount_of_split_sets, split_states);
          if (!IsDestinationSetProcessed(destination_set_index, processed_destination_sets_indexes)) {
            processed_destination_sets_indexes.push_back(destination_set_index);
            std::vector<State> new_split_set_by_destination;
            for (int set_state_index = 0; set_state_index < split_states[set_index].size(); set_state_index++) {
              if (GetSplitSetIndex(GetDestinationStateIndex(split_states, set_index, set_state_index, current_input_word_index), amount_of_split_sets, split_states) == destination_set_index) {
                new_split_set_by_destination.push_back(split_states[set_index][set_state_index]);
              }
            }
            split_states.push_back(new_split_set_by_destination);
          }
        }
      }
      split_states.erase(split_states.begin(), split_states.begin() + amount_of_split_sets);
      if (split_states.size() != amount_of_split_sets) {
        flag_split_occurred = true;
      }
    }
  }
  RearrangeStatesForMinDFA(split_states);
}

void Automata::PrintAutomata() {
  std::cout << std::endl;
  std::cout << "AUTOMATA ";
  std::cout << std::endl;
  for (int i = 0; i < states.size(); i++) {
    std::cout << "State " << i << std::endl;
    std::cout << "is staring: " << states[i].is_start << std::endl;
    std::cout << "is final: " << states[i].is_final << std::endl;
    for (auto& transition : states[i].transitions) {
      std::cout << transition.input << " " << transition.destination_state_index << std::endl;
    }
    std::cout << std::endl;
  }
}

void Automata::InsertTransition(int from_index, const std::string& input, int to_index) {
  states[from_index].transitions.emplace_back(input, to_index);
}

void Automata::DeleteEpsilonTransitions() {
  bool changes = true;
  while (changes) {
    changes = false;
    for (int i = 0; i < states.size(); i++) {
      int transitions_size = (int)states[i].transitions.size();
      for (int j = 0; j < transitions_size; j++) {
        if (states[i].transitions[j].input == epsilon) {
          if (states[i].is_start) {
            states[states[i].transitions[j].destination_state_index].is_start = true;
          }
          if (states[states[i].transitions[j].destination_state_index].is_final) {
            states[i].is_final = true;
          }
          for (auto& destination_state_transition : states[states[i].transitions[j].destination_state_index].transitions) {
            if (destination_state_transition.input != removed_epsilon) {
              if (!(destination_state_transition.input == epsilon && states[i].index == destination_state_transition.destination_state_index)) {
                if (!TransitionExistsCheck(states[i].index, destination_state_transition.input, destination_state_transition.destination_state_index)) {
                  InsertTransition(states[i].index, destination_state_transition.input, destination_state_transition.destination_state_index);
                }
              }
            }
          }
          changes = true;
          states[i].transitions[j].input = removed_epsilon;
        }
      }
    }
  }
  for (auto& state : states) {
    for (int i = 0; i < state.transitions.size(); i++) {
      if (state.transitions[i].input == removed_epsilon) {
        state.transitions.erase(state.transitions.begin() + i);
        i--;
      }
    }
  }
}

bool Automata::IsAnyStateStarting(std::vector<int>& states_set_indexes) {
  for (auto& index : states_set_indexes) {
    if (states[index].is_start) {
      return true;
    }
  }
  return false;
}

bool Automata::IsAnyStateFinal(std::vector<int>& states_set_indexes) {
  for (auto& index : states_set_indexes) {
    if (states[index].is_final) {
      return true;
    }
  }
  return false;
}

std::string Automata::FDFAtoRegularExpression() {
  std::vector<int> final_states_indexes = GetFinalStatesIndexes();
  int final_state_index;
  if (final_states_indexes.size() > 1) {
    final_state_index = (int)states.size();
    states.emplace_back(final_state_index, true, false);
    for (auto& index : final_states_indexes) {
      InsertTransition(index, epsilon, final_state_index);
    }
  } else {
    final_state_index = final_states_indexes[0];
  }
  //todo
}

std::vector<int> Automata::GetFinalStatesIndexes() {
  std::vector<int> final_states_indexes;
  for (auto& state : states) {
    if (state.is_final) {
      final_states_indexes.push_back(state.index);
    }
  }
  return final_states_indexes;
}

bool Automata::IsDFAFullCheck() {
  for (auto& state : states) {
    if (state.transitions.size() != alphabet.size()) {
      return false;
    }
  }
  return true;
}

int Automata::GetSplitSetIndex(int state_index, int set_size, const std::vector<std::vector<State>>& split_states) {
  for (int set_index = 0; set_index < set_size; set_index++) {
    for (auto& state : split_states[set_index]) {
      if (state.index == state_index) {
        return set_index;
      }
    }
  }
  return -1; //for error
}

bool Automata::IsDestinationSetProcessed(int destination_set_index, const std::vector<int>& processed_destination_sets_indexes) {
  for (auto& index : processed_destination_sets_indexes) {
    if (index == destination_set_index) {
      return true;
    }
  }
  return false;
}

int Automata::GetDestinationStateIndex(const std::vector<std::vector<State>>& split_states, int set_index, int state_index, int input_word_index) {
  for (auto& transition : split_states[set_index][state_index].transitions) {
    if (transition.input == alphabet[input_word_index]) {
      return transition.destination_state_index;
    }
  }
}

bool Automata::IsAnyStateStarting(std::vector<State>& states_array) {
  for (auto& state : states_array) {
    if (state.is_start) {
      return true;
    }
  }
  return false;
}

void Automata::RearrangeStatesForMinDFA(std::vector<std::vector<State>>& split_states) {
  std::vector<State> new_states;
  for (int set_index = 0; set_index < split_states.size(); set_index++) {
    bool is_starting = IsAnyStateStarting(split_states[set_index]);
    new_states.emplace_back(set_index, split_states[set_index][0].is_final, is_starting);
    if (is_starting) {
      start_state_index = set_index;
    }
  }
  for (int set_index = 0; set_index < split_states.size(); set_index++) {
    for (auto& state : split_states[set_index]) {
      for (auto& transition : state.transitions) {
        int destination_set_index = GetSplitSetIndex(transition.destination_state_index, (int)split_states.size(), split_states);
        if (!TransitionExistsCheck(set_index, transition.input, destination_set_index, new_states)) {
          new_states[set_index].transitions.emplace_back(transition.input, destination_set_index);
        }
      }
    }
  }
  states = new_states;
}

bool Automata::TransitionExistsCheck(int from_index, const std::string& input, int to_index, const std::vector<State>& new_states) {
  for (auto& transition : new_states[from_index].transitions) {
    if (transition.input == input && transition.destination_state_index == to_index) {
      return true;
    }
  }
  return false;
}

bool Automata::TransitionExistsCheck(int from_index, const std::string& input, int to_index) {
  for (auto& transition : states[from_index].transitions) {
    if (transition.input == input && transition.destination_state_index == to_index) {
      return true;
    }
  }
  return false;
}
