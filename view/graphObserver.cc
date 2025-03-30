#include <iostream>
#include <utility>
#include <memory>
#include "../chess-pieces/chessPiece.h"
#include "../controller/gameController.h"
#include "graphObserver.h"
#include "observer.h"
#include "window.h"

GraphObserver::GraphObserver(std::shared_ptr<GameController> s) : 
    subject{s}, window{10 * (8), 10 * (8)} {
    subject->attach(this);
    for (int i = 0; i < boardSize; ++i) {
      for (int j = 0; j < boardSize; ++j) {
        std::shared_ptr<ChessPiece> p = subject->getState({i, j});

        if ((i + j) % 2 == 0) {
          // White square
          window.fillRectangle(squareSize * j, squareSize * i, squareSize, squareSize, Xwindow::Lightblue);
        } else {
          // Black square
          window.fillRectangle(squareSize * j, squareSize * i, squareSize, squareSize, Xwindow::Steelblue);
        }
      }
    }
}


GraphObserver::~GraphObserver() { subject->detach(this); }

void GraphObserver::notify() {
  for (int i = boardSize - 1; i >= 0; --i) {
    for (int j = 0; j < boardSize; ++j) {
      std::shared_ptr<ChessPiece> p = subject->getState({i, j});

      if (p) {
        std::string msg;
        msg += p->getSymbol();
        window.drawString(squareSize * j, squareSize * (boardSize - i), msg);
      } else {
        if ((i + j) % 2 == 1) {
          window.fillRectangle(squareSize * j, squareSize * (boardSize - i - 1), squareSize, squareSize, Xwindow::Lightblue);
        } else {
          window.fillRectangle(squareSize * j, squareSize * (boardSize - i - 1), squareSize, squareSize, Xwindow::Steelblue);
        }
      }
    }
  }
}

