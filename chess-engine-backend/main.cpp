#include "Board.h"

#include <iostream>
#include <string>

int main() {
  Board b;
  b.init();
  size_t i = 0;
  while (true) {
    std::cerr << "Ready to read..." << std::endl;
    json msg;
    std::cin >> msg;
    std::cerr << "Received " << msg.dump(4) << std::endl;
    std::cout << b.jsonify() << std::endl;
  }
}