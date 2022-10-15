#ifndef FORMALS_SRC_AUTOMATA_H_
#define FORMALS_SRC_AUTOMATA_H_

#include "DFA.h"
#include "State.h"
#pragma once

class DFA;

class Automata {
 public:
  std::vector<State> states;
  int start_state_index;

  Automata(int states_amount, int starting_state_index, const std::vector<int>& final_indexes);

  explicit Automata(DFA& dfa);

  void DFAtoFullDFA();

  void FullDFAtoComplementDFA();

  void FullDFAtoMinDFA();

  void PrintAutomata();

  void InsertTransition(int from_index, const std::string& input, int to_index);

  void DeleteEpsilonTransitions();

  bool IsAnyStateStarting(std::vector<int>& states_set_indexes);

  bool IsAnyStateFinal(std::vector<int>& states_set_indexes);

  std::string FDFAtoRegularExpression();

 private:
  std::vector<int> GetFinalStatesIndexes();

  bool IsDFAFullCheck();

  static int GetSplitSetIndex(int state_index, int set_size, const std::vector<std::vector<State>>& split_states);

  static bool IsDestinationSetProcessed(int destination_set_index, const std::vector<int>& processed_destination_sets_indexes);

  static int GetDestinationStateIndex(const std::vector<std::vector<State>>& split_states, int set_index, int state_index, int input_word_index);

  static bool IsAnyStateStarting(std::vector<State>& states_array);

  void RearrangeStatesForMinDFA(std::vector<std::vector<State>>& split_states);

  static bool TransitionExistsCheck(int from_index, const std::string& input, int to_index, const std::vector<State>& new_states);

  bool TransitionExistsCheck(int from_index, const std::string& input, int to_index);
};

#endif //FORMALS_SRC_AUTOMATA_H_
