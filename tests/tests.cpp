#include <gtest/gtest.h>
#include <algorithm>
#include "src/Automata.h"
#include "src/DFA.h"
#include "NFA_input_data.h"
#include "src/Constants.h"

void AutomataTransitionsInserting(Automata& automata) {
  for (int i = 0; i < states_amount; i++) {
    for (int j = 0; j < state_transitions_inputs[i].size(); j++) {
      automata.InsertTransition(i, state_transitions_inputs[i][j], state_transitions_destinations[i][j]);
    }
  }
}

TEST(Automata_test, Eps_deleteing) {
  Automata automata = Automata(states_amount, starting_state_index, final_states_indexes);
  AutomataTransitionsInserting(automata);
  automata.DeleteEpsilonTransitions();
  for (auto& state : automata.states) {
    for (auto& transition : state.transitions) {
      ASSERT_NE(transition.input, epsilon);
    }
  }
}

TEST(Automata_test, FDFA_creation) {
  Automata automata = Automata(states_amount, starting_state_index, final_states_indexes);
  AutomataTransitionsInserting(automata);
  automata.DeleteEpsilonTransitions();
  DFA dfa = DFA(automata);
  Automata dfa_auto = Automata(dfa);
  dfa_auto.DFAtoFullDFA();
  for (auto& state : dfa_auto.states) {
    EXPECT_EQ(state.transitions.size(), 2);
    std::vector<std::string> current_transition_input_words;
    for (auto& transition : state.transitions) {
      current_transition_input_words.push_back(transition.input);
    }
    std::sort(current_transition_input_words.begin(), current_transition_input_words.end());
    for (int i = 0; i < alphabet.size(); i++) {
      EXPECT_EQ(alphabet[i], current_transition_input_words[i]);
    }
  }
}

TEST(Automata_test, ComplementDFA_creation) {
  Automata automata1 = Automata(states_amount, starting_state_index, final_states_indexes);
  AutomataTransitionsInserting(automata1);
  automata1.DeleteEpsilonTransitions();
  DFA dfa1 = DFA(automata1);
  Automata dfa_auto1 = Automata(dfa1);
  dfa_auto1.DFAtoFullDFA();
  Automata automata2 = Automata(states_amount, starting_state_index, final_states_indexes);
  AutomataTransitionsInserting(automata2);
  automata1.DeleteEpsilonTransitions();
  DFA dfa2 = DFA(automata1);
  Automata dfa_auto2 = Automata(dfa2);
  dfa_auto2.DFAtoFullDFA();
  dfa_auto2.FullDFAtoComplementDFA();
  for (int i = 0; i < dfa_auto2.states.size(); i++) {
    ASSERT_NE(dfa_auto1.states[i].is_final, dfa_auto2.states[i].is_final);
  }
}

bool TransitionExists(const State& state, const std::string& input_word, int destination_state_index) {
  for (auto& transition : state.transitions) {
    if (transition.input == input_word && destination_state_index == transition.destination_state_index) {
      return true;
    }
  }
  return false;
}

TEST(Automata_test, MinDFA_creation) {
  Automata automata = Automata(states_amount, starting_state_index, final_states_indexes);
  AutomataTransitionsInserting(automata);
  automata.DeleteEpsilonTransitions();
  DFA dfa = DFA(automata);
  Automata dfa_auto = Automata(dfa);
  dfa_auto.DFAtoFullDFA();
  dfa_auto.FullDFAtoMinDFA();
  EXPECT_EQ(true, TransitionExists(dfa_auto.states[0], "a", 0));
  EXPECT_EQ(true, TransitionExists(dfa_auto.states[0], "b", 0));
  EXPECT_EQ(true, TransitionExists(dfa_auto.states[1], "b", 0));
  EXPECT_EQ(true, TransitionExists(dfa_auto.states[1], "a", 2));
  EXPECT_EQ(true, TransitionExists(dfa_auto.states[2], "a", 2));
  EXPECT_EQ(true, TransitionExists(dfa_auto.states[2], "b", 1));
}

TEST(Automata_test, RegularExpression_generation) {
  Automata automata = Automata(states_amount, starting_state_index, final_states_indexes);
  AutomataTransitionsInserting(automata);
  automata.DeleteEpsilonTransitions();
  DFA dfa = DFA(automata);
  Automata dfa_auto = Automata(dfa);
  dfa_auto.DFAtoFullDFA();
  dfa_auto.FullDFAtoMinDFA();
  EXPECT_EQ(regex, dfa_auto.FDFAtoRegularExpression());
}

