#ifndef KING_H
#define KING_H
#include <utility>

#include "chessPiece.h"

using namespace std;

class King : public ChessPiece {
  bool moved;
  char symbol;

 public:
  King(const pair<int, int> position, const Color::Color color);
  ~King() = default;

  Piece::Id getId() const override;

  Points::Points getValue() const override;

  char getSymbol() const override;

  bool getMoved() const;

  void setMoved();
};

#endif
