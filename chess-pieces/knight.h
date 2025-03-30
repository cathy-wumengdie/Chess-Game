#ifndef KNIGHT_H
#define KNIGHT_H
#include <utility>

#include "chessPiece.h"

using namespace std;

class Knight : public ChessPiece {
  char symbol;

 public:
  Knight(const pair<int, int> position, const Color::Color color);
  ~Knight() = default;

  Piece::Id getId() const override;

  Points::Points getValue() const override;

  char getSymbol() const override;
};

#endif
