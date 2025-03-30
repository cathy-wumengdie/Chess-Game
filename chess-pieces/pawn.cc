#include "pawn.h"

using namespace std;

Pawn::Pawn(const pair<int, int> position, const Color::Color color)
    : ChessPiece{position, Piece::Pawn, color, Points::Pawn},
      moved{false},
      symbol{color == Color::White ? 'P' : 'p'} {}

Piece::Id Pawn::getId() const { return this->pieceId; }

Points::Points Pawn::getValue() const { return this->value; }

char Pawn::getSymbol() const { return symbol; }

bool Pawn::getMoved() const { return moved; }

void Pawn::setMoved() { moved = true; }
