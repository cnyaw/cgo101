
//
// Copyright (c) 2008 Waync Cheng.
// All Rights Reserved.
//
// 2008/04/22 Waync.
//

#pragma once

namespace cgo {

//
// Monte Carlo.
//

class GameMonteCarlo : public CardGame
{
public:

  bool mNeedArrange;

  //
  // property.
  //

  virtual int GetName() const
  {
    return IDS_002_NAME;
  } // GetName

  virtual int GetDesc() const
  {
    return IDS_002_DESC;
  } // GetDesc

  //
  // New game.
  //

  virtual void NewGame()
  {
    CardGame::NewGame();

    for (int i = 0; i < 20; ++i) {
      mList[0].push_back(GetNewCard());
    }

    mNeedArrange = false;
  } // NewGame

  //
  // New card.
  //

  virtual bool NewCard()
  {
    mNeedArrange = false;

    //
    // Need arrange?
    //

    bool bNeedArrange = false;
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      if (-1 == mList[0][i]) {
        bNeedArrange = true;
      } else if (bNeedArrange) {
        for (int i = 0; i < (int)mList[0].size() - 1; ++i) {
          if (-1 == mList[0][i]) {
            for (int j = i + 1; j < (int)mList[0].size(); ++j) {
              if (-1 != mList[0][j]) {
                std::swap(mList[0][i], mList[0][j]);
                break;
              }
            }
          }
        }
        return true;
      }
    }

    //
    // Verify all decks are cleared or stay remain.
    //

    for (int i = 0; i < (int)mList[0].size(); ++i) {
      if (CanClear(i)) {
        return false;
      }
    }

    //
    // Deal new cards.
    //

    bool bUpdate = false;
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      if (-1 == mList[0][i]) {
        for (int j = i; j < (int)mList[0].size(); ++j) {
          if (mCards.empty()) {
            break;
          }
          mList[0][j] = GetNewCard();
          bUpdate = true;
        }
      }
    }

    return bUpdate;
  } // NewCard

  //
  // Draw game.
  //

  virtual void DrawGame(CardGameRenderer &g) const
  {
    //
    // Draw deal new card deck.
    //

    if (mCards.empty()) {
      g.DrawEmptyCard(4, 4);
    } else {
      g.DrawCover(4, 4);
    }

    //
    // Draw decks.
    //

    RECT_t rcDeck;
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      GetDeckRect(i, rcDeck);
      int card = mList[0][i];
      if (-1 == card || mSel == i) {
        g.DrawEmptyCard(rcDeck.left, rcDeck.top);
      } else {
        g.DrawCard(rcDeck.left, rcDeck.top, card);
      }
    }

    //
    // Draw selected card.
    //

    if (-1 != mSel) {
      g.DrawCard(mPtCurr.x, mPtCurr.y, mList[0][mSel]);
    }

  } // DrawGame

  //
  // Event handler.
  //

  virtual bool OnMouseDown(POINT_t point, RECT_t const& rcClient)
  {
    //
    // Deal new card or arrange?
    //

    RECT_t rcNewCard(4, 4, 4 + CARD_WIDTH, 4 + CARD_HEIGHT);
    if (rcNewCard.ptInRect(point)) {
      return NewCard();
    }

    //
    // Can move.
    //

    if (mNeedArrange) {
      return false;
    }

    //
    // Start to drag card to collapse?
    //

    RECT_t rcDeck;
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      GetDeckRect(i, rcDeck);
      if (rcDeck.ptInRect(point)) {
        mSel = i;
        mPtDown.x = point.x - rcDeck.left;
        mPtDown.y = point.y - rcDeck.top;
        break;
      }
    }

    return false;
  } // OnMouseDown

  virtual bool OnMouseUp(POINT_t point, RECT_t const& rcClient)
  {
    if (-1 != mSel) {

      RECT_t rcDeck;
      int sel = GetHitDeck(point, rcDeck);
      if (CanClear(sel, mSel)) {
        mList[0][sel] = -1;
        mList[0][mSel] = -1;
        mNeedArrange = true;
      }

      mSel = -1;
      return true;
    }

    return false;
  } // OnMouseUp

  //
  // Helper.
  //

  bool CanClear(int iDeck) const
  {
    int card = mList[0][iDeck];
    if (-1 == card) {
      return false;
    }

    int number = GetNumber(card);

    int x = iDeck % 5;
    int y = iDeck / 5;

    static int offsetxy[] = {
      0, -1, 1, -1, 1, 0, 1, 1,
      0, 1, -1, 1, -1, 0, -1, -1
    };

    for (int i = 0; i < 8; ++i) {
      if (CanClear(x + offsetxy[2 * i], y + offsetxy[2 * i + 1], number)) {
        return true;
      }
    }

    return false;
  } // CanClear

  bool CanClear(int iDeck1, int iDeck2) const
  {
    if (-1 == iDeck1 || -1 == iDeck2 || iDeck1 == iDeck2) {
      return false;
    }

    int card1 = mList[0][iDeck1];
    int card2 = mList[0][iDeck2];

    if (-1 == card1 || -1 == card2) {
      return false;
    }

    int x1 = iDeck1 % 5;
    int y1 = iDeck1 / 5;
    int x2 = iDeck2 % 5;
    int y2 = iDeck2 / 5;

    if (1 < ::abs(x1 - x2) || 1 < ::abs(y1 - y2)) {
      return false;
    }

    int number1 = GetNumber(card1);
    int number2 = GetNumber(card2);

    return number1 == number2;
  } // CanClear

  bool CanClear(int x, int y, int number) const
  {
    if (0 > x || 4 < x) {
      return false;
    }

    if (0 > y || 3 < y) {
      return false;
    }

    int iDeck = x + 5 * y;
    int card = mList[0][iDeck];

    if (-1 == card) {
      return false;
    }

    return number == GetNumber(card);
  } // CanClear

  void GetDeckRect(int iDeck, RECT_t& rcDeck) const
  {
    int x = iDeck % 5;
    int y = iDeck / 5;
    InitCardRect(rcDeck, 4 + OFFSETW * (1 + x), 4 + OFFSETH * y);
  } // GetDeckRect

  int GetHitDeck(POINT_t point, RECT_t& rcDeck) const
  {
    for (int i = 0; i < (int)mList[0].size(); ++i) {
      GetDeckRect(i, rcDeck);
      if (rcDeck.ptInRect(point)) {
        return i;
      }
    }

    return -1;
  } // GetHitDeck
};

} // namespace cgo

// end of 002_MonteCarlo.h
