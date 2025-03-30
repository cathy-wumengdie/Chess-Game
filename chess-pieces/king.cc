#include "king.h"

using namespace std;

King::King(const pair<int, int> position, const Color::Color color)
    : ChessPiece{position, Piece::King, color, Points::King},
      moved{false},
      symbol{color == Color::White ? 'K' : 'k'} {}

Piece::Id King::getId() const { return this->pieceId; }

Points::Points King::getValue() const { return this->value; }

char King::getSymbol() const { return symbol; }

bool King::getMoved() const { return moved; }

void King::setMoved() { moved = true; }
