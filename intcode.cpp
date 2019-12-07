#include "intcode.h"

const int POSITION = 0;
const int IMMEDIATE = 1;

int interpret(vector<int>& program, int val, int mode) {
  if (mode == POSITION) {
    return program[val];
  } else if (mode == IMMEDIATE) {
    return val;
  }
  cout << "Invalid mode: " << mode << endl;
  return -1;
}

State run(State state, vector<int> inputs) {
  vector<int> program = state.program;
  int idx = state.idx;
  vector<int> output = state.output;

  int input_ctr = 0;
  while (1) {
    int opcode = program[idx] % 100;
    int modes = program[idx] / 100;
    if (opcode == 99) {
      break;
    }

    int a, b;
    a = interpret(program, program[idx + 1], modes % 10);
    b = interpret(program, program[idx + 2], (modes/10) % 10);

    switch(opcode) {
      case 1:
      case 2:
        program[program[idx + 3]] = (opcode == 1) ? a + b : a * b;
        idx += 4;
        break;
      case 3:
        if (input_ctr < inputs.size()) {
          program[program[idx + 1]] = inputs[input_ctr++];
        } else {
          return State(false, idx, program, output);
        }
        idx += 2;
        break;
      case 4:
        output.push_back(a);
        idx += 2;
        break;
      case 5:
        idx = (a != 0) ? b : idx + 3;
        break;
      case 6:
        idx = (a == 0) ? b : idx + 3;
        break;
      case 7:
        program[program[idx + 3]] = a < b ? 1 : 0;
        idx += 4;
        break;
      case 8:
        program[program[idx + 3]] = a == b ? 1 : 0;
        idx += 4;
        break;
    }
  }
  return State(true, idx, program, output);
}
