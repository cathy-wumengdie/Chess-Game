#include "computerPlayer.h"

#include <algorithm> // find, random_shuffle
#include <unordered_map>

ComputerPlayer::ComputerPlayer(Color::Color color, std::string id, int level):
  Player{color, id, level} {}

//void ComputerPlayer::generateMove(shared_ptr<ChessBoard> chessBoard) {}

shared_ptr<ChessPiece> ComputerPlayer::generateRandomPiece(shared_ptr<ChessBoard> chessBoard) {
  random_shuffle(chessPieces.begin(),chessPieces.end());
  return chessPieces.at(0);
}

bool ComputerPlayer::generateRandomMove(shared_ptr<ChessPiece> piece, shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos) {
  vector<Move> allValidMoves = generateValidMoves(chessBoard);
  vector<Move> validMoves4Piece;
  for (unsigned int i = 0; i < allValidMoves.size(); i++) {
    if (allValidMoves.at(i).getStartPos() == piece->getPosition()) {
      validMoves4Piece.emplace_back(allValidMoves.at(i));
    }
  }
  random_shuffle(validMoves4Piece.begin(),validMoves4Piece.end());
  
  //cout << " validMoves4Piece.size(): " << validMoves4Piece.size() << endl;
  if (validMoves4Piece.size() == 0) return false;
  oldPos = piece->getPosition();
  newPos = validMoves4Piece.at(0).getEndPos();
  //movePiece(piece,validMoves4Piece.at(0).getEndPos(),chessBoard);
  return true;
}

bool ComputerPlayer::capturePiece(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos) {
  pair<int,int> pre_position{-1,-1};
  pair<int,int> next_position{-1,-1};
  for (unsigned int i = 0; i < chessPieces.size(); i++) {
    vector<Move> allValidCaptures = generate1PieceValidCaptures(chessPieces.at(i), chessBoard);
    if (allValidCaptures.size() != 0) {
      pre_position = allValidCaptures.begin()->getStartPos();
      next_position = allValidCaptures.begin()->getEndPos();
    }
  }
  if (next_position != pair<int,int>{-1,-1}) {
    oldPos = pre_position;
    newPos = next_position;
    std::cout << "{" << pre_position.first << "," << pre_position.second << "}";   //will delete later
    //movePiece(getPiece(pre_position),next_position,chessBoard);
    std::cout << " -> {" << next_position.first << "," << next_position.second << "}" << endl;   //will delete later
    cout << "Capture a piece!" << endl;
    return true;
  } return false;
} 

//NEVER pass in Queen as ID!!
vector<pair<int,int>> ComputerPlayer::getPreCapturePos(Piece::Id ID, pair<int,int> attackPos, shared_ptr<ChessBoard> chessBoard) {
  vector<pair<int,int>> preCapturePos; //the vector of all possible check positions
  if (ID == Piece::Pawn) {
    //capture position for a black pawn
    if (color == Color::Black) {
      if (attackPos.first+1 <= chessBoard->getRows() && attackPos.second-1 >= 0)
      preCapturePos.emplace_back(pair<int,int>{attackPos.first+1,attackPos.second-1});
      if (attackPos.first-1 >= 0 && attackPos.second-1 >= 0)
      preCapturePos.emplace_back(pair<int,int>{attackPos.first-1,attackPos.second-1});
    }
    //capture position for a white pawn
    else {
      if (attackPos.first+1 <= chessBoard->getRows() && attackPos.second+1 <= chessBoard->getCols())
      preCapturePos.emplace_back(pair<int,int>{attackPos.first+1,attackPos.second+1}); 
      if (attackPos.first-1 >= 0 && attackPos.second+1 <= chessBoard->getCols())
      preCapturePos.emplace_back(pair<int,int>{attackPos.first-1,attackPos.second+1}); 
    }
  }
  
  if (ID == Piece::Bishop) {
    //bishop's max possible next move position for one single move
    const int max_possiblePos = 13; 
    //get all the possible capture positions for bishop
    for (int i = 1; i <= max_possiblePos; i++){
      //positions on the left upper diagonal
      if (attackPos.first-i >= 0 && attackPos.second+i <= chessBoard->getRows()) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first-i,attackPos.second+i});
      }
      //positions on the right upper diagonal
      if (attackPos.first+i <= chessBoard->getCols() && attackPos.second+i <= chessBoard->getRows()) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first+i,attackPos.second+i});
      }
      //positions on the left lower diagonal
      if (attackPos.first-i >= 0 && attackPos.second-i >= 0) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first-i,attackPos.second-i});
      }
      //positions on the right lower diagonal
      if (attackPos.first+i <= chessBoard->getCols() && attackPos.second-i >= 0) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first+i,attackPos.second-i});
      }
    }
  }
    
  if (ID == Piece::Rook) { 
    //rook's max possible next move position for one single move
    const int max_possiblePos = 14; 
    //get all the possible capture positions for rook
    for (int i = 1; i <= max_possiblePos; i++){
      //positions on the left
      if (attackPos.first-i >= 0) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first-i,attackPos.second});
      }
      //positions on the right
      if (attackPos.first+i <= chessBoard->getCols()) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first+i,attackPos.second});
      }
      //positions on the forth
      if (attackPos.second+i <= chessBoard->getRows()) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first,attackPos.second+i});
      }
      //positions on the back
      if (attackPos.second-i >= 0) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first,attackPos.second-i});
      }
    }
  }

  if (ID == Piece::Knight) {
    //knight can only change it's coordinate on an L-shape
    const int change1 = 1;
    const int change2 = 2;
    //positions on the L-shap to the top
    if (attackPos.second+change2 <= chessBoard->getRows()) {
      if (attackPos.first-change1 >= 0) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first-change1,attackPos.second+change2});
      }
      if (attackPos.first+change1 <= chessBoard->getCols()) {
       preCapturePos.emplace_back(pair<int,int>{attackPos.first+change1,attackPos.second+change2});
      }
    }
    //positions on the L-shap to the bottem
    if (attackPos.second-change2 >= 0) {
      if (attackPos.first-change1 >= 0) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first-change1,attackPos.second-change2});
      }
      if (attackPos.first+change1 <= chessBoard->getCols()) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first+change1,attackPos.second-change2});
      }
    }
    //positions on the L-shape to the left
    if (attackPos.first-change2 >= 0) {
      if (attackPos.second-change1 >= 0) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first-change2,attackPos.second-change1});
      }
      if (attackPos.second+change1 <= chessBoard->getRows()) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first-change2,attackPos.second+change1});
      }
    }
    //positions on the L-shape to the right
    if (attackPos.first+change2 <= chessBoard->getCols()) {
      if (attackPos.second-change1 >= 0) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first+change2,attackPos.second-change1});
      }
      if (attackPos.second+change1 <= chessBoard->getRows()) {
        preCapturePos.emplace_back(pair<int,int>{attackPos.first+change2,attackPos.second+change1});
      }
    }
  }
  
  if (ID == Piece::King) {
    //positions on the forth
    if (attackPos.second+1 <= chessBoard->getRows()) {
      preCapturePos.emplace_back(pair<int,int>{attackPos.first,attackPos.second+1});
      if (attackPos.first-1 >= 0) preCapturePos.emplace_back(pair<int,int>{attackPos.first-1,attackPos.second+1});
      if (attackPos.first+1 <= chessBoard->getCols()) preCapturePos.emplace_back(pair<int,int>{attackPos.first+1,attackPos.second+1});
    }
    //positions at the back
    if (attackPos.second-1 >= 0) {
      preCapturePos.emplace_back(pair<int,int>{attackPos.first,attackPos.second-1});
      if (attackPos.first-1 >= 0) preCapturePos.emplace_back(pair<int,int>{attackPos.first-1,attackPos.second-1});
      if (attackPos.first+1 <= chessBoard->getCols()) preCapturePos.emplace_back(pair<int,int>{attackPos.first+1,attackPos.second-1});
    }
    //position on the left
    if (attackPos.first-1 >= 0) preCapturePos.emplace_back(pair<int,int>{attackPos.first-1,attackPos.second});
    //position on the right
    if (attackPos.first+1 <= chessBoard->getCols()) preCapturePos.emplace_back(pair<int,int>{attackPos.first+1,attackPos.second});
  } return preCapturePos;
}

bool ComputerPlayer::canMove2PreCapturePos(Piece::Id ID, vector<pair<int,int>> checkPos, 
            vector<shared_ptr<ChessPiece>> pieces, shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos) {
  for (unsigned int i = 0; i < chessPieces.size(); i++) {
    if (chessPieces.at(i)->getId() == ID) {
      for (unsigned int j = 0; j < checkPos.size(); j++) {
        if (isValidMove(Move{chessPieces.at(i)->getPosition(), checkPos.at(j)}, chessBoard)) {
          oldPos = chessPieces.at(i)->getPosition();
          newPos = checkPos.at(j);
          std::cout << "{" << chessPieces.at(i)->getPosition().first << "," << chessPieces.at(i)->getPosition().second << "}";   //will delete later
          //movePiece(chessPieces.at(i),checkPos.at(j),chessBoard);
          std::cout << " -> {" << checkPos.at(j).first << "," << checkPos.at(j).second << "}" << endl;   //will delete later
          return true;
        }
      }
    }
  } return false;
}

vector<pair<int,int>> ComputerPlayer::getCheckPos(vector<pair<int,int>> preCapturePos, pair<int,int> kingPos, shared_ptr<ChessBoard> chessBoard) const {
  vector<pair<int,int>> checkPos;
  for (auto checkpos: preCapturePos) {
    if (!chessBoard->anyPiecesInTheWay(checkpos,kingPos)) checkPos.emplace_back(checkpos);
  } return checkPos;
}

bool ComputerPlayer::check(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos) {
  vector<pair<int,int>> preCapturePos;
  vector<pair<int,int>> checkPos; //the vector of all possible check positions
  pair<int,int> kingPos;
  shared_ptr<ChessPiece> piece;  //the piece that can check
  if (color == Color::Black) {
    if (!chessBoard->getKing(Color::White)) return false;
    kingPos = chessBoard->getKing(Color::White)->getPosition();
  }
  else if (!chessBoard->getKing(Color::Black)) return false;
  else kingPos = chessBoard->getKing(Color::Black)->getPosition();

  //get all the possible check positions for a pawn
  preCapturePos = getPreCapturePos(Piece::Pawn,kingPos,chessBoard);           ///check on this later
  checkPos = getCheckPos(preCapturePos, kingPos, chessBoard);
  //check if there's a pawn that can get to the check position
  if (canMove2PreCapturePos(Piece::Pawn,checkPos,chessPieces,chessBoard, oldPos, newPos)) {   //////need to add checking if there's other pieces in the way
    cout << "Check!!!" << endl;
    return true;
  }
  preCapturePos.clear();
  checkPos.clear();

  //get all the possible check positions for a bishop
  preCapturePos = getPreCapturePos(Piece::Bishop,kingPos,chessBoard);
  checkPos = getCheckPos(preCapturePos, kingPos, chessBoard);
  //check if there's a bishop or a queen that can get to the check position
  if (canMove2PreCapturePos(Piece::Bishop,checkPos,chessPieces,chessBoard, oldPos, newPos)) {
    cout << "Check!!!" << endl;
    return true;
  }
  if (canMove2PreCapturePos(Piece::Queen,checkPos,chessPieces,chessBoard, oldPos, newPos)) {
    cout << "Check!!!" << endl;
    return true;
  }
  preCapturePos.clear();
  checkPos.clear();

  //get all the possible check positions for rook
  preCapturePos = getPreCapturePos(Piece::Rook,kingPos,chessBoard);
  checkPos = getCheckPos(preCapturePos, kingPos, chessBoard);
  //check if there's a rook or a queen that can get to the check position
  if (canMove2PreCapturePos(Piece::Rook,checkPos,chessPieces,chessBoard, oldPos, newPos)) {
    cout << "Check!!!" << endl;
    return true;
  }
  if (canMove2PreCapturePos(Piece::Queen,checkPos,chessPieces,chessBoard, oldPos, newPos)) {
    cout << "Check!!!" << endl;
    return true;
  }
  preCapturePos.clear();
  checkPos.clear();

  //get all the possible check positions for a knight
  preCapturePos = getPreCapturePos(Piece::Knight,kingPos,chessBoard);
  checkPos = getCheckPos(preCapturePos, kingPos, chessBoard);
  //check if there's a knight that can get to the check position
  if (canMove2PreCapturePos(Piece::Knight,checkPos,chessPieces,chessBoard, oldPos, newPos)) {
    cout << "Check!!!" << endl;
    return true;
  }
  preCapturePos.clear();
  checkPos.clear();

  return false;
}

bool ComputerPlayer::moveUnderAttackPiece(Piece::Id ID, vector<shared_ptr<ChessPiece>> opponentPieces, shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos) {
  for (unsigned int i = 0; i < chessPieces.size(); i++) {
    shared_ptr<ChessPiece> piece = chessPieces.at(i);
    vector<pair<int,int>> preCapturePos;
    if (ID != Piece::Queen) preCapturePos = getPreCapturePos(ID,piece->getPosition(),chessBoard);
    else {  //ID is Queen
      preCapturePos = getPreCapturePos(Piece::Rook,piece->getPosition(),chessBoard);  //get opponent's rook's possible capture positions
      vector<pair<int,int>> preCapturePos_Bishop = getPreCapturePos(Piece::Bishop,piece->getPosition(),chessBoard);  //get opponent's bishop's possible capture positions
      preCapturePos.insert(preCapturePos.end(),preCapturePos_Bishop.begin(),preCapturePos_Bishop.end());  //combine the possible capture positions of rook and bishop
    }
    bool isUnderAttack = false;
    for (unsigned int j = 0; j < opponentPieces.size(); j++) {
      if (opponentPieces.at(j)->getId() == ID){
        auto foundPos = find(preCapturePos.begin(), preCapturePos.end(), opponentPieces.at(j)->getPosition());
        if (foundPos != preCapturePos.end()) {  //there's an opponent's piece on the position that can capture the CP's piece
          isUnderAttack = true;
          break;
        }
      }  
    } 
    if (isUnderAttack) {
      generateRandomMove(piece,chessBoard, oldPos, newPos);  //move the piece under attack away randomly
      return true;
    }
  } return false;
}

bool ComputerPlayer::avoidCapture(shared_ptr<ChessBoard> chessBoard, pair<int,int>& oldPos, pair<int,int>& newPos) {
  //get opponent's pieces
  vector<shared_ptr<ChessPiece>> opponentPieces;
  if (color == Color::Black) opponentPieces = chessBoard->getPieces(Color::White);
  else opponentPieces = chessBoard->getPieces(Color::Black);

  if (moveUnderAttackPiece(Piece::Pawn,opponentPieces,chessBoard,oldPos,newPos)) return true;
  if (moveUnderAttackPiece(Piece::Rook,opponentPieces,chessBoard,oldPos,newPos)) return true;
  if (moveUnderAttackPiece(Piece::Bishop,opponentPieces,chessBoard,oldPos,newPos)) return true;
  if (moveUnderAttackPiece(Piece::Knight,opponentPieces,chessBoard,oldPos,newPos)) return true;
  if (moveUnderAttackPiece(Piece::Queen,opponentPieces,chessBoard,oldPos,newPos)) return true;
  if (moveUnderAttackPiece(Piece::King,opponentPieces,chessBoard,oldPos,newPos)) return true;
  return false;
}
