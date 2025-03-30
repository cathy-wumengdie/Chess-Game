#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include <memory>
#include <string>
#include <vector>

#include "../chess-board/chessBoard.h"
#include "../chess-pieces/chessPiece.h"
#include "../players/player.h"
#include "../view/subject.h"
#include "../view/scoreboard.h"

using namespace std;

class GameController : public Subject,
                       public enable_shared_from_this<GameController> {
  pair<shared_ptr<Player>, shared_ptr<Player>> players;
  shared_ptr<Player> currentPlayer;
  shared_ptr<ChessBoard> chessBoard;
  shared_ptr<Scoreboard> scoreboard;
  Mode::Mode mode;

 public:
  GameController(shared_ptr<Player> player1, shared_ptr<Player> player2);

  void render();

  shared_ptr<ChessPiece> getState(pair<int, int> pos) const override;

  Mode::Mode getMode() const;

  void setMode(Mode::Mode mode);

  shared_ptr<Player> getPlayer(Color::Color color) const;

  void setPlayer(shared_ptr<Player> player);

  void setCurrentPlayer(Color::Color color);

  shared_ptr<Player> getCurrentPlayer() const;

  shared_ptr<Player> getOpponentPlayer() const;

  shared_ptr<ChessBoard> getBoard() const;

  void initializeBoard();

  void initializeBlankBoard();

  bool validSetup() const;

  void switchTurn();

  void setupPiece(pair<int, int> pos, Piece::Id pieceId, Color::Color color);

  bool movePiece(pair<int, int> oldPos, pair<int, int> newPos);

  void removePiece(pair<int, int> pos);

  bool isCheckmate() const;

  bool isCheck() const;

  bool isStalemate() const;

  void updateScoreboard(Color::Color color, int value);

  void printScoreboard() const;

  ~GameController() = default;
};

#endif
