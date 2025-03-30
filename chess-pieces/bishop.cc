#include "bishop.h"

using namespace std;

Bishop::Bishop(const pair<int, int> position, const Color::Color color)
    : ChessPiece{position, Piece::Bishop, color, Points::Bishop},
      symbol{color == Color::White ? 'B' : 'b'} {}

Piece::Id Bishop::getId() const { return this->pieceId; }

Points::Points Bishop::getValue() const { return this->value; }

char Bishop::getSymbol() const { return symbol; }
