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

  /*
  cout << "nums" << endl;
  for (int n : nums) {
    cout << n << endl;
  }
  cout << "===" << endl;
  */
  while (1) {
    int opcode = nums[idx] % 100;
    int modes = nums[idx] / 100;
    if (opcode == 99) {
      break;
    }

    // cout << "opcode: " << opcode << endl;

    int a, b, c;
    switch(opcode) {
      case 1:
      case 2:
        a = interpret(nums, nums[idx + 1], modes % 10);
        b = interpret(nums, nums[idx + 2], (modes/10) % 10);
        c = nums[idx + 3];
        /*
        cout << "a: " << a << endl;
        cout << "mode a: " << modes % 10 << endl;
        cout << "b: " << b << endl;
        cout << "mode b: " << (modes/10) % 10 << endl;
        cout << "c: " << c << endl;
        cout << "mode c: " << (modes/100) % 10 << endl;
        */

        nums[c] = (opcode == 1) ? a + b : a * b;
        idx += 4;
        break;

      case 3:
        cout << "> ";
        cin >> nums[nums[idx + 1]];
        idx += 2;
        break;

      case 4:
        cout << nums[nums[idx + 1]];
        idx += 2;
        break;
    }
  }
  /*
  cout << "NUMS" << endl;
  for (int n : nums) {
    cout << n << endl;
  }
  cout << "===" << endl;
  */
  return nums[0];
}

