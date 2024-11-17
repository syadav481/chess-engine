#pragma once

#include "nlohmann/json_fwd.hpp"
#include "utils.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

using std::uint64_t;
using json = nlohmann::json;

// TODO: Separate all the header files into header and source files at some
// point

enum Pieces : size_t { KING, QUEEN, ROOKS, BISHOPS, KNIGHTS, PAWNS };

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
  const std::string int_to_piece(unsigned int p, char color) const {
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
      // TODO: Set the remaining fields of the FEN
    }
  }

public:
  std::vector<uint64_t> w_pieces;
  std::vector<uint64_t> b_pieces;

  std::string
      en_passant_captures; // a string in algebraic notation representing
                           // squares that are en-passant targets
  uint32_t halfmove_clock = 0;
  uint32_t fullmove_clock = 0;
  char active_color = 'w'; // can be one of w or b.
  bool w_queen_side_castle = false;
  bool w_king_side_castle = false;
  bool b_queen_side_castle = false;
  bool b_king_side_castle = false;

  Board() {
    w_pieces.resize(6);
    b_pieces.resize(6);
  }

  Board(const std::string &s) {
    w_pieces.resize(6);
    b_pieces.resize(6);

    for (size_t i = 0; i < NUM_PIECE_TYPES; ++i) {
      w_pieces[i] = 0;
      b_pieces[i] = 0;
    }
    parse_fen(s);
  }

  // REQUIRES: A FEN
  // MODIFIES: the board
  // EFFECTS:  wipes the board clean and applies a new FEN.
  void set_board(const std::string &s) {
    for (size_t i = 0; i < NUM_PIECE_TYPES; ++i) {
      w_pieces[i] = 0;
      b_pieces[i] = 0;
    }
    parse_fen(s);
  }

  // THIS SHOULD NOT HAVE ANY SIDEEFFECTS
  const std::string to_string() const {
    std::vector<std::string> board(64, ".");
    std::stringstream ss;
    ss << std::boolalpha;
    for (size_t i = 0; i < NUM_SQUARES_ON_BOARD; ++i) {
      uint64_t pos = static_cast<uint64_t>(1) << i; // cast is necessary
      for (size_t p = 0; p < NUM_PIECE_TYPES; ++p) {
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
    for (size_t i = 0; i < NUM_SQUARES_ON_BOARD; ++i) {
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
};

void to_json(json &j, const Board &b) {
  std::cerr << "Asked to serialize board:" << std::endl;
  std::cerr << b.to_string() << std::endl;

  // TODO: Handle generating possible moves here (or anywhere else??)
  std::vector<Piece> pieces(64);
  for (size_t i = 0; i < NUM_SQUARES_ON_BOARD; ++i) {
    for (size_t piece_type = 0; i < NUM_PIECE_TYPES; ++i) {
      // if the location has a 1 e.g. the board at that location does have a
      // piece, based on the current piece type push it back and then break out.
      Piece p;
      if ((b.w_pieces[piece_type] & (1 << i)) ==
          (static_cast<size_t>(1) << i)) {
        p.color = 'w';
      } else if ((b.b_pieces[piece_type] & (1 << i)) ==
                 (static_cast<size_t>(1) << i)) {
        p.color = 'b';
      } else {
        p.name = "EMPTY";
      }
      switch (piece_type) {
      case KING:
        p.name = "KING";
        break;
      case QUEEN:
        p.name = "QUEEN";
        break;
      case ROOKS:
        p.name = "ROOKS";
        break;
      case BISHOPS:
        p.name = "BISHOPS";
        break;
      case KNIGHTS:
        p.name = "KNIGHTS";
        break;
      case PAWNS:
        p.name = "PAWNS";
        break;
      default:
        break;
      }
      if (p.name != "EMPTY") {
        pieces[i] = p;
        break;
      }
    }
  }
  j = json{{"BOARD", pieces}};
}

/*

https://www.chess.com/terms/fen-chess
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