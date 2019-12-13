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

long read_memory(unordered_map<long, long>& memory, long idx) {
  if (memory.find(idx) != memory.end()) {
    return memory.at(idx);
  }
  return 0;
}

long read(unordered_map<long, long>& memory, long val, long mode, long relative_base) {
  switch(mode) {
    case POSITION:
      return read_memory(memory, val);
    case IMMEDIATE:
      return val;
    case RELATIVE:
      return read_memory(memory, val + relative_base);
  }
  cout << "Invalid mode: " << mode << endl;
  return -1;
}

int write_idx(long val, long mode, long relative_base) {
  return (mode == RELATIVE) ? val + relative_base : val;
}

State run(State state, vector<long> inputs) {
  unordered_map<long, long> memory = state.memory;
  long pc = state.pc;
  long relative_base = state.relative_base;
  vector<long> output = state.output;

  long input_ctr = 0;
  while (1) {
    long opcode = memory.at(pc) % 100;
    long modes = memory.at(pc) / 100;
    long mode_a = modes % 10, mode_b = (modes/10) % 10, mode_c = (modes/100) % 10;
    if (opcode == 99) {
      break;
    }

    long a, b, w_idx;
    switch(opcode) {
      case 1:
      case 2:
        a = read(memory, memory.at(pc + 1), mode_a, relative_base);
        b = read(memory, memory.at(pc + 2), mode_b, relative_base);
        w_idx = write_idx(memory.at(pc + 3), mode_c, relative_base);
        memory[w_idx] = (opcode == 1) ? a + b : a * b;
        pc += 4;
        break;
      case 3:
        if (input_ctr < inputs.size()) {
          w_idx = write_idx(memory.at(pc + 1), mode_a, relative_base);
          memory[w_idx] = inputs[input_ctr++];
        } else {
          return State(false, pc, relative_base, memory, output);
        }
        pc += 2;
        break;
      case 4:
        a = read(memory, memory.at(pc + 1), mode_a, relative_base);
        output.push_back(a);
        pc += 2;
        break;
      case 5:
        a = read(memory, memory.at(pc + 1), mode_a, relative_base);
        b = read(memory, memory.at(pc + 2), mode_b, relative_base);
        pc = (a != 0) ? b : pc + 3;
        break;
      case 6:
        a = read(memory, memory.at(pc + 1), mode_a, relative_base);
        b = read(memory, memory.at(pc + 2), mode_b, relative_base);
        pc = (a == 0) ? b : pc + 3;
        break;
      case 7:
        a = read(memory, memory.at(pc + 1), mode_a, relative_base);
        b = read(memory, memory.at(pc + 2), mode_b, relative_base);
        w_idx = write_idx(memory.at(pc + 3), mode_c, relative_base);
        memory[w_idx] = a < b ? 1 : 0;
        pc += 4;
        break;
      case 8:
        a = read(memory, memory.at(pc + 1), mode_a, relative_base);
        b = read(memory, memory.at(pc + 2), mode_b, relative_base);
        w_idx = write_idx(memory.at(pc + 3), mode_c, relative_base);
        memory[w_idx] = a == b ? 1 : 0;
        pc += 4;
        break;
      case 9:
        a = read(memory, memory.at(pc + 1), mode_a, relative_base);
        relative_base += a;
        pc += 2;
        break;
    }
  }
  return State(true, pc, relative_base, memory, output);
}
