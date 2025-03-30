#include "./chessBoard.h"

#include <iostream>
#include <vector>

// imports for forward declaration:
#include "../chess-pieces/chessPiece.h"
#include "../chess-pieces/king.h"
#include "../players/player.h"

ChessBoard::ChessBoard()
    : pieces{rows, vector<shared_ptr<ChessPiece>>(cols, nullptr)},
      lastMove{Move{Move::InvalidPos, Move::InvalidPos}} {}

ChessBoard::ChessBoard(shared_ptr<Player> player1, shared_ptr<Player> player2)
    : players{player1, player2},
      pieces{rows, vector<shared_ptr<ChessPiece>>(cols, nullptr)},
      lastMove{Move{Move::InvalidPos, Move::InvalidPos}} {
  for (auto piece : player1->getPieces()) {
    setPosition(piece->getPosition(), piece);
  }
  for (auto piece : player2->getPieces()) {
    setPosition(piece->getPosition(), piece);
  }
}

shared_ptr<ChessPiece> ChessBoard::getKing(Color::Color color) const {
  shared_ptr<ChessPiece> king;
  for (unsigned int i = 0; i < pieces.size(); i++) {
    for (unsigned int j = 0; j < pieces.at(i).size(); j++) {
      if (pieces.at(i).at(j) && pieces.at(i).at(j)->getColor() == color && pieces.at(i).at(j)->getId() == Piece::King) 
      king = pieces.at(i).at(j);
    }
  } return king;
}

vector<vector<shared_ptr<ChessPiece>>> ChessBoard::getPieces() const { return pieces; }

vector<shared_ptr<ChessPiece>> ChessBoard::getPieces(Color::Color color) const {
  vector<shared_ptr<ChessPiece>> cPieces;
  for (unsigned int i = 0; i < pieces.size(); i++) {
    for (unsigned int j = 0; j < pieces.at(i).size(); j++) {
      if (pieces.at(i).at(j) && pieces.at(i).at(j)->getColor() == color) cPieces.emplace_back(pieces.at(i).at(j));
      //added checking if piece is nullptr
    }
  } return cPieces;
}

vector<shared_ptr<ChessPiece>> ChessBoard::getCapturedPieces(
    Color::Color color) const {
  return color == players.first->getColor()
             ? players.first->getCapturedPieces()
             : players.second->getCapturedPieces();
}

void ChessBoard::setCaptured(pair<int, int> position) {
  shared_ptr<ChessPiece> piece = getPosition(position);
  if (piece == nullptr) {
    return;
  }
  setPosition(position, nullptr);
  if (piece->getColor() == players.first->getColor()) {
    players.first->makeCaptured(piece);
  } else {
    players.second->makeCaptured(piece);
  }
}

void ChessBoard::undoCapture(pair<int, int> position, Color::Color color) {
  shared_ptr<Player> player =
      players.first->getColor() == color ? players.first : players.second;
  shared_ptr<ChessPiece> piece = player->getCapturedPiece(position);
  if (piece == nullptr) {
    return;
  }
  player->undoCapture(piece);
  setPosition(position, player->getPiece(position));
}

void ChessBoard::setPosition(pair<int, int> position,
                             shared_ptr<ChessPiece> piece) {
  // set board position
  pieces.at(position.first).at(position.second) = piece;
  // set piece position
  if (piece) piece->setPosition(position);
}

shared_ptr<ChessPiece> ChessBoard::getPosition(pair<int, int> position) const {
  if (position.first < 0 || position.first > 7 || position.second < 0 ||
      position.second > 7) {
    return nullptr;
  }
  // return the chess piece with the position
  return pieces.at(position.first).at(position.second);
}

int ChessBoard::getRows() const { return rows; }

int ChessBoard::getCols() const { return cols; }

bool ChessBoard::isMoveWithinBounds(pair<int, int> position) const {
  return position.first >= 0 && position.second < getRows() &&
         position.second >= 0 && position.second < getCols();
}

void ChessBoard::movePiece(Move move) {
  if (move.effect == SideEffect::Enpassant ||
      move.effect == SideEffect::Capture) {
    setCaptured(move.capturedPos);
    setPosition(move.endPos, getPosition(move.startPos));
    setPosition(move.startPos, nullptr);
  } else if (move.effect == SideEffect::Castle) {
    // set King
    setPosition(move.endPos, getPosition(move.startPos));
    setPosition(move.startPos, nullptr);
    // set Castle
    setPosition(move.endCastlePos, getPosition(move.startCastlePos));
    setPosition(move.startCastlePos, nullptr);
  } else {
    setPosition(move.endPos, getPosition(move.startPos));
    setPosition(move.startPos, nullptr);
  }
  setLastMove(move);
}

void ChessBoard::undoMove(Move move) {
  shared_ptr<ChessPiece> piece = getPosition(move.endPos);
  if (move.effect == SideEffect::Enpassant ||
      move.effect == SideEffect::Capture) {
    // reverse enpassant and capture
    // color of captured piece is the opposite of the piece that captured it
    Color::Color c =
        piece->getColor() == Color::White ? Color::Black : Color::White;
    shared_ptr<Player> player =
        c == players.first->getColor() ? players.first : players.second;

    setPosition(move.startPos, piece);
    setPosition(move.endPos, nullptr);
    undoCapture(move.capturedPos, c);
    setPosition(move.capturedPos, player->getPiece(move.capturedPos));
  } else if (move.effect == SideEffect::Castle) {
    // Reverse castling
    setPosition(move.startPos, piece);
    setPosition(move.endPos, nullptr);
    // Reverse Rook position
    setPosition(move.startCastlePos, getPosition(move.endCastlePos));
    setPosition(move.endCastlePos, nullptr);
  } else {
    // Reverse standard move
    setPosition(move.startPos, piece);
    setPosition(move.endPos, nullptr);
  }
}

Move ChessBoard::getLastMove() const { return lastMove; }

void ChessBoard::setLastMove(Move move) { lastMove = move; };

bool ChessBoard::isLegalMove(Move move, Color::Color color) {
  auto legalMoves = generateLegalMoves(color);
  return find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end();
}

// IMPORTANT: REVIEW
// Important dependency
// King pointer is always initialized
vector<Move> ChessBoard::generateLegalMoves(Color::Color color) {
  shared_ptr<ChessBoard> currentBoard = make_shared<ChessBoard>(*this);

  shared_ptr<Player> attackingPlayer;
  shared_ptr<Player> defendingPlayer;
  vector<Move> legalMoves;
  if (color == players.first->getColor()) {
    attackingPlayer = players.first;
    defendingPlayer = players.second;
  } else {
    attackingPlayer = players.second;
    defendingPlayer = players.first;
  }

  // Simulate all possible moves
  for (Move move : attackingPlayer->generateValidMoves(currentBoard)) {
    // execute attackingPlayer's move
    movePiece(move);

    // checking if attack puts player in checkmate
    vector<Move> responses = defendingPlayer->generateValidMoves(currentBoard);
    bool legalMoveFlag = true;
    for (Move response : responses) {
      // if the response is a capture of the king
      if (response.effect == SideEffect::Capture &&
          response.endPos == attackingPlayer->getKing()->getPosition()) {
        legalMoveFlag = false;
      }
    }
    if (legalMoveFlag) {
      legalMoves.emplace_back(move);
    }

    undoMove(move);
  }
  return legalMoves;
}

bool ChessBoard::anyPiecesInTheWay(pair<int,int> startPos, pair<int,int> endPos) const {
  if (startPos == endPos) return false;
  // Two positions are on the same row
  if (startPos.first == endPos.first) {
    if (startPos.second < endPos.second) {
      for (int i = startPos.second+1; i < endPos.second; i++){
        if (getPosition(pair<int,int>{startPos.first,i})) return true; //found a piece between the two positions
      }
    }
    if (startPos.second > endPos.second) {
      for (int i = endPos.second+1; i < startPos.second; i++){
        if (getPosition(pair<int,int>{startPos.first,i})) return true; //found a piece between the two positions
      }
    }
  }
  // Two positions are on the same column
  if (startPos.second == endPos.second) {
    if (startPos.first < endPos.first) {
      for (int i = startPos.first+1; i < endPos.first; i++){
        if (getPosition(pair<int,int>{i,startPos.second})) return true; //found a piece between the two positions
      }
    }
    if (startPos.first > endPos.first) {
      for (int i = endPos.first+1; i < startPos.first; i++){
        if (getPosition(pair<int,int>{i,startPos.second})) return true; //found a piece between the two positions
      }
    }
  }
  // Two positions are diagonal
  if (startPos.first < endPos.first && startPos.second < endPos.second) {
    for (unsigned int i = startPos.first+1, j = startPos.second+1; i < endPos.first && j < endPos.second; i++, j++){
      if (getPosition(pair<int,int>{i,j})) return true; //found a piece between the two positions
    }
  }
  if (startPos.first > endPos.first && startPos.second > endPos.second) {
    for (unsigned int i = endPos.first+1, j = endPos.second+1; i < startPos.first && j < startPos.second; i++, j++){
      if (getPosition(pair<int,int>{i,j})) return true; //found a piece between the two positions
    }
  }
  if (startPos.first < endPos.first && startPos.second > endPos.second) {
    for (unsigned int i = endPos.first-1, j = endPos.second+1; i > startPos.first && j < startPos.second; i--, j++){
      if (getPosition(pair<int,int>{i,j})) return true; //found a piece between the two positions
    }
  }
  if (startPos.first > endPos.first && startPos.second < endPos.second) {
    for (unsigned int i = endPos.first+1, j = endPos.second-1; i < startPos.first && j > startPos.second; i++, j--){
      if (getPosition(pair<int,int>{i,j})) return true; //found a piece between the two positions
    }
  } return false;
}
