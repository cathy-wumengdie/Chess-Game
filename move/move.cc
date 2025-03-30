#include "move.h"

const pair<int, int> Move::InvalidPos = {-1, -1};

Move::Move(pair<int, int> startPos, pair<int, int> endPos,
           SideEffect::SideEffect effect, pair<int, int> capturedPos,
           pair<int, int> startCastlePos, pair<int, int> endCastlePos)
    : startPos{startPos},
      endPos{endPos},
      effect{effect},
      capturedPos{capturedPos},
      startCastlePos{startCastlePos},
      endCastlePos{endCastlePos} {}

bool Move::operator==(const Move& other) const {
  return startPos == other.startPos && endPos == other.endPos;
}

pair<int,int> Move::getStartPos() const { return startPos; }

pair<int,int> Move::getEndPos() const { return endPos; }
