#include "Board.h"

#include <chrono> 
#include <thread>

#include <iostream>
#include <string>

int main() {
  Board b;
  b.init();
  size_t i = 0;
  while (true) {
    std::cout << "Engine listening ... " << i++ << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    std::string in;
    std::cout << "waiting for client input: ";
    std::cin >> in;
    std::cout << "client said: " << in << std::endl;
  }
}