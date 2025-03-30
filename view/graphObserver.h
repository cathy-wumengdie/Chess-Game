#ifndef __GRAPHOBSERVER_H__
#define __GRAPHOBSERVER_H__
#include <iostream>
#include <memory>
#include "../controller/gameController.h"
#include "observer.h"
#include "window.h"

class GraphObserver : public Observer,
                     public std::enable_shared_from_this<GraphObserver> {
  std::shared_ptr<GameController> subject;
  const int boardSize = 8;
  const int squareSize = 10;

  Xwindow window;

 public:
  GraphObserver(std::shared_ptr<GameController> s);
  void notify() override;
  ~GraphObserver();
};

#endif

