#ifndef QUEEN_H
#define QUEEN_H
#include <utility>

#include "chessPiece.h"

using namespace std;

class Queen : public ChessPiece {
  char symbol;

 public:
  Queen(const pair<int, int> position, const Color::Color color);
  ~Queen() = default;

  Piece::Id getId() const override;

  Points::Points getValue() const override;

  char getSymbol() const override;
};

#endif
