#ifndef SUBJECT_H
#define SUBJECT_H
#include <memory>
#include <utility>
#include <vector>

#include "../chess-pieces/chessPiece.h"
using namespace std;

// Forward declaration:
class Observer;

class Subject : public enable_shared_from_this<Subject> {
  vector<Observer *> observers;

 public:
  void attach(Observer *o);
  void detach(Observer *o);
  void notifyObservers();
  virtual shared_ptr<ChessPiece> getState(pair<int, int> pos) const = 0;
  virtual ~Subject() = default;
};

#endif
