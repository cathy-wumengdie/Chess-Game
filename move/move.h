#ifndef MOVE_H
#define MOVE_H
#include <utility>

using namespace std;

#include "../enum.h"

// switch to class w/ getters and setters later
struct Move {
  static const pair<int, int> InvalidPos;

  pair<int, int> startPos;
  pair<int, int> endPos;

  pair<int, int> capturedPos;
  pair<int, int> startCastlePos;
  pair<int, int> endCastlePos;

  SideEffect::SideEffect effect;

  Move() = default;

  Move(pair<int, int> startPos, pair<int, int> endPos,
       SideEffect::SideEffect effect = SideEffect::None,
       pair<int, int> capturedPos = InvalidPos,
       pair<int, int> startCastlePos = InvalidPos,
       pair<int, int> endCastlePos = InvalidPos);

  bool operator==(const Move& other) const;
  
  pair<int,int> getStartPos() const;

  pair<int,int> getEndPos() const;
};

#endif
