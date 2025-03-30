#ifndef __TEXTOBSERVER_H__
#define __TEXTOBSERVER_H__
#include <iostream>
#include <memory>

#include "../controller/gameController.h"
#include "observer.h"

class TextObserver : public Observer,
                     public std::enable_shared_from_this<TextObserver> {
  std::shared_ptr<GameController> subject;
  const int boardSize = 8;

  std::ostream &out = std::cout;

 public:
  TextObserver(std::shared_ptr<GameController> s);
  void notify() override;
  ~TextObserver();
};

#endif
