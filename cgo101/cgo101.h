
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/21 waync
//

#pragma once

#include <algorithm>
#include <vector>

namespace cgo {

#define NUM_DECK 24
#define NUM_CARDS 52
#define OFFSETW (8 + CARD_WIDTH)
#define OFFSETH (8 + CARD_HEIGHT)

#define GetColor(card) ((card) / 13)
#define GetNumber(card) (((card) % 13) + 1)

struct POINT_t
{
  POINT_t(int x_ = 0, int y_ = 0) : x(x_), y(y_)
  {
  }

  int x;
  int y;
};

struct RECT_t
{
  RECT_t(const RECT_t& rc) : left(rc.left), top(rc.top), right(rc.right), bottom(rc.bottom)
  {
  }

  RECT_t(int l = 0, int t = 0, int r = 0, int b = 0) : left(l), top(t), right(r), bottom(b)
  {
  }

  inline bool ptInRect(const POINT_t& pt) const
  {
    return left <= pt.x && right > pt.x && top <= pt.y && bottom > pt.y;
  }

  inline void offset(int xval, int yval)
  {
    left += xval;
    top += yval;
    right += xval;
    bottom += yval;
  }
  
  int left;
  int top;
  int right;
  int bottom;
};

class CardGameRenderer
{
public:

  RECT_t rcClient;

  virtual void DrawCard(int x, int y, int card) {}
  virtual void DrawCover(int x, int y) {}
  virtual void DrawEmptyCard(int x, int y) {}
  virtual void EraseBkgnd() {}
};

class CardGame
{
public:

  std::vector<int> mCards;
  std::vector<int> mList[NUM_DECK];

  int mSel;

  POINT_t mPtDown;                      // General use for dragging card.
  POINT_t mPtCurr;

  CardGame() : mSel(-1)
  {
  }

  virtual ~CardGame()
  {
  }

  //
  // Property.
  //

  virtual int GetName() const
  {
    return 0;
  }

  virtual int GetDesc() const
  {
    return 0;
  }

  //
  // New game.
  //

  virtual void NewGame()
  {
    PrepareCards();

    for (int i = 0; i < NUM_DECK; ++i) {
      mList[i].clear();
    }

    mSel = -1;
  }

  void NewGame2()
  {
    PrepareCards2();

    for (int i = 0; i < NUM_DECK; ++i) {
      mList[i].clear();
    }

    mSel = -1;
  }

  void PrepareCards()
  {
    mCards.clear();

    for (int i = 0; i < NUM_CARDS; ++i) {
      mCards.push_back(i);
    }

    std::random_shuffle(mCards.begin(), mCards.end());
  }

  void PrepareCards2()
  {
    mCards.clear();

    for (int i = 0; i < NUM_CARDS; ++i) {
      mCards.push_back(i);
    }

    for (int i = 0; i < NUM_CARDS; ++i) {
      mCards.push_back(i);
    }

    std::random_shuffle(mCards.begin(), mCards.end());
  }

  //
  // New card.
  //

  virtual bool NewCard()                // Return true if can deal new card.
  {
    return false;
  }

  int GetNewCard()
  {
    int card = mCards.front();
    mCards.erase(mCards.begin());
    return card;
  }

  int GetNewCard(int number)
  {
    for (size_t i = 0; i < mCards.size(); ++i) {
      int card = mCards[i];
      if (number == GetNumber(card)) {
        mCards.erase(mCards.begin() + i);
        return card;
      }
    }
    return -1;
  }

  int GetNewCard(int color, int number)
  {
    for (size_t i = 0; i < mCards.size(); ++i) {
      int card = mCards[i];
      if (GetColor(card) == color && GetNumber(card) == number) {
        mCards.erase(mCards.begin() + i);
        return card;
      }
    }
    return -1;
  }

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
  }

  //
  // Event handler, return true to re-draw game.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    return false;
  }

  virtual bool OnMouseMove(POINT_t point, RECT_t const& rcClient)
  {
    if (-1 != mSel) {
      mPtCurr.x = point.x - mPtDown.x;
      mPtCurr.y = point.y - mPtDown.y;
      return true;
    }
    return false;
  }

  virtual bool OnMouseUp(POINT_t point, RECT_t const& rcClient)
  {
    return false;
  }
};

} // namespace cgo

// Card Games for One 101

#include "001_Accordion.h"
#include "002_MonteCarlo.h"
#include "003_BlockEleven.h"
#include "004_Calculation.h"
#include "005_LittleSpider.h"
#include "006_PussInTheCorner.h"
#include "007_Pyramid.h"
#include "008_EasyGo.h"
#include "009_Scorpion.h"
#include "010_SirTommy.h"
#include "011_SixBySix.h"
#include "012_LeCadran.h"
#include "013_SimpleSimon.h"
#include "014_Klondike.h"
#include "015_MrsMop.h"
#include "016_RoyalParade.h"
#include "017_Martha.h"
#include "018_BakersDozen.h"
#include "019_Golf.h"
#include "020_KingAlbert.h"
#include "021_Diavolo.h"
#include "022_EightByEight.h"
#include "023_Labyrinth.h"
#include "024_Dieppe.h"
#include "025_Bisley.h"
#include "026_AboveAndBelow.h"
#include "027_EightOff.h"
#include "028_TheCarpet.h"
#include "029_StoneWall.h"
#include "030_ThreeBlindMice.h"
#include "031_Vacancies.h"
#include "032_Raglan.h"
#include "033_KingsWay.h"
#include "034_Capricious.h"

namespace cgo {

class CardGameManager
{
  CardGameManager()
  {
    mGames.push_back(new GameAccordion);
    mGames.push_back(new GameMonteCarlo);
    mGames.push_back(new GameBlockEleven);
    mGames.push_back(new GameCalculation);
    mGames.push_back(new GameLittleSpider);
    mGames.push_back(new GamePussInTheCorner);
    mGames.push_back(new GamePyramid);
    mGames.push_back(new GameEasyGo);
    mGames.push_back(new GameScorpion);
    mGames.push_back(new GameSirTommy);
    mGames.push_back(new GameSixBySix);
    mGames.push_back(new GameLeCadran);
    mGames.push_back(new GameSimpleSimon);
    mGames.push_back(new GameKlondike);
    mGames.push_back(new GameMrsMop);
    mGames.push_back(new GameRoyalParade);
    mGames.push_back(new GameMartha);
    mGames.push_back(new GameBakersDozen);
    mGames.push_back(new GameGolf);
    mGames.push_back(new GameKingAlbert);
    mGames.push_back(new GameDiavolo);
    mGames.push_back(new GameEightByEight);
    mGames.push_back(new GameLabyrinth);
    mGames.push_back(new GameDieppe);
    mGames.push_back(new GameBisley);
    mGames.push_back(new GameAboveAndBelow);
    mGames.push_back(new GameEightOff);
    mGames.push_back(new GameTheCarpet);
    mGames.push_back(new GameStoneWall);
    mGames.push_back(new GameThreeBlindMice);
    mGames.push_back(new GameVacancies);
    mGames.push_back(new GameRaglan);
    mGames.push_back(new GameKingsWay);
    mGames.push_back(new GameCapricious);
  } // CardGameManager

public:

  std::vector<CardGame*> mGames;

  ~CardGameManager()
  {
    for (int i = 0; i < (int)mGames.size(); ++i) {
      delete mGames[i];
    }
  } // ~CardGameManager

  static CardGameManager& inst()
  {
    static CardGameManager i;
    return i;
  } // inst
};

} // namespece cgo

// end of cgo101.h
