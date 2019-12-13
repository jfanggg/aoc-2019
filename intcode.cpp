#include "intcode.h"

const long POSITION = 0;
const long IMMEDIATE = 1;
const long RELATIVE = 2;

unordered_map<long, long> parse_input() {
  unordered_map<long, long> memory;
  long memory_ctr = 0;
  string s;

  while(std::getline(cin, s, ',')) {
    memory[memory_ctr++] = stol(s);
  }
  return memory;
}

long read(unordered_map<long, long>& memory, long idx) {
  if (memory.find(idx) != memory.end()) {
    return memory.at(idx);
  }
  return 0;
}

long interpret(unordered_map<long, long>& memory, long val, long mode, long relative_base) {
  switch(mode) {
    case POSITION:
      return read(memory, val);
    case IMMEDIATE:
      return val;
    case RELATIVE:
      return read(memory, val + relative_base);
  }
  cout << "Invalid mode: " << mode << endl;
  return -1;
}

void write(unordered_map<long, long>& memory, long idx, long val, long mode, long relative_base) {
  if (mode == RELATIVE) {
    idx += relative_base;
  }
  memory[idx] = val; 
}

State run(State state, vector<long> inputs) {
  unordered_map<long, long> memory = state.memory;
  long pc = state.pc;
  long relative_base = state.relative_base;
  vector<long> output = state.output;

  long input_ctr = 0;
  while (1) {
    long opcode = memory.at(pc) % 100;
    // cout << "opcode: " << opcode << endl;
    long modes = memory.at(pc) / 100;
    // cout << "mode: " << modes << endl;
    long mode_a = modes % 10, mode_b = (modes/10) % 10, mode_c = (modes/100) % 10;
    if (opcode == 99) {
      break;
    }

    long a, b;
    switch(opcode) {
      case 1:
      case 2:
        a = interpret(memory, memory.at(pc + 1), mode_a, relative_base);
        b = interpret(memory, memory.at(pc + 2), mode_b, relative_base);
        write(memory, memory.at(pc + 3), (opcode == 1) ? a + b : a * b, mode_c, relative_base);
        pc += 4;
        break;
      case 3:
        if (input_ctr < inputs.size()) {
          write(memory, memory.at(pc + 1), inputs[input_ctr++], mode_a, relative_base);
        } else {
          return State(false, pc, relative_base, memory, output);
        }
        pc += 2;
        break;
      case 4:
        a = interpret(memory, memory.at(pc + 1), mode_a, relative_base);
        output.push_back(a);
        pc += 2;
        break;
      case 5:
        a = interpret(memory, memory.at(pc + 1), mode_a, relative_base);
        b = interpret(memory, memory.at(pc + 2), mode_b, relative_base);
        pc = (a != 0) ? b : pc + 3;
        break;
      case 6:
        a = interpret(memory, memory.at(pc + 1), mode_a, relative_base);
        b = interpret(memory, memory.at(pc + 2), mode_b, relative_base);
        pc = (a == 0) ? b : pc + 3;
        break;
      case 7:
        a = interpret(memory, memory.at(pc + 1), mode_a, relative_base);
        b = interpret(memory, memory.at(pc + 2), mode_b, relative_base);
        write(memory, memory.at(pc + 3), a < b ? 1 : 0, mode_c, relative_base);
        pc += 4;
        break;
      case 8:
        a = interpret(memory, memory.at(pc + 1), mode_a, relative_base);
        b = interpret(memory, memory.at(pc + 2), mode_b, relative_base);
        write(memory, memory.at(pc + 3), a == b ? 1 : 0, mode_c, relative_base);
        pc += 4;
        break;
      case 9:
        a = interpret(memory, memory.at(pc + 1), mode_a, relative_base);
        relative_base += a;
        pc += 2;
        break;
    }
  }
  return State(true, pc, relative_base, memory, output);
}
