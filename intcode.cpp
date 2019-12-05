#include "intcode.h"

const int POSITION = 0;
const int IMMEDIATE = 1;

int interpret(vector<int>& nums, int val, int mode) {
  if (mode == POSITION) {
    return nums[val];
  } else if (mode == IMMEDIATE) {
    return val;
  }
  return -1;
}

int run(vector<int> nums) {
  int idx = 0;
  int ctr = 0;

  while (1) {
    ctr += 1;
    int opcode = nums[idx] % 100;
    int modes = nums[idx] / 100;
    if (opcode == 99) {
      break;
    }

    int a, b, in;
    a = interpret(nums, nums[idx + 1], modes % 10);
    b = interpret(nums, nums[idx + 2], (modes/10) % 10);

    switch(opcode) {
      case 1:
      case 2:
        nums[nums[idx + 3]] = (opcode == 1) ? a + b : a * b;
        idx += 4;
        break;

      case 3:
        cout << "> ";
        cin >> in;
        nums[nums[idx + 1]] = in;
        idx += 2;
        break;

      case 4:
        cout << nums[nums[idx + 1]] << endl;
        idx += 2;
        break;
    }
  }
  return nums[0];
}

