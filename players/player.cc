#include "player.h"

#include "../chess-pieces/bishop.h"
#include "../chess-pieces/king.h"
#include "../chess-pieces/knight.h"
#include "../chess-pieces/pawn.h"
#include "../chess-pieces/queen.h"
#include "../chess-pieces/rook.h"

/* To do:
 * 2. Check
 * 3. Checkmate
 * 4. Implement promotion
 * 5. Do more indepth-capture tests
 */

using namespace std;

Player::Player(Color::Color color, string id, int level)
    : color{color}, id{id}, level{level} {}       //////cathy added level

//Player::~Player() {}

void Player::reset() { chessPieces.clear(); }

Color::Color Player::getColor() const { return color; }

string Player::getID() const { return id; }

int Player::getLevel() const { return level; }

vector<shared_ptr<ChessPiece>> Player::getCapturedPieces() const {
  return capturedPieces;
}

void Player::createPiece(Piece::Id pieceId, pair<int, int> position) {
  switch (pieceId) {
    case Piece::Pawn:
      chessPieces.emplace_back(make_shared<Pawn>(position, getColor()));
      break;
    case Piece::Rook:
      chessPieces.emplace_back(make_shared<Rook>(position, getColor()));
      break;
    case Piece::Knight:
      chessPieces.emplace_back(make_shared<Knight>(position, getColor()));
      break;
    case Piece::Bishop:
      chessPieces.emplace_back(make_shared<Bishop>(position, getColor()));
      break;
    case Piece::Queen:
      chessPieces.emplace_back(make_shared<Queen>(position, getColor()));
      break;
    case Piece::King:
      chessPieces.emplace_back(make_shared<King>(position, getColor()));
      break;
  }
}

shared_ptr<ChessPiece> Player::getPiece(pair<int, int> position) const {
  for (auto piece : chessPieces) {
    if (piece->getPosition() == position) {
      return piece;
    }
  }
  return nullptr;
}

shared_ptr<ChessPiece> Player::getCapturedPiece(pair<int, int> position) const {
  for (auto piece : capturedPieces) {
    if (piece->getPosition() == position) {
      return piece;
    }
  }
  return nullptr;
}

void Player::makeCaptured(shared_ptr<ChessPiece> piece) {
  if (piece) {
    auto it = find(chessPieces.begin(), chessPieces.end(), piece);
    if (it != chessPieces.end()) {
      // pop off the captured piece
      chessPieces.erase(it);
      // put the captured piece in the capturedPiece
      capturedPieces.emplace_back(piece);
    }
  }
}

void Player::undoCapture(shared_ptr<ChessPiece> piece) {
  if (piece) {
    auto it = find(capturedPieces.begin(), capturedPieces.end(), piece);
    if (it != capturedPieces.end()) {
      // pop off the captured piece
      capturedPieces.erase(it);
      // put the captured piece in the capturedPiece
      chessPieces.emplace_back(piece);
    }
  }
}

shared_ptr<ChessPiece> Player::getKing() const { return king; }

void Player::setKing(shared_ptr<King> king) { this->king = king; }

bool Player::movePiece(shared_ptr<ChessPiece> piece, pair<int, int> endPosition,
                       shared_ptr<ChessBoard> chessBoard) {
  if (piece == nullptr || piece->getColor() != color) {
    return false;
  }
  if (chessBoard->isMoveWithinBounds(endPosition) &&
      isValidMove(Move{piece->getPosition(), endPosition}, chessBoard)) {
    Move fullMoveDetails = getValidMoveDetails(
        Move{piece->getPosition(), endPosition}, chessBoard);
    // may cause issues
    if (!chessBoard->isLegalMove(fullMoveDetails, color)) {
      return false;
    }

    switch (piece->getId()) {
      case Piece::Pawn:
        dynamic_pointer_cast<Pawn>(piece)->setMoved();
        break;
      case Piece::Rook:
        dynamic_pointer_cast<Rook>(piece)->setMoved();
        break;
      case Piece::King:
        dynamic_pointer_cast<King>(piece)->setMoved();
        break;
    }
    chessBoard->movePiece(fullMoveDetails);
    piece->setPosition(endPosition);             ////newly added!!!!!!!!
    return true;
  }
  return false;
}

bool Player::setPiece(shared_ptr<ChessPiece> piece, pair<int, int> endPosition,
                      shared_ptr<ChessBoard> chessBoard) {
  if (piece == nullptr) {
    return false;
  }
  if (chessBoard->isMoveWithinBounds(endPosition)) {
    chessBoard->movePiece(
        Move{piece->getPosition(), endPosition, SideEffect::None});
    return true;
  }
  return false;
}

vector<shared_ptr<ChessPiece>> Player::getPieces() const { return chessPieces; }

bool Player::isValidMove(Move move, shared_ptr<ChessBoard> chessBoard) const {
  auto validMoves = generateValidMoves(chessBoard);
  return find(validMoves.begin(), validMoves.end(), move) != validMoves.end();
}

Move Player::getValidMoveDetails(Move move,
                                 shared_ptr<ChessBoard> chessBoard) const {
  auto validMoves = generateValidMoves(chessBoard);
  auto res = find(validMoves.begin(), validMoves.end(), move);
  if (res == validMoves.end()) {
    return Move{Move::InvalidPos, Move::InvalidPos};
  }
  return *res;
}

vector<Move> Player::generateValidMoves(
    shared_ptr<ChessBoard> chessBoard) const {
  vector<Move> moves;
  for (int i = 0; i < chessBoard->getRows(); ++i) {
    for (int j = 0; j < chessBoard->getCols(); ++j) {
      shared_ptr<ChessPiece> piece = getPiece({i, j});
      if (piece) {  // piece exists
        // diagonal pieces
        if (piece->getId() == Piece::Bishop || piece->getId() == Piece::Queen) {
          // direction: down and left
          for (int depth = 1; i - depth >= 0 && j - depth >= 0; ++depth) {
            pair<int, int> targetSquare = {i - depth, j - depth};
            // blocked by friendly piece
            if (getPiece(targetSquare)) {
              break;
            }
            // occupied by enemy piece
            if (chessBoard->getPosition(targetSquare)) {
              moves.emplace_back(Move{
                  {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              break;
            } else {
              moves.emplace_back(Move{{i, j}, targetSquare, SideEffect::None});
            }
          }

          // direction: down and right
          for (int depth = 1; i - depth >= 0 && j + depth <= 7; ++depth) {
            pair<int, int> targetSquare = {i - depth, j + depth};
            // blocked by friendly piece
            if (getPiece(targetSquare)) {
              break;
            }
            // occupied by enemy piece
            if (chessBoard->getPosition(targetSquare)) {
              moves.emplace_back(Move{
                  {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              break;
            } else {
              moves.emplace_back(Move{{i, j}, targetSquare, SideEffect::None});
            }
          }

          // direction: up and left
          for (int depth = 1; i + depth <= 7 && j - depth >= 0; ++depth) {
            pair<int, int> targetSquare = {i + depth, j - depth};
            // blocked by friendly piece
            if (getPiece(targetSquare)) {
              break;
            }

            // occupied by enemy piece
            if (chessBoard->getPosition(targetSquare)) {
              moves.emplace_back(Move{
                  {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              break;
            } else {
              moves.emplace_back(Move{{i, j}, targetSquare, SideEffect::None});
            }
          }

          // direction: up and right
          for (int depth = 1; i + depth <= 7 && j + depth <= 7; ++depth) {
            pair<int, int> targetSquare = {i + depth, j + depth};
            // blocked by friendly piece
            if (getPiece(targetSquare)) {
              break;
            }

            // occupied by enemy piece
            if (chessBoard->getPosition(targetSquare)) {
              moves.emplace_back(Move{
                  {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              break;
            } else {
              moves.emplace_back(Move{{i, j}, targetSquare, SideEffect::None});
            }
          }
        }

        // horizontol and vertical pieces
        if (piece->getId() == Piece::Rook || piece->getId() == Piece::Queen) {
          // direction: down
          for (int depth = 1; i - depth >= 0; ++depth) {
            pair<int, int> targetSquare = {i - depth, j};
            // blocked by friendly piece
            if (getPiece(targetSquare)) {
              break;
            }

            // occupied by enemy piece
            if (chessBoard->getPosition(targetSquare)) {
              moves.emplace_back(Move{
                  {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              break;
            } else {
              moves.emplace_back(Move{{i, j}, targetSquare, SideEffect::None});
            }
          }

          // direction: left
          for (int depth = 1; j - depth >= 0; ++depth) {
            pair<int, int> targetSquare = {i, j - depth};
            // blocked by friendly piece
            if (getPiece(targetSquare)) {
              break;
            }
            // occupied by enemy piece
            if (chessBoard->getPosition(targetSquare)) {
              moves.emplace_back(Move{
                  {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              break;
            } else {
              moves.emplace_back(Move{{i, j}, targetSquare, SideEffect::None});
            }
          }

          // direction: up
          for (int depth = 1; i + depth <= 7; ++depth) {
            pair<int, int> targetSquare = {i + depth, j};
            // blocked by friendly piece
            if (getPiece(targetSquare)) {
              break;
            }

            // occupied by enemy piece
            if (chessBoard->getPosition(targetSquare)) {
              moves.emplace_back(Move{
                  {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              break;
            } else {
              moves.emplace_back(Move{{i, j}, targetSquare, SideEffect::None});
            }
          }

          // direction: right
          for (int depth = 1; j + depth <= 7; ++depth) {
            pair<int, int> targetSquare = {i, j + depth};
            // blocked by friendly piece
            if (getPiece(targetSquare)) {
              break;
            }
            // occupied by enemy piece
            if (chessBoard->getPosition(targetSquare)) {
              moves.emplace_back(Move{
                  {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              break;
            } else {
              moves.emplace_back(Move{{i, j}, targetSquare, SideEffect::None});
            }
          }
        }

        // Knight
        if (piece->getId() == Piece::Knight) {
          const vector<std::pair<int, int>> knightMoves = {
              {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
              {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

          for (auto move : knightMoves) {
            pair<int, int> targetSquare = {i + move.first, j + move.second};
            if (targetSquare.first >= 0 && targetSquare.first < 8 &&
                targetSquare.second >= 0 && targetSquare.second < 8) {
              // blocked by friendly piece
              if (getPiece(targetSquare)) {
                continue;
              }
              // occupied by enemy piece
              if (chessBoard->getPosition(targetSquare)) {
                moves.emplace_back(Move{
                    {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              } else {
                moves.emplace_back(
                    Move{{i, j}, targetSquare, SideEffect::None});
              }
            }
          }
        }
        // King
        if (piece->getId() == Piece::King) {
          shared_ptr<King> king = dynamic_pointer_cast<King>(piece);
          const vector<std::pair<int, int>> kingMoves = {
              {1, 0}, {0, -1}, {0, 1},  {-1, 0},
              {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

          for (auto move : kingMoves) {
            pair<int, int> targetSquare = {i + move.first, j + move.second};
            if (targetSquare.first >= 0 && targetSquare.first < 8 &&
                targetSquare.second >= 0 && targetSquare.second < 8) {
              // blocked by friendly piece
              if (getPiece(targetSquare)) {
                continue;
              }
              // occupied by enemy piece
              if (chessBoard->getPosition(targetSquare)) {
                moves.emplace_back(Move{
                    {i, j}, targetSquare, SideEffect::Capture, targetSquare});
              } else {
                moves.emplace_back(
                    Move{{i, j}, targetSquare, SideEffect::None});
              }
            }
          }

          // handle castling
          // Notes:
          // can't castle away from a check
          // can't castle when any piece is attack a piece in between
          // can't castle into a check

          // check these conditions later

          // if piece is on the start or end rows
          if (!king->getMoved()) {
            if (color == Color::White) {
              shared_ptr<ChessPiece> rookLeft = getPiece({0, 0});
              shared_ptr<ChessPiece> rookRight = getPiece({0, 7});
              if (i == 0 && j == 4) {
                // queenside
                if (rookLeft && rookLeft->getId() == Piece::Rook &&
                    !dynamic_pointer_cast<Rook>(rookLeft)->getMoved()) {
                  bool castleFlag = true;
                  for (int k = 1; k < 4; ++k) {
                    if (chessBoard->getPosition({0, k})) {
                      castleFlag = false;
                      break;
                    }
                  }
                  if (castleFlag) {
                    moves.emplace_back(Move{{i, j},
                                            {i, 2},
                                            SideEffect::Castle,
                                            Move::InvalidPos,
                                            {i, 0},
                                            {i, 3}});
                  }
                }

                // kingside
                if (rookRight && rookRight->getId() == Piece::Rook &&
                    !dynamic_pointer_cast<Rook>(rookRight)->getMoved()) {
                  bool castleFlag = true;
                  for (int k = 5; k < 7; ++k) {
                    if (chessBoard->getPosition({0, k})) {
                      castleFlag = false;
                      break;
                    }
                  }
                  if (castleFlag) {
                    moves.emplace_back(Move{{i, j},
                                            {i, 6},
                                            SideEffect::Castle,
                                            Move::InvalidPos,
                                            {i, 7},
                                            {i, 5}});
                  }
                }
              }

            } else if (color == Color::Black) {
              shared_ptr<ChessPiece> rookLeft = getPiece({7, 0});
              shared_ptr<ChessPiece> rookRight = getPiece({7, 7});
              if (i == 7 && j == 4) {
                // queenside
                if (rookLeft && rookLeft->getId() == Piece::Rook &&
                    !dynamic_pointer_cast<Rook>(rookLeft)->getMoved()) {
                  bool castleFlag = true;
                  for (int k = 1; k < 4; ++k) {
                    if (chessBoard->getPosition({7, k})) {
                      castleFlag = false;
                      break;
                    }
                  }
                  if (castleFlag) {
                    moves.emplace_back(Move{{i, j},
                                            {i, 2},
                                            SideEffect::Castle,
                                            Move::InvalidPos,
                                            {i, 0},
                                            {i, 3}});
                  }
                }

                // kingside
                if (rookRight && rookRight->getId() == Piece::Rook &&
                    !dynamic_pointer_cast<Rook>(rookRight)->getMoved()) {
                  bool castleFlag = true;
                  for (int k = 5; k < 7; ++k) {
                    if (chessBoard->getPosition({7, k})) {
                      castleFlag = false;
                      break;
                    }
                  }
                  if (castleFlag) {
                    moves.emplace_back(Move{{i, j},
                                            {i, 6},
                                            SideEffect::Castle,
                                            Move::InvalidPos,
                                            {i, 7},
                                            {i, 5}});
                  }
                }
              }
            }
          }
        }

        // Pawn
        if (piece->getId() == Piece::Pawn) {
          shared_ptr<Pawn> pawn = dynamic_pointer_cast<Pawn>(piece);

          if (color == Color::White) {
            // direction: up (White)
            if (i + 1 <= 7) {
              // not blocked by friendly piece OR enemy piece
              if (!chessBoard->getPosition({i + 1, j})) {
                moves.emplace_back(Move{{i, j}, {i + 1, j}, SideEffect::None});
              }
            }

            // initial double square move
            if (!pawn->getMoved() && i + 2 <= 7) {
              if (!chessBoard->getPosition({i + 1, j}) &&
                  !chessBoard->getPosition({i + 2, j})) {
                moves.emplace_back(Move{{i, j}, {i + 2, j}, SideEffect::None});
              }
            }

            // pawn take left
            if (i + 1 <= 7 && j - 1 >= 0) {
              // not blocked by friendly piece
              // occupied by enemy piece
              if (!getPiece({i + 1, j - 1}) &&
                  chessBoard->getPosition({i + 1, j - 1})) {
                moves.emplace_back(Move{{i, j},
                                        {i + 1, j - 1},
                                        SideEffect::Capture,
                                        {i + 1, j - 1}});
              }
            }

            // pawn take right
            if (i + 1 <= 7 && j + 1 <= 7) {
              // not blocked by friendly piece
              // occupied by enemy piece
              if (!getPiece({i + 1, j + 1}) &&
                  chessBoard->getPosition({i + 1, j + 1})) {
                moves.emplace_back(Move{{i, j},
                                        {i + 1, j + 1},
                                        SideEffect::Capture,
                                        {i + 1, j + 1}});
              }
            }

            // en passant
            Move move = chessBoard->getLastMove();
            pair<int, int> moveStart = move.startPos;
            pair<int, int> moveEnd = move.endPos;

            // Move is valid
            if (i == 4 && moveStart.first >= 0 && moveStart.first <= 7 &&
                moveStart.second >= 0 && moveStart.second <= 7 &&
                moveEnd.first >= 0 && moveEnd.first <= 7 &&
                moveEnd.second >= 0 && moveEnd.second <= 7) {
              shared_ptr<ChessPiece> enemyPawn =
                  chessBoard->getPosition(moveEnd);
              if (enemyPawn && enemyPawn->getColor() == Color::Black &&
                  enemyPawn->getId() == Piece::Pawn &&
                  (moveEnd == make_pair(4, j - 1) ||
                   moveEnd == make_pair(4, j + 1)) &&
                  moveStart == make_pair(6, moveEnd.second)) {
                if (!getPiece({i + 1, moveEnd.second}) &&
                    !chessBoard->getPosition({i + 1, moveEnd.second})) {
                  moves.emplace_back(Move{{i, j},
                                          {i + 1, moveEnd.second},
                                          SideEffect::Enpassant,
                                          moveEnd});
                }
              }
            }
          }

          if (color == Color::Black) {
            // direction: down (Black)
            if (i - 1 >= 0) {
              // not blocked by friendly piece OR enemy piece
              if (!chessBoard->getPosition({i - 1, j})) {
                moves.emplace_back(Move{{i, j}, {i - 1, j}, SideEffect::None});
              }
            }

            // initial double square move
            if (!pawn->getMoved() && i - 2 >= 0) {
              if (!chessBoard->getPosition({i - 1, j}) &&
                  !chessBoard->getPosition({i - 2, j})) {
                moves.emplace_back(Move{{i, j}, {i - 2, j}, SideEffect::None});
              }
            }

            // pawn take left
            if (i - 1 >= 0 && j - 1 >= 0) {
              // not blocked by friendly piece
              // occupied by enemy piece
              if (!getPiece({i - 1, j - 1}) &&
                  chessBoard->getPosition({i - 1, j - 1})) {
                moves.emplace_back(Move{{i, j},
                                        {i - 1, j - 1},
                                        SideEffect::Capture,
                                        {i - 1, j - 1}});
              }
            }

            // pawn take right
            if (i - 1 >= 0 && j + 1 <= 7) {
              // not blocked by friendly piece
              // occupied by enemy piece
              if (!getPiece({i - 1, j + 1}) &&
                  chessBoard->getPosition({i - 1, j + 1})) {
                moves.emplace_back(Move{{i, j},
                                        {i - 1, j + 1},
                                        SideEffect::Capture,
                                        {i - 1, j + 1}});
              }
            }

            // en passant
            Move move = chessBoard->getLastMove();
            pair<int, int> moveStart = move.startPos;
            pair<int, int> moveEnd = move.endPos;

            // Move is valid
            if (i == 3 && moveStart.first >= 0 && moveStart.first <= 7 &&
                moveStart.second >= 0 && moveStart.second <= 7 &&
                moveEnd.first >= 0 && moveEnd.first <= 7 &&
                moveEnd.second >= 0 && moveEnd.second <= 7) {
              shared_ptr<ChessPiece> enemyPawn =
                  chessBoard->getPosition(moveEnd);
              if (enemyPawn && enemyPawn->getColor() == Color::White &&
                  enemyPawn->getId() == Piece::Pawn &&
                  (moveEnd == make_pair(3, j - 1) ||
                   moveEnd == make_pair(3, j + 1)) &&
                  moveStart == make_pair(1, moveEnd.second)) {
                if (!getPiece({i - 1, moveEnd.second}) &&
                    !chessBoard->getPosition({i - 1, moveEnd.second})) {
                  moves.emplace_back(Move{{i, j},
                                          {i - 1, moveEnd.second},
                                          SideEffect::Enpassant,
                                          moveEnd});
                }
              }
            }
          }
        }
      }
    }
  }

  return moves;
}

vector<Move> Player::generate1PieceValidCaptures(shared_ptr<ChessPiece> piece, shared_ptr<ChessBoard> chessBoard) const {
  vector<Move> allValidMoves = generateValidMoves(chessBoard);
  vector<Move> pieceValidCaptures;
  //get opponent's pieces
  vector<shared_ptr<ChessPiece>> opponentPieces;
  if (color == Color::White) opponentPieces = chessBoard->getPieces(Color::Black);
  else opponentPieces = chessBoard->getPieces(Color::White);
  
  for (auto validMove: allValidMoves) {
    if (validMove.getStartPos() == piece->getPosition()) {
      //if the move is white pawn moving forward - not a valid capture move, continue without push back the move
      if (piece->getId() == Piece::Pawn && piece->getColor() == Color::White && 
          validMove.getEndPos() == pair<int,int>{validMove.getStartPos().first,validMove.getStartPos().second+1}) continue;
      //if the move is black pawn moving forward - not a valid capture move, continue without push back the move
      if (piece->getId() == Piece::Pawn && piece->getColor() == Color::Black && 
          validMove.getEndPos() == pair<int,int>{validMove.getStartPos().first,validMove.getStartPos().second-1}) continue;
      
      //get the positions that can capture opponent's pieces
      for (auto opponentPiece: opponentPieces) {
        if (validMove.getEndPos() == opponentPiece->getPosition()) pieceValidCaptures.emplace_back(validMove);
      }
    }
  } return pieceValidCaptures;
}
