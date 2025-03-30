#include "gameController.h"

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../chess-board/chessBoard.h"
#include "../chess-pieces/chessPiece.h"
#include "../players/player.h"
#include "../view/scoreboard.h"
#include "../view/subject.h"

using namespace std;

GameController::GameController(shared_ptr<Player> player1,
                               shared_ptr<Player> player2)
    : players{player1->getColor() == Color::White ? player1 : player2,
              player2->getColor() == Color::Black ? player2 : player1},
      currentPlayer{player1->getColor() == Color::White ? player1 : player2},
      chessBoard{make_shared<ChessBoard>(player1, player2)},
      scoreboard{make_shared<Scoreboard>()},
      mode{Mode::Menu} {}

void GameController::render() { notifyObservers(); }

shared_ptr<ChessPiece> GameController::getState(pair<int, int> pos) const {
  return chessBoard->getPosition(pos);
}

void GameController::setMode(Mode::Mode mode) { this->mode = mode; }

Mode::Mode GameController::getMode() const { return mode; }

shared_ptr<Player> GameController::getPlayer(Color::Color color) const {
  return color == players.first->getColor() ? players.first : players.second;
}

void GameController::setPlayer(shared_ptr<Player> player) {
  if (player->getColor() == Color::White) {
    players.first = player;
  } else {
    players.second = player;
  }
}

void GameController::setCurrentPlayer(Color::Color color) {
  if (color == players.first->getColor()) {
    currentPlayer = players.first;
  } else {
    currentPlayer = players.second;
  }
}

shared_ptr<Player> GameController::getCurrentPlayer() const {
  return currentPlayer;
}

shared_ptr<Player> GameController::getOpponentPlayer() const {
  if (currentPlayer == players.first)
    return players.second;
  else
    return players.first;
}

shared_ptr<ChessBoard> GameController::getBoard() const { return chessBoard; }

void GameController::initializeBoard() {
  shared_ptr<ChessPiece> piece;

  players.first->reset();
  players.second->reset();

  // Making Player1 pieces
  players.first->createPiece(Piece::Rook, {0, 0});
  players.first->createPiece(Piece::Rook, {0, 7});
  players.first->createPiece(Piece::Knight, {0, 1});
  players.first->createPiece(Piece::Knight, {0, 6});
  players.first->createPiece(Piece::Bishop, {0, 2});
  players.first->createPiece(Piece::Bishop, {0, 5});
  players.first->createPiece(Piece::Queen, {0, 3});
  players.first->createPiece(Piece::King, {0, 4});
  players.first->setKing(dynamic_pointer_cast<King>(players.first->getPiece({0, 4})));
  for (int i = 0; i < chessBoard->getCols(); ++i) {
    players.first->createPiece(Piece::Pawn, {1, i});
  }

  // Making Player2 pieces
  players.second->createPiece(Piece::Rook, {7, 0});
  players.second->createPiece(Piece::Rook, {7, 7});
  players.second->createPiece(Piece::Knight, {7, 1});
  players.second->createPiece(Piece::Knight, {7, 6});
  players.second->createPiece(Piece::Bishop, {7, 2});
  players.second->createPiece(Piece::Bishop, {7, 5});
  players.second->createPiece(Piece::Queen, {7, 3});
  players.second->createPiece(Piece::King, {7, 4});
  players.second->setKing(dynamic_pointer_cast<King>(players.second->getPiece({7, 4})));
  for (int i = 0; i < chessBoard->getCols(); ++i) {
    players.second->createPiece(Piece::Pawn, {6, i});
  }

  chessBoard = make_shared<ChessBoard>(players.first, players.second);
}

void GameController::initializeBlankBoard() {
  players.first->reset();
  players.second->reset();
  chessBoard = make_shared<ChessBoard>(players.first, players.second);
}

bool GameController::validSetup() const {
  int numBlackKing = 0;
  int numWhiteKing = 0;
  for (int i = 0; i < chessBoard->getRows(); ++i) {
    for (int j = 0; j < chessBoard->getCols(); ++j) {
      shared_ptr<ChessPiece> piece = chessBoard->getPosition({i, j});
      if (piece && piece->getId() == Piece::King &&
          piece->getColor() == Color::White) {  // if p is a white king
        if (numWhiteKing >= 1) {
          return false;
        }
        players.first->setKing(dynamic_pointer_cast<King>(piece));
        ++numWhiteKing;
      }
      if (piece && piece->getId() == Piece::King &&
          piece->getColor() == Color::Black) {  // if p is a black king
        if (numBlackKing >= 1) {
          return false;
        }
        players.second->setKing(dynamic_pointer_cast<King>(piece));
        ++numBlackKing;
      }
      if (piece && (i == 0 || i == 7) && piece->getId() == Piece::Pawn) {
        return false;
      }
    }
  }
  return true;
}

void GameController::switchTurn() {
  if (currentPlayer == players.first)
    currentPlayer = players.second;
  else
    currentPlayer = players.first;
}

void GameController::setupPiece(pair<int, int> pos, Piece::Id pieceId,
                                Color::Color color) {
  shared_ptr<ChessPiece> piece;
  if (color == Color::White) {
    players.first->createPiece(pieceId, pos);
    piece = players.first->getPiece(pos);
  } else {
    players.second->createPiece(pieceId, pos);
    piece = players.second->getPiece(pos);
  }
  chessBoard->setPosition(pos, piece);
}

bool GameController::movePiece(pair<int, int> oldPos, pair<int, int> newPos) {
  shared_ptr<ChessPiece> piece = currentPlayer->getPiece(oldPos);
  if (piece) {
    return currentPlayer->movePiece(piece, newPos, chessBoard);
  }
  return false;
}

void GameController::removePiece(pair<int, int> pos) {
  shared_ptr<ChessPiece> p = getState(pos);
  if (p) {
    chessBoard->setCaptured(pos);
  }
}

bool GameController::isCheckmate() const { return false; }

bool GameController::isCheck() const { return false; }

bool GameController::isStalemate() const {
  return currentPlayer->generateValidMoves(chessBoard).empty();
}

void GameController::updateScoreboard(Color::Color color, int value) {
  scoreboard->updateScoreboard(color, value);
}

void GameController::printScoreboard() const { scoreboard->printScoreboard(); }
