//
//  Player.cpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "Player.hpp"
#include "Game.hpp"

namespace ECE141 {
  
  int Player::pcount = 0; //init our static member to track # of players...
  
  static PieceColor autoColor() { //auto assigns a color
    return (++Player::pcount % 2) ? PieceColor::blue : PieceColor::gold;
  }
  
  Player::Player() : color(autoColor()) {}
  
  bool Player::takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog) {
    bool moved = false;
    BestMove bestlist;
    size_t theCount=aGame.countAvailablePieces(color);
    for(int pos=0;pos<theCount;pos++) {
      
       //NOTICE HOW WE CHECK FOR CAPTURED PIECES?
       if(const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)) {
         bestlist.findBest(thePiece, aGame);
       }
       
         //add logic here to find your best move, then move ONE piece per turn...
    }
    while (!bestlist.best.empty()) {
      moved = true;
      auto thebest= bestlist.best.top();
      auto thePieceMove = thebest.second;
      auto thePiece = thePieceMove.first;
      auto theLocation = thePieceMove.second;
      //std::cout << "jumping to: "<<theLocation.row << " "<<theLocation.col<<"\n";
      aGame.movePieceTo(*thePiece, theLocation);
      if(!bestlist.can_jump)
        bestlist.reset();

      else {
        bestlist.reset();
        bestlist.findBest(thePiece, aGame);
      }
    }
      if (moved) 
        return true;
      return false;
  }

  bool canStep(const Location & aLoc,const Piece * aPiece, Game& aGame) {
    if (!aGame.getTileAt(aLoc)) //if nullptr, not valid
      return false;
    if (aGame.getTileAt(aLoc)->getPiece()) //if not nullptr, cant step
      return false;

    return true;
  }
  bool canJumpOver(const Location & aLoc,const Piece * aPiece, Game& aGame ) {
    if (!aGame.getTileAt(aLoc)) //if nullptr, not valid
      return false;
    if (canStep(aLoc, aPiece, aGame))
      return false;
    if (aGame.getTileAt(aLoc)->getPiece()->getColor() == aPiece->getColor())
      return false;
    
    return true;
  }

  BestMove & BestMove::findBest(const Piece * aPiece, Game& aGame) {
    
  
    auto moves = getMoves[aPiece->getKind()](aPiece, aGame);
    if (moves.empty()) {
      return *this;
    }
    
    std::priority_queue<move_val, std::vector<move_val>, ComparePairMove> temp;
    
    for (auto p : moves) {
      if(canJumpOver(p, aPiece, aGame)) {
        auto jump = next_jump(p, aPiece, aGame);
        
        if (jump) {
          Location aCopy(*jump);
          delete jump;
          temp.push(move_val(2,aCopy));
          //std::cout << "yay we can jump\n";
          this->can_jump = true;

        }

      }
      else if(canStep(p, aPiece, aGame)){
        temp.push(move_val(1, p));
      }
      else {continue;}
    }
    
    if (!temp.empty()) {

      piece_move p(aPiece, temp.top().second);
      piece_val p_1(temp.top().first, p);
      //std::cout << "pushed: ifnotempty"<<temp.top().second.row<< " " << temp.top().second.col<<"\n";
      this->best.push(p_1);
    }

    return *this;
  }


  bool check_valid(const Location & aLoc, Game& aGame) {
    const Tile* aTile = aGame.getTileAt(aLoc);
    if (aTile)
      return true;
    else
      return false;
  }

  bool check_free(const Location & aLoc, Game& aGame) {
    
    if (!check_valid(aLoc, aGame)){
      return false;
    }
    if(aGame.getTileAt(aLoc)) {
      
      if (aGame.getTileAt(aLoc)->getPiece())
        return false;
      
      return true;
    }
    else 
      return false;
  }

  Location * next_jump(const Location & aNewLoc, const Piece * aPiece, Game& aGame) {
    
    const Location & prevLoc = aPiece->getLocation();
    MoveCalc mc(prevLoc, 2, 2);
  
    if ((prevLoc.col - aNewLoc.col) > 0)
      mc.reverseO();
    if (aPiece->getColor() == PieceColor::blue) 
      mc.reverseUpd();
    
    
    auto temp = mc.calcLocation();
    if (check_free(temp, aGame)) {
      return new Location(temp);
    }
    else 
      return nullptr;
  }

  
  locList getPawnMoves(const Piece * aPiece, Game& aGame) {
    locList pawnMoves;
    auto thisLocation = aPiece->getLocation();
    MoveCalc mc(thisLocation);
    if (aPiece->getColor() == PieceColor::blue)
      mc.reverseUpd();

    for (int it = 0; it < 2; ++it) {
      auto temp = mc.calcLocation();
      if(check_valid(temp, aGame)) {
        //std::cout <<"valid: "<<temp.row<< " "<<temp.col<<"\n";

        pawnMoves.push_back(temp);
      }
      mc.reverseO();
    }

    return pawnMoves;
  }

  locList getKingMoves(const Piece * aPiece, Game& aGame) {
    locList kingMoves = getPawnMoves(aPiece,aGame);
    auto thisLocation = aPiece->getLocation();
    MoveCalc mc(thisLocation);
    if (aPiece->getColor() == PieceColor::gold)
      mc.reverseUpd();
    for (int it = 0; it < 2; ++it) {
      auto temp = mc.calcLocation();
      if(check_valid(temp, aGame)) {
        kingMoves.push_back(temp);
      }
      mc.reverseO();
    }
    return kingMoves;

  }
  
}
