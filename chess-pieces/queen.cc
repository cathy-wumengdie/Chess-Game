#include "queen.h"

using namespace std;

Queen::Queen(const pair<int, int> position, const Color::Color color)
    : ChessPiece{position, Piece::Queen, color, Points::Queen},
      symbol{color == Color::White ? 'Q' : 'q'} {}

Piece::Id Queen::getId() const { return this->pieceId; }

Points::Points Queen::getValue() const { return this->value; }

char Queen::getSymbol() const { return symbol; }
