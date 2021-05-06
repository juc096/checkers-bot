//
//  Player.hpp
//  Checkers
//
//  Created by rick gessner on 2/22/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Piece.hpp"
#include <map>
#include <functional>
#include <algorithm>
#include <queue>
#include <vector>
#include <utility>
#include <iostream>


namespace ECE141 {

  class Game; //forward declare...  
  using locList = std::vector<Location>;
  using piece_move = std::pair<const Piece *, Location>;
  using piece_val = std::pair<int, piece_move>;
  using move_val = std::pair<int, Location>;

  
  
  bool canStep(const Location & aLoc,const Piece * aPiece, Game& aGame );
  bool canJumpOver(const Location & aLoc,const Piece * aPiece, Game& aGame );
  bool check_valid(const Location & aLoc, Game& aGame);
  bool check_free(const Location & aLoc,Game& aGame);
  Location * next_jump(const Location & aNewLoc, const Piece * aPiece, Game& aGame);
  locList getPawnMoves(const Piece * aPiece, Game& aGame);
  locList getKingMoves(const Piece * aPiece, Game& aGame);

  static std::map< ECE141::PieceKind, std::function<locList (const ECE141::Piece *, Game& aGame)> >   
    getMoves {
      {ECE141::PieceKind::pawn, getPawnMoves },
      {ECE141::PieceKind::king, getKingMoves }};

  
  
  class Player {
  public:
                      Player();
    virtual bool      takeTurn(Game &aGame, Orientation aDirection, std::ostream &aLog);
    const PieceColor  color;
    static int        pcount; //how many created so far?
  };

  class JustincPlayer : public Player {
    public:
                      JustincPlayer() : Player() {};
  };

  struct ComparePair {
    bool operator()(const piece_val & a1, const piece_val &a2) {
      return a1.first < a2.first;
    }
  };
  struct ComparePairMove {
    bool operator()(const move_val & a1, const move_val &a2) {
      return a1.first < a2.first;
    }
  };

  struct MoveCalc {
    public:
      MoveCalc(const Location & aLoc, int anO = 1, int upd = 1) : loc(aLoc), orientation(anO), up_down(upd) {};

      Location & getLocation() {return loc;}
      MoveCalc & reverseO() {
        orientation = -orientation;
        return *this;
      }
      MoveCalc & reverseUpd() {
        up_down = -up_down;
        return *this;
      }
      Location calcLocation() {
        Location temp(loc);
        temp.col += orientation;
        temp.row += up_down;
        return temp;
      }

    private:
      Location loc;
      int orientation;
      int up_down;
  };

  class BestMove {
    public:
                      BestMove() {can_jump = false;};
      using p_queue = std::priority_queue<piece_val, std::vector<piece_val>, ComparePair>;
      BestMove & reset() {
        best = p_queue();
        can_jump = false;
        return *this;
      }
      BestMove & findBest(const Piece * aPiece, Game& aGame);
      p_queue best;
      bool can_jump;
  };
}

#endif /* Player_hpp */
