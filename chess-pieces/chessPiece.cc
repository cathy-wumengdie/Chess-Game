#include "./chessPiece.h"

// imports for forward declaration:
#include "../chess-board/chessBoard.h"

ChessPiece::ChessPiece(const pair<int, int> position, const Piece::Id pieceId,
                       const Color::Color color, const Points::Points value)
    : position{position}, pieceId{pieceId}, color{color}, value{value} {}

pair<int, int> ChessPiece::getPosition() const { return this->position; }

void ChessPiece::setPosition(pair<int, int> position) {
  this->position = position;
}

Color::Color ChessPiece::getColor() const { return color; }
