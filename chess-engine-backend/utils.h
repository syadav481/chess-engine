#pragma once

#include <bitset>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

std::bitset<64> int_to_bin(uint64_t u) { return std::bitset<64>(u); }

const size_t NUM_PIECE_TYPES = 6;
const size_t NUM_SQUARES_ON_BOARD = 64;

struct Piece {
  std::string
      name;   // in the set KING, QUEEN, ROOKS, BISHOPS, KNIGHTS, PAWNS, EMPTY
  char color; // b or w
  std::vector<std::string>
      available_moves; // a string storing the positions a piece can
                       // currently move in algebraic notation

  Piece() {
    name = "EMPTY";
    color = '\0';
  }
};

void to_json(json &j, const Piece &p) {
  j = json{{"name", p.name},
           {"color", p.color},
           {"available_moves", p.available_moves}};
}