#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "./chess-board/chessBoard.h"
#include "./controller/gameController.h"
#include "./players/player.h"
#include "./view/textObserver.h"
#include "./players/computerPlayer.h"
#include "./players/level1Bot.h"
#include "./players/level2Bot.h"
#include "./view/graphObserver.h"

using namespace std;

bool validPos(string pos) {
  if (pos.length() != 2)
    return false;
  else {
    char x = pos[0];
    char y = pos[1];
    if (x >= 'a' && x <= 'h' && y >= '1' && y <= '8') {
      return true;
    } else {
      return false;
    }
  }
}

pair<int, int> getPos(string pos) {
  int y = pos[0] - 'a';
  int x = pos[1] - '1';
  return {x, y};
}

bool validColor(string color) { return color == "White" || color == "Black"; }

Color::Color getColor(string color) {
  return color == "White" ? Color::White : Color::Black;
}

string convertColor2String(Color::Color color) {
  return color == Color::White ? "White" : "Black";
}

Color::Color convertSymbolToColor(char pieceSymbol) {
  if (pieceSymbol >= 'A' && pieceSymbol <= 'Z')
    return Color::White;
  else
    return Color::Black;
}

bool validPiece(char pieceSymbol) {
  if (pieceSymbol == 'k' || pieceSymbol == 'q' || pieceSymbol == 'b' ||
      pieceSymbol == 'r' || pieceSymbol == 'n' || pieceSymbol == 'p' ||
      pieceSymbol == 'K' || pieceSymbol == 'Q' || pieceSymbol == 'B' ||
      pieceSymbol == 'R' || pieceSymbol == 'N' || pieceSymbol == 'P') {
    return true;
  } else
    return false;
}

Piece::Id convertSymbolToId(char pieceSymbol) {
  if (pieceSymbol == 'k' || pieceSymbol == 'K') return Piece::King;
  if (pieceSymbol == 'q' || pieceSymbol == 'Q') return Piece::Queen;
  if (pieceSymbol == 'b' || pieceSymbol == 'B') return Piece::Bishop;
  if (pieceSymbol == 'r' || pieceSymbol == 'R') return Piece::Rook;
  if (pieceSymbol == 'n' || pieceSymbol == 'N') return Piece::Knight;
  return Piece::Pawn;
}

int main() {
  shared_ptr<Player> player1 = make_shared<Player>(Color::White, "player1");
  shared_ptr<Player> player2 = make_shared<Player>(Color::Black, "player2");
  shared_ptr<GameController> game =
      make_shared<GameController>(player1, player2);
  shared_ptr<TextObserver> textObserver = make_shared<TextObserver>(game);
  // shared_ptr<GraphObserver> graphObserver = make_shared<GraphObserver>(game);

  string whitePlayer, blackPlayer;
  string oldPosition, newPosition;
  pair<int, int> oldPos, newPos;
  Move move;

  string cmd;
  game->setMode(Mode::Menu);
  while (true) {    //cathy edited
   if (game->getCurrentPlayer()->getLevel() == 0) cin >> cmd;   //cathy edited
    switch (game->getMode()) {
      case Mode::Menu:
        if (cmd == "setup") {
          game->setMode(Mode::Setup);
          game->setPlayer(player1);
          game->setPlayer(player2);
          game->setCurrentPlayer(Color::White);
          game->initializeBlankBoard();
          game->render();
        } else if (cmd == "game") {
          cin >> whitePlayer >> blackPlayer;
          // configuring whitePlayer
          if (whitePlayer == "human") {
            player1 = make_shared<Player>(Color::White, "player1", 0);
          } else if (whitePlayer == "computer[1]") {
            player1 = make_shared<Level1Bot>(Color::White, "player1", 1);
          } else if (whitePlayer == "computer[2]") {
            player1 = make_shared<Level2Bot>(Color::White, "player1", 2);
          } else if (whitePlayer == "computer[3]") {
            player1 = make_shared<ComputerPlayer>(Color::White, "player1", 3);
          } else {
            cerr << "Invalid command." << endl;
          }
          // configuring blackPlayer
          if (blackPlayer == "human") {
            player2 = make_shared<Player>(Color::Black, "player2", 0);
          } else if (blackPlayer == "computer[1]") {
            player2 = make_shared<Level1Bot>(Color::Black, "player2", 1);
          } else if (blackPlayer == "computer[2]") {

            player2 = make_shared<Level2Bot>(Color::Black, "player2", 2);
          } else if (blackPlayer == "computer[3]") {
            player2 = make_shared<ComputerPlayer>(Color::Black, "player2", 3);
          } else {
            cerr << "Invalid command." << endl;
          }
          game->setMode(Mode::Playing);
          game->setPlayer(player1);
          game->setPlayer(player2);
          game->setCurrentPlayer(Color::White);
          game->initializeBoard();
          game->render();
          cout << convertColor2String(game->getCurrentPlayer()->getColor())
                 << "'s turn:" << endl;
        } else {
          cerr << "Invalid command in Menu: " << cmd << "\n";
        }
        break;
      case Mode::Setup:
        if (cmd == "+") {
          char pieceSymbol;
          string position;
          cin >> pieceSymbol >> position;
          if (validPiece(pieceSymbol) &&
              validPos(position)) {  // check if the input format is valid
            pair<int, int> pos = getPos(position);
            shared_ptr<ChessPiece> p = game->getState(pos);
            if (p) {  // if a piece is already on pos
              game->removePiece(pos);
            }
            Color::Color color = convertSymbolToColor(pieceSymbol);
            Piece::Id pieceId = convertSymbolToId(pieceSymbol);
            game->setupPiece(pos, pieceId, color);
            game->render();
          } else {
            cerr << "Invalid command." << endl;
          }
        } else if (cmd == "-") {
          string position;
          cin >> position;
          if (validPos(position)) {
            pair<int, int> pos = getPos(position);
            shared_ptr<ChessPiece> p = game->getState(pos);
            if (p) {
              game->removePiece(pos);
              game->render();
            }
          } else {
            cerr << "Invalid command." << endl;
          }
        } else if (cmd == "=") {
          string color;
          cin >> color;
          if (validColor(color)) {
            Color::Color c = getColor(color);
            game->setCurrentPlayer(c);
          } else {
            cerr << "Invalid command." << endl;
          }
        } else if (cmd == "done") {
          if (game->validSetup()) {
            game->setMode(Mode::Playing);
            cout << convertColor2String(game->getCurrentPlayer()->getColor())
                 << "'s turn:" << endl;
          } else {
            cerr << "Invalid setup." << endl;
          }
        } else {
          cerr << "Invalid command." << endl;
        }
        break;
      case Mode::Playing:
        if (cmd == "resign") {
          string winner =
              convertColor2String(game->getOpponentPlayer()->getColor());
          cout << winner << " wins!" << endl;
          game->setMode(Mode::End);
          // update scoreaboard -> award the winner 1 point
          game->updateScoreboard(game->getOpponentPlayer()->getColor(), 1);
        } else if (cmd == "undo") {  // remove later
          cin >> oldPosition >> newPosition;
          if (validPos(oldPosition) && validPos(newPosition)) {
            pair<int, int> oldPos = getPos(oldPosition);
            pair<int, int> newPos = getPos(newPosition);
            Move move = Move{oldPos, newPos};

            game->getBoard()->undoMove(move);
            game->render();
            shared_ptr<ChessPiece> piece =
                game->getBoard()->getPosition(oldPos);
            cout << "{ " << piece->getPosition().first << " "
                 << piece->getPosition().second << " }\n";
            game->switchTurn();
          }
        } else if (cmd == "move") {
          //pair<int, int> oldPos, newPos;     //move to the top
          // human player has level 0
          if (game->getCurrentPlayer()->getLevel() == 0) {
            //cout << "This is human player" << endl;
            cin >> oldPosition >> newPosition;   ////////cathy edited
          }
          // computer player has level 1-3
          else if (game->getCurrentPlayer()->getLevel() == 1) {      ////will add other levels later
            //cout << "This is computer player level 1" << endl;
            //game->getCurrentPlayer()->generateMove(game->getBoard()); 
            dynamic_pointer_cast<Level1Bot>(game->getCurrentPlayer())->generateMove(game->getBoard(),oldPos,newPos);       ////////cathy edited
          }
          else if (game->getCurrentPlayer()->getLevel() == 2) {      ////will add other levels later
            //cout << "This is computer player level 2" << endl;
            //game->getCurrentPlayer()->generateMove(game->getBoard()); 
            dynamic_pointer_cast<Level2Bot>(game->getCurrentPlayer())->generateMove(game->getBoard(),oldPos,newPos);       ////////cathy edited
          }
          if ((validPos(oldPosition) && validPos(newPosition)) || (game->getCurrentPlayer()->getLevel() != 0)) { ////////cathy edited
            if (game->getCurrentPlayer()->getLevel() == 0) {   ////////cathy edited
              oldPos = getPos(oldPosition);
              newPos = getPos(newPosition);
            } move = Move{oldPos, newPos};  //deleted Move

            if (game->movePiece(oldPos, newPos)) {
              game->render();
              game->switchTurn();

              // checkmate -> game ends
              if (game->isCheckmate()) {
                string winner =
                    convertColor2String(game->getOpponentPlayer()->getColor());
                cout << winner << " wins!" << endl;
                game->setMode(Mode::End);
                // update scoreboard -> award the winner 1 point
                game->updateScoreboard(game->getOpponentPlayer()->getColor(),
                                       1);
              }

              // stalemate(draw) -> game ends
              if (game->isStalemate()) {
                cout << "Stalemate!" << endl;
                game->setMode(Mode::End);
                // update scoareboard -> award each player 0.5 point
                game->updateScoreboard(game->getCurrentPlayer()->getColor(),
                                       0.5);
                game->updateScoreboard(game->getOpponentPlayer()->getColor(),
                                       1);
              }

              // game continues
              cout << convertColor2String(game->getCurrentPlayer()->getColor())
                   << "'s turn:" << endl;
            } else {
              cerr << "Invalid move." << endl;
              break;   //////////////will delete after
            }
          } else {
            cerr << "Invalid command." << endl;
          }  // if (cmd == "move")
          break;
          case Mode::End:
            if (cmd == "menu") {
              game->setMode(Mode::Menu);
            }
            break;
        }
        if (cmd == "quit" || cmd == "q") {
          break;
        }
    }
  }
  game->printScoreboard();
  return 0;
}
