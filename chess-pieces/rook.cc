#include "rook.h"

using namespace std;

Rook::Rook(const pair<int, int> position, const Color::Color color)
    : ChessPiece{position, Piece::Rook, color, Points::Rook},
      moved{false},
      symbol{color == Color::White ? 'R' : 'r'} {}

Piece::Id Rook::getId() const { return this->pieceId; }

Points::Points Rook::getValue() const { return this->value; }

char Rook::getSymbol() const { return symbol; }

bool Rook::getMoved() const { return moved; }

void Rook::setMoved() { moved = true; }
