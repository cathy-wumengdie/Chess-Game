#ifndef BISHOP_H
#define BISHOP_H
#include <utility>

#include "chessPiece.h"

using namespace std;

class Bishop : public ChessPiece {
  char symbol;

 public:
  Bishop(const pair<int, int> position, const Color::Color color);
  ~Bishop() = default;

  Piece::Id getId() const override;

  Points::Points getValue() const override;

  char getSymbol() const override;
};

#endif
