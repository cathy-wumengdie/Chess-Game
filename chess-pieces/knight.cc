#include "knight.h"

using namespace std;

Knight::Knight(const pair<int, int> position, const Color::Color color)
    : ChessPiece{position, Piece::Knight, color, Points::Knight},
      symbol{color == Color::White ? 'N' : 'n'} {}

Piece::Id Knight::getId() const { return this->pieceId; }

Points::Points Knight::getValue() const { return this->value; }

char Knight::getSymbol() const { return symbol; }
