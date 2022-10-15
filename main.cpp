#include <iostream>
#include <utility>
#include <vector>
#include "src/Automata.h"
#include "src/DFA.h"


int main() {
  int states_amount;
  std::cout << "Enter states amount: ";
  std::cin >> states_amount;

  std::cout << "Enter starting states index: ";
  int starting_state_index;
  std::cin >> starting_state_index;


  int final_states_amount;
  std::cout << "Enter final states amount: ";
  std::cin >> final_states_amount;

  std::cout << "Enter final states indexes: ";
  std::vector<int> final_states_indexes;
  for (int i = 0; i < final_states_amount; i++) {
    int index;
    std::cin >> index;
    final_states_indexes.push_back(index);
  }

  Automata automata = Automata(states_amount, starting_state_index, final_states_indexes);
  std::cout << "ENTERING TRANSITIONS..";
  std::cout << std::endl;

  for (int i = 0; i < states_amount; i++) {
    std::cout << "Enter amount of transitions for state " << i << ": ";
    int amount_of_transitions;
    std::cin >> amount_of_transitions;
    for (int j = 0; j < amount_of_transitions; j++) {
      std::cout << "Enter input word and destination state index for transition: ";
      std::string input;
      std::cin >> input;
      int index;
      std::cin >> index;
      automata.InsertTransition(i, input, index);
    }
  }
  std::cout << "Entering transitions finished.. ";
  std::cout << std::endl;
  automata.PrintAutomata();
  DFA dfa = DFA(automata);
  Automata dfa_auto = Automata(dfa);
  std::cout << "......DFA......" << std::endl;
  dfa_auto.PrintAutomata();
  std::cout << "......FULL DFA......" << std::endl;
  dfa_auto.DFAtoFullDFA();
  dfa_auto.PrintAutomata();
  dfa_auto.FullDFAtoMinDFA();
  std::cout << "......MIN FULL DFA......" << std::endl;
  dfa_auto.PrintAutomata();
  return 0;
}
