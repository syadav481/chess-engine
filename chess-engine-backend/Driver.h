#include "Board.h"
#include <iostream>

class Driver {
  Board b;

public:
  void process_msg(json msg) {
    if (!msg.contains("OPERATION")) {
      std::cerr << "Received msg without OPERATION field: " << msg.dump(4)
                << std::endl;
      return;
    }
    std::string operation = msg["OPERATION"];
    if (operation == "INIT") {
      if (msg.contains("FEN")) {
        std::cerr << "board initialized with FEN: " << msg["FEN"] << std::endl;
        b.init(msg["FEN"]);
      } else {
        std::cerr << "board initialized with default FEN: "
                     "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
                  << std::endl;
        b.init();
      }
      json output = {{"board", b.jsonify()}};
      std::cout << output << std::endl;
    }
  }
};