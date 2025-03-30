#include "textObserver.h"

#include <iostream>
#include <memory>
#include <utility>

#include "../chess-pieces/chessPiece.h"
#include "../controller/gameController.h"
#include "observer.h"

TextObserver::TextObserver(std::shared_ptr<GameController> s) : subject{s} {
  subject->attach(this);
}

TextObserver::~TextObserver() { subject->detach(this); }

void TextObserver::notify() {
  for (int i = boardSize - 1; i >= 0; --i) {  // col
    out << i + 1 << " ";
    for (int j = 0; j < boardSize; ++j) {  // row
      std::shared_ptr<ChessPiece> p = subject->getState({i, j});
      if (p) {
        out << p->getSymbol();
      } else {
        if ((i + j) % 2 == 1) {
          // White square
          out << ' ';
        } else {
          // Black square
          out << '_';
        }
      }
    }
    out << std::endl;
  }
  out << "  abcdefgh" << std::endl;
}
