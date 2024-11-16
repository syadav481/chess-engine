#pragma once

#include "nlohmann/json_fwd.hpp"
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using std::uint64_t;
using json = nlohmann::json;

enum Pieces { KING, QUEEN, ROOKS, BISHOPS, KNIGHTS, PAWNS };

class Board {

  /*
   * EFFECTS: Just sets the bit at the specified index for the specified color
   * to 1 performs NO error checking
   */
  void set_position(Pieces p, char color, size_t board_index) {
    if (color == 'b') {
      b_pieces[p] |= (static_cast<uint64_t>(1) << board_index);
    } else {
      w_pieces[p] |= (static_cast<uint64_t>(1) << board_index);
    }
  }

  // TODO: Get rid of this when using an actual GUI
  std::string int_to_piece(unsigned int p, char color) {
    if (p == 0) {
      return color == 'b' ? "♚" : "♔";
    } else if (p == 1) {
      return color == 'b' ? "♛" : "♕";
    } else if (p == 2) {
      return color == 'b' ? "♜" : "♖";
    } else if (p == 3) {
      return color == 'b' ? "♝" : "♗";
    } else if (p == 4) {
      return color == 'b' ? "♞" : "♘";
    } else if (p == 5) {
      return color == 'b' ? "♟" : "♙";
    }
    return " ";
  }

  /*
   * REQUIRES: a valid FEN
   * MODIFIES: the internal board state
   *
   *  https://www.chess.com/terms/fen-chess
   */
  void parse_fen(const std::string &fen) {
    // an example fen:
    // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    std::stringstream ss(fen);
    // Handle the pieces;
    std::string pieces;
    ss >> pieces;
    {
      std::stringstream ss(pieces);
      size_t board_index = 0; // [0, 63]
      char c;
      while (ss >> c) {
        switch (c) {
        case 'r':
          set_position(ROOKS, 'b', board_index++);
          break;
        case 'n':
          set_position(KNIGHTS, 'b', board_index++);
          break;
        case 'b':
          set_position(BISHOPS, 'b', board_index++);
          break;
        case 'q':
          set_position(QUEEN, 'b', board_index++);
          break;
        case 'k':
          set_position(KING, 'b', board_index++);
          break;
        case 'p':
          set_position(PAWNS, 'b', board_index++);
          break;
        case 'R':
          set_position(ROOKS, 'w', board_index++);
          break;
        case 'N':
          set_position(KNIGHTS, 'w', board_index++);
          break;
        case 'B':
          set_position(BISHOPS, 'w', board_index++);
          break;
        case 'Q':
          set_position(QUEEN, 'w', board_index++);
          break;
        case 'K':
          set_position(KING, 'w', board_index++);
          break;
        case 'P':
          set_position(PAWNS, 'w', board_index++);
          break;
        case '/':
          break;
        // c is a number
        default:
          board_index += static_cast<uint64_t>(c - '0');
          break;
        }
      }
    }
  }

public:
  std::vector<uint64_t> w_pieces;
  std::vector<uint64_t> b_pieces;

  Board() {
    w_pieces.resize(6);
    b_pieces.resize(6);
  }

  // REQUIRES: A FEN
  // MODIFIES: the board
  // EFFECTS:  wipes the board clean and applies a new FEN.
  void init(const std::string &s) {
    for (size_t i = 0; i < 6; ++i) {
      w_pieces[i] = 0;
      b_pieces[i] = 0;
    }
    parse_fen(s);
  }

  void init() {
    std::string fen =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    parse_fen(fen);
  }

  // THIS SHOULD NOT HAVE ANY SIDEEFFECTS
  std::string print() {
    std::vector<std::string> board(64, ".");
    std::stringstream ss;
    ss << std::boolalpha;
    for (size_t i = 0; i < 64; ++i) {
      uint64_t pos = static_cast<uint64_t>(1) << i; // cast is necessary
      for (size_t p = 0; p < 6; ++p) {
        // janky but just doing this for now until i actually add a gui, it does
        // not do any error checking e.g if both w_pieces[king] == 1 and
        // b_pieces[king] == 1 it will overwrite
        if (static_cast<uint64_t>(w_pieces[p] & pos) == pos) {
          board[i] = int_to_piece(p, 'w');
          break;
        }
        if (static_cast<uint64_t>(b_pieces[p] & pos) == pos) {
          board[i] = int_to_piece(p, 'b');
          break;
        }
      }
    }
    size_t rank = 8;
    for (size_t i = 0; i < 64; ++i) {
      if (i % 8 == 0 && i != 0) {
        ss << "\n";
      }
      ss << board[i];
      if (i % 8 == 7) {
        ss << rank-- << std::endl;
      }
    }
    for (char i = 'a'; i <= 'h'; ++i) {
      ss << i;
    }
    ss << "\n";
    return ss.str();
  }

  /*
   * converts the board into a json object as per the API of the client
   * defined by ME
   */
  json jsonify() {
    json b = {{"board stringified: ", print()}};
    return b;
  }
};

/*

an example FEN: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1

8 | 0 0 0 0 0 0 0 0
7 | 0 0 0 0 0 0 0 0
6 | 0 0 0 0 0 0 0 0
5 | 0 0 0 0 0 0 0 0
4 | 0 0 0 0 0 0 0 0
3 | 0 0 0 0 0 0 0 0
2 | 0 0 0 0 0 0 0 0
1 | 0 0 0 0 0 0 0 0
  -----------------
    a b c d e f g h

*/