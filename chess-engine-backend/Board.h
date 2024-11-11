#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>

using std::uint64_t;

enum Pieces {
    KING, 
    QUEEN,
    ROOKS,
    BISHOPS,
    KNIGHTS,
    PAWNS
};

class Board {

    std::string int_to_piece(unsigned int p, char color) {
        if (p == 0) {
            return color == 'b' ? "♚" : "♔";
        }
        else if (p == 1) {
            return color == 'b' ? "♛" : "♕";
        }
        else if (p == 2) {
            return color == 'b' ? "♜" : "♖";
        }
        else if (p == 3) {
            return color == 'b' ? "♝" : "♗";
        }
        else if (p == 4) {
            return color == 'b' ? "♞" : "♘";
        }
        else if (p == 5) {
            return color == 'b' ? "♟" : "♙";
        }
        return " ";
    }

    public: 
        uint64_t w_pieces[6];
        uint64_t b_pieces[6];
    
    void init() {
        
    }

    void print() {
        using std::cout;
        std::vector<std::string> board(64, ".");
        for (size_t i = 0; i < 64; ++i) {
            uint64_t pos = static_cast<uint64_t>(1) << i; // cast is necessary
            for (size_t p = 0; p < 6; ++p) {
                // janky but just doing this for now until i actually add a gui, it does not do any error checking
                // e.g if both w_pieces[king] == 1 and b_pwieces[king] == 1 it will overwrite
                if (w_pieces[p] == pos) {
                    board[i] = int_to_piece(p, 'w');
                    break;
                }
                if (b_pieces[p] == pos) {
                    board[i] = int_to_piece(p, 'b');
                    break;
                }
            }
        }
        for (size_t i = 0; i < 64; ++i) {
            if (i % 8 == 0 && i != 0) {
                std::cout << "\n";
            }
            std::cout << board[i];
        }
        std::cout << "\n";
    }
};