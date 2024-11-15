#include "Board.h"

#include <iostream>
#include <string>

int main() {
  Board b;
  b.init();
  size_t i = 0;
  while (true) {
    std::cout << "Engine ready to read..." << std::endl;
    std::string in;
    std::string msg;
    char c;
    while (std::cin >> c) {
      if (c == '#') {
        msg += in;
        msg += ' ';
        in = "";
      } else if (c == '!') {
        msg += in;
        break;
      } else {
        in += c;
      }
    }
    std::cout << "ENGINE: Received " << msg << std::endl;
  }
}