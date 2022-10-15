#include "DFA.h"
#include "Constants.h"

DFA::DFA(Automata& NFA) {
  std::vector<std::vector<int>> states_sets_queue;
  int index_in_states = 0;
  std::vector<int> zero_set = {NFA.start_state_index};
  states.emplace_back(index_in_states, zero_set, NFA.IsAnyStateStarting(zero_set), NFA.IsAnyStateFinal(zero_set));
  index_in_states++;
  int loop_index = 0;
  while (loop_index < states.size()) {
    for (auto& input_word : alphabet) {
      std::vector<int> current_input_word_nfa_states_set_destination_indexes;
      for (auto& nfa_state_set_index : states[loop_index].nfa_states_set_indexes) {
        for (auto& nfa_state_set_index_transition : NFA.states[nfa_state_set_index].transitions) {
          if (nfa_state_set_index_transition.input == input_word) {
            if (!CheckIfStateExists(current_input_word_nfa_states_set_destination_indexes, nfa_state_set_index_transition.destination_state_index)) {
              current_input_word_nfa_states_set_destination_indexes.push_back(nfa_state_set_index_transition.destination_state_index);
            }
          }
        }
      }
      std::sort(current_input_word_nfa_states_set_destination_indexes.begin(), current_input_word_nfa_states_set_destination_indexes.end());
      if (!current_input_word_nfa_states_set_destination_indexes.empty()) {
        int dfa_destination_state_index;
        if (CheckIfNFAStatesSetExists(current_input_word_nfa_states_set_destination_indexes)) {
          dfa_destination_state_index = GetIndexOfNFAStateSet(current_input_word_nfa_states_set_destination_indexes);
        } else {
          states.emplace_back(index_in_states, current_input_word_nfa_states_set_destination_indexes, NFA.IsAnyStateStarting(current_input_word_nfa_states_set_destination_indexes), NFA.IsAnyStateFinal(current_input_word_nfa_states_set_destination_indexes));
          dfa_destination_state_index = index_in_states;
          index_in_states++;
        }
        states[loop_index].transitions.emplace_back(input_word, dfa_destination_state_index);
      }
    }
    loop_index++;
  }
}

bool DFA::CheckIfStateExists(const std::vector<int>& current_input_word_nfa_states_set_destination_indexes, int index) {
  for(auto& element : current_input_word_nfa_states_set_destination_indexes) {
    if (index == element) {
      return true;
    }
  }
  return false;
}

bool DFA::CheckIfNFAStatesSetExists(const std::vector<int>& nfa_states_set) {
  for (auto& dfa_state : states) {
    if (nfa_states_set == dfa_state.nfa_states_set_indexes) {
      return true;
    }
  }
  return false;
}

int DFA::GetIndexOfNFAStateSet(const std::vector<int>& nfa_states_set) {
  for (int i = 0; i < states.size(); i++) {
    if (nfa_states_set == states[i].nfa_states_set_indexes) {
      return i;
    }
  }
  return -1; //in case of error
}