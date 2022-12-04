#ifndef FORMALS_TESTS_NFA_INPUT_DATA_H_
#define FORMALS_TESTS_NFA_INPUT_DATA_H_

const int states_amount = 6;
const int starting_state_index = 0;
const std::vector<int> final_states_indexes = {5};

const std::vector<std::vector<std::string>> state_transitions_inputs = {{"e", "b", "e"}, {"b"}, {"a", "b"}, {"a", "b", "b"}, {"b"}, {"a", "b"}};
const std::vector<std::vector<int>> state_transitions_destinations = {{2, 4, 1}, {5}, {3, 3}, {3, 3, 4}, {5}, {5, 5}};

const std::string regex = "(b((a + b))* + a((a + ba))*bb((a + b))*)";

#endif //FORMALS_TESTS_NFA_INPUT_DATA_H_
